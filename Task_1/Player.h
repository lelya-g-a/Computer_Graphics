#pragma once

#include "Image.h"
#include "Tiles.h"

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
    RIGHT,
    ACTION
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
    
    int GetX () {return coords.x;}
    int GetY () {return coords.y;}
    
    void SetCoords (int x, int y) 
    {
        coords.x = x;
        coords.y = y;
    }
    
    int IsNewRoom() 
    {
        int tmp = newRoom;
        newRoom = 0;
        return tmp;
    }
    
    bool IsLife()
    {
        bool tmp = lifeFlag;
        lifeFlag = 0;
        return tmp;
    }

private:
    Point coords     {.x = 10, .y = 10};
    Point old_coords {.x = 10, .y = 10};
    Pixel color      {.r = 255, .g = 255, .b = 0, .a = 255};
    
    std::string positions [4] = {"resources/person0.png",
                                 "resources/person1.png",
                                 "resources/person2.png",
                                 "resources/person3.png"};
    int  position   = 0; 
    int  newRoom    = 0;
    int  move_speed = 2;
    int  lifes      = 3;
    bool lifeFlag   = 0;
    bool trapFlag   = 0;
};
