#pragma once

#include <SFML/Graphics.hpp>

#include "CircularSet.hpp"
#include "Circle.hpp"

class Game
{
  public:

    //Creates a new game
    Game();

    //Runs the game
    void Run();

    //Frees the memory
    ~Game();

  private:

    //The window where the game is rendered
    sf::RenderWindow m_window;

    //An array of the colors of every possible circle in the image
    sf::Color *m_circleColors;
    int m_circlesCount;
    //A set of the ids of the currently active circles
    CircularSet<int, Circle::MAX_ACTIVE> m_activeCircles;

    //The time (frames) passed from the last key press.
    int m_keyTime;
    static const int KEY_DELAY = 8;

    //When a circle is divided, we should not divide the smaller circles immediately.
    //Instead we are locked in the small circle containing the mouse, until we get out of it
    int m_lockedCircleId;
    //Indicates whether we are currently locked in a circle
    bool m_locked;

  private:

    //Updates the game according to user's input
    void Update();

    //Renders the game to the window
    void Draw();

    //Draws the circle with the given id to the window
    void DrawCircle(int circleId);

    //Divides the given active circle (its index in active circles) into 4 smaller circles.
    //The big circles becomes unactive, the 4 smaller circles become active
    void DivideCircle(int circleInd, const sf::Vector2i& mouse);

    //Divides every active circle into 4 smaller circles
    void DivideAllCircles();

    //Finds an active circle that contains the given point, returns circle's index in active circles
    int FindActiveCircle(int x, int y) const;

    //Calculates the colors of each possible circle
    void FillCircleColors(const sf::Image&);

    //Calculates the average color of the pixels in the specified region in the image
    static sf::Color CalcAvgColor(const sf::Image&, int x, int y, int width, int height);

    //Calculates the average color of an array of colors
    static sf::Color CalcAvgColor(const sf::Color*, int colorsCount);
};