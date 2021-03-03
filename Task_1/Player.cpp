#include "Player.h"
#include <iostream>


bool Player::Moved() const
{
    if (coords.x == old_coords.x && 
        coords.y == old_coords.y)
        return false;
    else
        return true;
}


void Player::ProcessInput(MovementDir dir, Image &screen)
{
    int move_dist = move_speed * 1;
    switch(dir)
    {
        case MovementDir::UP:  
            if (!screen.IsBlockY(coords.x, coords.y + tileSize + 1))
            {
                old_coords.y  = coords.y;
                coords.y     += move_dist;
            }
            if (screen.IsEmptyY(coords.x, coords.y + tileSize))
            {
                Image img ("resources/game_over.png");
                for (int i = 0; i < 320; ++i)
                {
                    for (int j = 0; j < 320; ++j)
                    {
                        screen.PutPixel(j + tileSize, i + tileSize, 
                            img.Data()[img.Width() * (319 - i) +  j]);
                    }
                }
                coords.x = -1;
                coords.y = -1;
            }
            break;
        case MovementDir::DOWN:
            if (!screen.IsBlockY(coords.x, coords.y - 1))
            {
                old_coords.y  = coords.y;
                coords.y     -= move_dist;
            }
            if (screen.IsEmptyY(coords.x, coords.y))
            {
                Image img ("resources/game_over.png");
                for (int i = 0; i < 320; ++i)
                {
                    for (int j = 0; j < 320; ++j)
                    {
                        screen.PutPixel(j + tileSize, i + tileSize, 
                            img.Data()[img.Width() * (319 - i) +  j]);
                    }
                }
                coords.x = -1;
                coords.y = -1;
            }
            break;
        case MovementDir::LEFT:
            if (!screen.IsBlockX(coords.x - 1, coords.y))
            {
                old_coords.x  = coords.x;
                coords.x     -= move_dist;
            }
            if (screen.IsEmptyX(coords.x, coords.y))
            {
                Image img ("resources/game_over.png");
                for (int i = 0; i < 320; ++i)
                {
                    for (int j = 0; j < 320; ++j)
                    {
                        screen.PutPixel(j + tileSize, i + tileSize, 
                            img.Data()[img.Width() * (319 - i) +  j]);
                    }
                }
                coords.x = -1;
                coords.y = -1;
            }
            break;
        case MovementDir::RIGHT:
            if (!screen.IsBlockX(coords.x + tileSize + 1, coords.y))
            {
                old_coords.x  = coords.x;
                coords.x     += move_dist;
            }
            if (screen.IsEmptyX(coords.x, coords.y))
            {
                Image img ("resources/game_over.png");
                for (int i = 0; i < 320; ++i)
                {
                    for (int j = 0; j < 320; ++j)
                    {
                        screen.PutPixel(j + tileSize, i + tileSize, 
                            img.Data()[img.Width() * (319 - i) +  j]);
                    }
                }
                coords.x = -1;
                coords.y = -1;
            }
            break;
        case MovementDir::ACTION:
            int x;
            int y;
            
            // Action for treasure
            
            // UP
            if (screen.IsTreasureY(coords.x, coords.y + tileSize + 1))
            {
                x = coords.x;
                y = coords.y + tileSize;
            }
            // DOWN
            else if (screen.IsTreasureY(coords.x, coords.y - 1))
            {
                x = coords.x;
                y = coords.y - tileSize;
            }
            // LEFT
            else if (screen.IsTreasureX(coords.x - 1, coords.y))
            {
                x = coords.x - tileSize;
                y = coords.y;
            }
            // RIGHT
            else if (screen.IsTreasureX(coords.x + tileSize + 1, 
                                        coords.y))
            {
                x = coords.x + tileSize;
                y = coords.y;
            }
            else 
            {
                x = -1;
                y = -1;
            }
            
            if ((x >= 0) && (y >= 0))
            {
                //Tiles treasure("resources/tr1.png", 4);
                screen.PutPixels(x, y, backgroundColor, '.');
            }
            
            
            // Action for door
            
            int next;
            
            // UP
            if (screen.IsDoorY(coords.x, coords.y + tileSize + 1))
            {
                x = coords.x;
                y = coords.y + tileSize;
                next = -5;
            }
            // DOWN
            else if (screen.IsDoorY(coords.x, coords.y - 1))
            {
                x = coords.x;
                y = coords.y - tileSize;
                next = 5;
            }
            // LEFT
            else if (screen.IsDoorX(coords.x - 1, coords.y))
            {
                x = coords.x - tileSize;
                y = coords.y;
                next = -1;
            }
            // RIGHT
            else if (screen.IsDoorX(coords.x + tileSize + 1, 
                                        coords.y))
            {
                x = coords.x + tileSize;
                y = coords.y;
                next = 1;
            }
            else 
            {
                x = -1;
                y = -1;
            }
            
            if ((x >= 0) && (y >= 0))
            {
                //Tiles treasure;
                //treasure.SetPic("resources/door1.png", 13);
                //screen.PutPixels(x, y, treasure.Pic(), 'x');
                screen.NewRoom(next);
                screen.ReadFile(screen.Room(), screen.Type());
                old_coords.x  = coords.x;
                coords.x      = screen.XCoord();
                old_coords.y  = coords.y;
                coords.y      = screen.YCoord();
            }
            
            
            // Action for Exit
            
            if (screen.IsExit(coords.x, coords.y))
            {
                Image img ("resources/game_over.png");
                for (int i = 0; i < 320; ++i)
                {
                    for (int j = 0; j < 320; ++j)
                    {
                        screen.PutPixel(j + tileSize, i + tileSize, 
                            img.Data()[img.Width() * (319 - i) +  j]);
                    }
                }
                coords.x = -1;
                coords.y = -1;
            }
            
            break;
        default:
            break;
    }
}


void Player::Draw(Image &screen)
{
    if ((coords.x >= 0) && (coords.y >= 0))
    {
        Tiles person;
        person.SetPic("resources/pacman1.png", 10);
        if (Moved())
        {
            screen.PutPixels(old_coords.x, old_coords.y, 
                             backgroundColor, '.');
            old_coords = coords;
        }
    
        screen.PutPixels(coords.x, coords.y, person.Pic(), '.');
    }
}
