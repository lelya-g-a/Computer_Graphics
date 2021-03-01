#pragma once

#include "Image.h"

Pixel* Wall()
{
    Pixel* wall = new Pixel [tileSize * tileSize];

    Image img("resources/blue1.png");
    for (int i = 0; i < tileSize; ++i)
    {
        for (int j = 0; j < tileSize; ++j)
        {
            wall[tileSize * i + j] = img.Data()
                    [3 * img.Width() * (tileSize - 1 - i) + 3 * j];
        }
    }

    return wall;
}


Pixel* Teasure()
{
    Pixel* teasure = new Pixel [tileSize * tileSize];
    Pixel  color   = {255, 255, 255, 255};

    for (int i = 0; i < tileSize; ++i)
    {
        for (int j = 0; j < tileSize; ++j)
        {
            teasure[tileSize * i + j] = backgroundColor;
        }
    }

    for (int i = 6; i < 10; ++i)
    {
        for (int j = 6; j < 10; ++j)
        {
            teasure[tileSize * i + j] = color;
        }
    }

    teasure[tileSize * 6  + 6]  = backgroundColor;
    teasure[tileSize * 10 + 10] = backgroundColor;
    teasure[tileSize * 6  + 10] = backgroundColor;
    teasure[tileSize * 10 + 6]  = backgroundColor;

    return teasure;
}


Pixel* Door()
{
    Pixel* door = new Pixel [tileSize * tileSize];

    Image img("resources/blue3.png");
    for (int i = 0; i < tileSize; ++i)
    {
        for (int j = 0; j < tileSize; ++j)
        {
            door[tileSize * i + j] = img.Data()
                    [3 * img.Width() * (tileSize - 1 - i) + 3 * j];
        }
    }

    return door;
}

/*
Pixel* Person()
{
    Pixel* person = new Pixel [tileSize * tileSize];

    Image img("resources/pacman1.png");
    for (int i = 0; i < tileSize; ++i)
    {
        for (int j = 0; j < tileSize; ++j)
        {
            person[tileSize * i + j] = img.Data()
                    [10 * img.Width() * (tileSize - 1 - i) + 10 * j];
        }
    }

    return person;
}
*/
