#pragma once

struct Image
{
    //The name of the file of the image that we want to use
    static const char* FILENAME;

    //The size power of the image.
    //We allow only square images of size that is a power of 2,
    //so if the size power is n, the image should be 2^n by 2^n pixels
    static const int SIZEPOWER = 10;

    //The max size power allowed for an image
    static const int MAX_SIZEPOWER = 15;

    //The image's size in pixels
    static const int SIZE = (1 << SIZEPOWER);

    //The max size in pixels allowed for an image
    static const int MAX_SIZE = (1 << MAX_SIZEPOWER);
};