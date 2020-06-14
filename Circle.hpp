#pragma once

#include "SFML/Graphics.hpp"

#include "Image.hpp"

struct Circle
{
    //The depth of the circle
    int depth;
    //The relative coordinates of the circle
    int relX, relY;

    //Calculates the diameter of the circle
    int CalcDiam() const;

    //Calculates the radius of the circle
    //(The circle might be a single pixel and then the radius will be 0.5, so we need a float)
    float CalcRadius() const;

    //Calculates the position of the circle in pixel coordinates
    sf::Vector2i CalcPosition() const;

    //Returns the circle id.
    //Ids are a way to assign every possible circle in the image a unique natural number.
    //Let 2 ^ n be the size of the image
    //On depth = 0 we have 1 big circle with diameter 2 ^ n. We assign that with id = 0.
    //On depth = 1 we have 4 circles with diameters 2 ^ (n - 1). We assign them with ids = 1, 2, 3, 4.
    //On depth = 2 we have 8 circles with diameters 2 ^ (n - 2). We assign them with ids = 5, ... , 12.
    //...
    //On depth = n we have 2 ^ (2 * n) circles with diameter 2 ^ (n - n) = 1, so basically just pixels.
    int GetCircleId();

    //Returns the circle corresponding to the given id
    static Circle GetCircleFromId(int id);

    //Calculates the number of all circles needed for the image size power
    static int CalcCirclesCount();

    //Finds the ids of the four circles to which the circle is divided
    static void DivideId(int circleId, int* divided);

    //Checks whether the circle with the given id contains the given point
    static bool CircleContains(int circleId, int x, int y);

    //Returns the center of the circle
    sf::Vector2f GetCenter() const;

    //Keeps track of where the ids of circles of each depth begin
    static const int IDS_BEGIN[Image::MAX_SIZEPOWER + 1];

    //For each depth keeps track of the total number of circles up to that depth
    static const int* const DEPTH_SUMS;

    //A factor for how precise we want to draw circles.
    //All circles are of size that is a power of 2,
    //so if a circle has a diameter 2^n,
    //then the number of points used to draw it will be n times this factor
    static const int DRAW_PRECISION = 20;

    //The minimum diameter power allowed for a circle's diameter
    static const int MIN_DIAMPOWER = 2;

    //The minimum diameter in pixels allowed for a circle
    static const int MIN_DIAM = (1 << MIN_DIAMPOWER);

    //The maximum number of active circles in any moment
    static const int MAX_ACTIVE = 1 << ((Image::SIZEPOWER - Circle::MIN_DIAMPOWER) * 2);
};