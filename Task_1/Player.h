#pragma once

#include "Image.h"

struct Point
{
    int x;
    int y;
};


enum class MovementDir
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};


struct Player
{
    explicit Player(Point pos = {.x = 10, .y = 10}) :
        coords     (pos), 
        old_coords (coords) 
    {};

    bool Moved() const;
    void ProcessInput(MovementDir dir, Image &screen);
    void Draw(Image &screen);

private:
    Point coords     {.x = 10, .y = 10};
    Point old_coords {.x = 10, .y = 10};
    Pixel color      {.r = 255, .g = 255, .b = 0, .a = 255};
    int   move_speed = 2;
};