#include "Game.hpp"

#include "Image.hpp"
#include "Circle.hpp"

#include <iostream>

Game::Game()
    : m_window(sf::VideoMode(Image::SIZE, Image::SIZE), "Image Circles")
{
    m_window.setFramerateLimit(60);

    sf::Image img;
    img.loadFromFile(Image::FILENAME);

    this->FillCircleColors(img);

    m_activeCircles.Add(0);

    m_keyTime = 0;

    m_locked = false;
}

void Game::Run()
{
    //The game loop
    while (m_window.isOpen())
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            m_window.close();
        }

        m_window.clear();

        this->Update();

        this->Draw();

        m_window.display();
    }
}

void Game::Update()
{
    sf::Vector2i mouse = sf::Mouse::getPosition(m_window);

    //Find the active circle containing the mouse
    int circleId = this->FindActiveCircle(mouse.x, mouse.y);
    if (circleId != -1)
    {
        this->DivideCircle(circleId, mouse);
    }

    if (m_locked && !Circle::CircleContains(m_lockedCircleId, mouse.x, mouse.y))
    {
        m_locked = false;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && m_keyTime >= Game::KEY_DELAY)
    {
        this->DivideAllCircles();
        m_keyTime = 0;
    }

    m_keyTime++;
}

void Game::Draw()
{
    for (int i = 0; i < m_activeCircles.GetSize(); i++)
    {
        this->DrawCircle(m_activeCircles[i]);
    }
}

void Game::DrawCircle(int circleId)
{
    //Find the circle corresponding to that id
    Circle circle = Circle::GetCircleFromId(circleId);
    //Create a circle shape with the circle's radius
    sf::CircleShape shape(circle.CalcRadius(), (Image::SIZEPOWER - circle.depth) * Circle::DRAW_PRECISION);
    //Set the circle shape position
    sf::Vector2i circlePosition = circle.CalcPosition();
    shape.setPosition(circlePosition.x, circlePosition.y);
    //Set the circle shape color
    shape.setFillColor(m_circleColors[circleId]);

    //Draw the circle shape to the window
    m_window.draw(shape);
}

void Game::DivideCircle(int circleInd, const sf::Vector2i& mouse)
{
    //Divide the circle into 4 smaller circles
    int divided[4];
    Circle::DivideId(m_activeCircles[circleInd], divided);

    //Check if the divided circles are within the allowed circles.
    //Checking only one of them is enough, because they will be with the same depth
    if (divided[0] >= m_circlesCount)
        return;

    //Make the big circle inactive
    m_activeCircles.RemoveAt(circleInd);

    //Make the 4 smaller circles active
    for (int i = 0; i < 4; i++)
        m_activeCircles.Add(divided[i]);

    for (int i = 0; i < 4; i++)
    {
        if (Circle::CircleContains(divided[i], mouse.x, mouse.y))
        {
            m_lockedCircleId = divided[i];
            m_locked = true;
            break;
        }
    }
}

void Game::DivideAllCircles()
{
    //Copy the currently active circles
    int oldActivesCount = m_activeCircles.GetSize();
    int* oldActives = new int[oldActivesCount];
    for (int i = 0; i < oldActivesCount; i++)
    {
        oldActives[i] = m_activeCircles[i];
    }

    m_activeCircles.Clear();
    //Divide each one into 4 smaller circles
    for (int i = 0; i < oldActivesCount; i++)
    {
        int oldActive = oldActives[i];

        //Divide the old active circle into 4 smaller circles
        int divided[4];
        Circle::DivideId(oldActive, divided);

        //If the four smaller circles are too small, keep the old one
        if (divided[0] >= m_circlesCount)
        {
            m_activeCircles.Add(oldActive);
            continue;
        }
        //Otherwise activate the 4 smaller ones
        else
        {
            for (int i = 0; i < 4; i++)
            {
                m_activeCircles.Add(divided[i]);
            }
        }
    }
}

int Game::FindActiveCircle(int x, int y) const
{
    if (m_locked)
    {
        return -1;
    }

    for (int i = 0; i < m_activeCircles.GetSize(); i++)
    {
        if (Circle::CircleContains(m_activeCircles[i], x, y))
        {
            return i;
        }
    }

    return -1;
}

void Game::FillCircleColors(const sf::Image& img)
{
    //Allocate memory for all the possible circles
    m_circlesCount = Circle::CalcCirclesCount();
    m_circleColors = new sf::Color[m_circlesCount];

    //Calculate the colors of circles in the last depth allowed
    Circle circle;
    circle.depth = Image::SIZEPOWER - Circle::MIN_DIAMPOWER;
    int relSize = (1 << circle.depth);
    for (circle.relX = 0; circle.relX < relSize; circle.relX++)
    {
        for (circle.relY = 0; circle.relY < relSize; circle.relY++)
        {
            sf::Vector2i pixelPosition = circle.CalcPosition();
            int diam = circle.CalcDiam();

            sf::Color avgColor = Game::CalcAvgColor(img, pixelPosition.x, pixelPosition.y, diam, diam);

            int circleId = circle.GetCircleId();
            m_circleColors[circleId] = avgColor;
        }
    }

    //Calculate colors of circles in all previous depths
    for (circle.depth -= 1; circle.depth >= 0; circle.depth--)
    {
        relSize >>= 1;
        for (circle.relX = 0; circle.relX < relSize; circle.relX++)
        {
            for (circle.relY = 0; circle.relY < relSize; circle.relY++)
            {
                int circleId = circle.GetCircleId();

                //Divide the circle into four smaller circles
                int divideIds[4];
                Circle::DivideId(circleId, divideIds);

                //Find the average color of the four smaller circles
                sf::Color colors[4];
                for (int i = 0; i < 4; i++)
                    colors[i] = m_circleColors[divideIds[i]];
                sf::Color avgColor = Game::CalcAvgColor(colors, 4);

                m_circleColors[circleId] = avgColor;
            }
        }
    }
}

sf::Color Game::CalcAvgColor(const sf::Image& img, int left, int top, int width, int height)
{
    long long unsigned rSum = 0, gSum = 0, bSum = 0;
    for (int x = left; x < left + width; x++)
    {
        for (int y = top; y < top + height; y++)
        {
            rSum += img.getPixel(x, y).r;
            gSum += img.getPixel(x, y).g;
            bSum += img.getPixel(x, y).b;
        }
    }
    int totalPixels = width * height;

    sf::Uint8 rAvg = rSum / totalPixels;
    sf::Uint8 gAvg = gSum / totalPixels;
    sf::Uint8 bAvg = bSum / totalPixels;
    return {rAvg, gAvg, bAvg};
}

sf::Color Game::CalcAvgColor(const sf::Color* colors, int colorsCount)
{
    long long unsigned rSum = 0, gSum = 0, bSum = 0;
    for (int i = 0; i < colorsCount; i++)
    {
        rSum += colors[i].r;
        gSum += colors[i].g;
        bSum += colors[i].b;
    }

    sf::Uint8 rAvg = rSum / colorsCount;
    sf::Uint8 gAvg = gSum / colorsCount;
    sf::Uint8 bAvg = bSum / colorsCount;
    return {rAvg, gAvg, bAvg};
}