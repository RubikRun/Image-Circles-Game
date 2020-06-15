#include "Circle.hpp"

int Circle::CalcDiam() const
{
    return 1 << (Image::SIZEPOWER - this->depth);
}

float Circle::CalcRadius() const
{
    return (float)this->CalcDiam() / 2;
}

sf::Vector2i Circle::CalcPosition() const
{
    int diam = this->CalcDiam();
    return {relX * diam, relY * diam};
}

int Circle::GetCircleId()
{
    //The id of the first circle with this depth
    int idsBegin = IDS_BEGIN[this->depth];

    //The number of circles per row on this depth
    int perRow = (1 << this->depth);

    return idsBegin + this->relY * perRow + this->relX;
}

Circle Circle::GetCircleFromId(int id)
{
    Circle circle;

    //Find the circle's depth
    circle.depth = 0;
    while (IDS_BEGIN[circle.depth + 1] <= id)
    {
        circle.depth++;
    }
    int idsBegin = IDS_BEGIN[circle.depth];

    //The number of circles per row on this depth
    int perRow = (1 << circle.depth);

    //Find circle's relative coordinates
    circle.relX = (id - idsBegin) % perRow;
    circle.relY = (id - idsBegin) / perRow;

    return circle;
}

int Circle::CalcCirclesCount()
{
    //The maximum possible depth is the depth of the image, its size power.
    //But we have a minimum diameter allowed for a circle, so that's another upper bound of the depth.
    //So the maximum allowed depth should be the depth of the image minus the minimum diameter power
    return Circle::DEPTH_SUMS[Image::SIZEPOWER - Circle::MIN_DIAMPOWER];
}

void Circle::DivideId(int circleId, int* divided)
{
    Circle circle = Circle::GetCircleFromId(circleId);
    Circle smallerCircle = {circle.depth + 1, circle.relX * 2, circle.relY * 2};

    divided[0] = smallerCircle.GetCircleId();
    smallerCircle.relX++;
    divided[1] = smallerCircle.GetCircleId();
    smallerCircle.relX--; smallerCircle.relY++;
    divided[2] = smallerCircle.GetCircleId();
    smallerCircle.relX++;
    divided[3] = smallerCircle.GetCircleId();
}

bool Circle::CircleContains(int circleId, int x, int y)
{
    Circle circle = Circle::GetCircleFromId(circleId);

    sf::Vector2i circlePosition = circle.CalcPosition();
    float radius = circle.CalcRadius();

    sf::Vector2f center = {circlePosition.x + radius, circlePosition.y + radius};

    return (
        (x - center.x) * (x - center.x) + (y - center.y) * (y - center.y) <= radius * radius
    );
}

sf::Vector2f Circle::GetCenter() const
{
    sf::Vector2i circlePosition = this->CalcPosition();
    float radius = this->CalcRadius();

    return {circlePosition.x + radius, circlePosition.y + radius};
}

const int Circle::IDS_BEGIN[Image::MAX_SIZEPOWER  + 1]
{
    0,
    1*1,
    1*1 + 2*2,
    1*1 + 2*2 + 4*4,
    1*1 + 2*2 + 4*4 + 8*8,
    1*1 + 2*2 + 4*4 + 8*8 + 16*16,
    1*1 + 2*2 + 4*4 + 8*8 + 16*16 + 32*32,
    1*1 + 2*2 + 4*4 + 8*8 + 16*16 + 32*32 + 64*64,
    1*1 + 2*2 + 4*4 + 8*8 + 16*16 + 32*32 + 64*64 + 128*128,
    1*1 + 2*2 + 4*4 + 8*8 + 16*16 + 32*32 + 64*64 + 128*128 + 256*256,
    1*1 + 2*2 + 4*4 + 8*8 + 16*16 + 32*32 + 64*64 + 128*128 + 256*256 + 512*512,
    1*1 + 2*2 + 4*4 + 8*8 + 16*16 + 32*32 + 64*64 + 128*128 + 256*256 + 512*512 + 1024*1024,
    1*1 + 2*2 + 4*4 + 8*8 + 16*16 + 32*32 + 64*64 + 128*128 + 256*256 + 512*512 + 1024*1024 + 2048*2048,
    1*1 + 2*2 + 4*4 + 8*8 + 16*16 + 32*32 + 64*64 + 128*128 + 256*256 + 512*512 + 1024*1024 + 2048*2048 + 4096*4096,
    1*1 + 2*2 + 4*4 + 8*8 + 16*16 + 32*32 + 64*64 + 128*128 + 256*256 + 512*512 + 1024*1024 + 2048*2048 + 4096*4096 + 8192*8192,
    1*1 + 2*2 + 4*4 + 8*8 + 16*16 + 32*32 + 64*64 + 128*128 + 256*256 + 512*512 + 1024*1024 + 2048*2048 + 4096*4096 + 8192*8192 + 16384*16384
};

//Ids for each depth begin on the sum of previous depths, so no need to keep that same information twice
const int* const Circle::DEPTH_SUMS = Circle::IDS_BEGIN + 1;