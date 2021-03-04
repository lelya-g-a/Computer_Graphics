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
                lifes -= 1;
                if (lifes == 0)
                {
                    Image img ("resources/game_over.png");
                    for (int i = 0; i < 320; ++i)
                    {
                        for (int j = 0; j < 320; ++j)
                        {
                            if (img.Data()[img.Width() * 
                                (319 - i) +  j].a == 0)
                            {
                                screen.PutPixel(j + tileSize, 
                                                i + tileSize, 
                                backgroundColor);
                            }
                            else
                            {
                                screen.PutPixel(j + tileSize, 
                                                i + tileSize, 
                                img.Data()[img.Width() * 
                                                       (319 - i) +  j]);
                            }
                        }
                    }
                    coords.x = -1;
                    coords.y = -1;
                }
                else
                {   
                    coords.x = screen.XCoord();
                    coords.y = screen.YCoord();
                    
                    lifeFlag = 1;
                    trapFlag = 1;
                }
                
                int x = screen.Width() / tileSize - 1;
                screen.PutPixels((x * tileSize), 
                                 (2 * (lifes + 1) * tileSize), 
                                 backgroundColor,
                                 ' ');
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
                lifes -= 1;
                if (lifes == 0)
                {
                    Image img ("resources/game_over.png");
                    for (int i = 0; i < 320; ++i)
                    {
                        for (int j = 0; j < 320; ++j)
                        {
                            if (img.Data()[img.Width() * 
                                (319 - i) +  j].a == 0)
                            {
                                screen.PutPixel(j + tileSize, 
                                                i + tileSize, 
                                backgroundColor);
                            }
                            else
                            {
                                screen.PutPixel(j + tileSize, 
                                                i + tileSize, 
                                img.Data()[img.Width() * 
                                                       (319 - i) +  j]);
                            }
                        }
                    }
                    coords.x = -1;
                    coords.y = -1;
                }
                else
                {
                    coords.x = screen.XCoord();
                    coords.y = screen.YCoord();
                    
                    lifeFlag = 1;
                    trapFlag = 1;
                }
                
                int x = screen.Width() / tileSize - 1;
                screen.PutPixels((x * tileSize), 
                                 (2 * (lifes + 1) * tileSize), 
                                 backgroundColor,
                                 ' ');
            }
            break;
        case MovementDir::LEFT:
            if (!trapFlag)
            {
                if (!screen.IsBlockX(coords.x - 1, coords.y))
                {
                    old_coords.x  = coords.x;
                    coords.x     -= move_dist;
                }
                if (screen.IsEmptyX(coords.x, coords.y))
                {
                    lifes -= 1;
                    if (lifes == 0)
                    {
                        Image img ("resources/game_over.png");
                        for (int i = 0; i < 320; ++i)
                        {
                            for (int j = 0; j < 320; ++j)
                            {
                                if (img.Data()[img.Width() * 
                                    (319 - i) +  j].a == 0)
                                {
                                    screen.PutPixel(j + tileSize, 
                                                    i + tileSize, 
                                    backgroundColor);
                                }
                                else
                                {
                                    screen.PutPixel(j + tileSize, 
                                                    i + tileSize, 
                                    img.Data()[img.Width() * 
                                                    (319 - i) +  j]);
                                }
                            }
                        }
                        coords.x = -1;
                        coords.y = -1;
                    }
                    else
                    {
                        coords.x = screen.XCoord();
                        coords.y = screen.YCoord();
                        
                        lifeFlag = 1;
                    }
                    
                    int x = screen.Width() / tileSize - 1;
                    screen.PutPixels((x * tileSize), 
                                     (2 * (lifes + 1) * tileSize), 
                                     backgroundColor,
                                     ' ');
                }
            }
            else 
            {
                trapFlag = 0;
            }
            break;
        case MovementDir::RIGHT:
            if (!trapFlag)
            {
                if (!screen.IsBlockX(coords.x + tileSize + 1, coords.y))
                {
                    old_coords.x  = coords.x;
                    coords.x     += move_dist;
                }
                if (screen.IsEmptyX(coords.x + tileSize, coords.y))
                {
                    lifes -= 1;
                    if (lifes == 0)
                    {
                        Image img ("resources/game_over.png");
                        for (int i = 0; i < 320; ++i)
                        {
                            for (int j = 0; j < 320; ++j)
                            {
                                if (img.Data()[img.Width() * 
                                    (319 - i) +  j].a == 0)
                                {
                                    screen.PutPixel(j + tileSize, 
                                                    i + tileSize, 
                                    backgroundColor);
                                }
                                else
                                {
                                    screen.PutPixel(j + tileSize, 
                                                    i + tileSize, 
                                    img.Data()[img.Width() * 
                                                    (319 - i) +  j]);
                                }
                            }
                        }
                        
                        coords.x = -1;
                        coords.y = -1;
                    }
                    else
                    {
                        coords.x = screen.XCoord();
                        coords.y = screen.YCoord();
                        
                        lifeFlag = 1;
                    }
                    
                    int x = screen.Width() / tileSize - 1;
                    screen.PutPixels((x * tileSize), 
                                     (2 * (lifes + 1) * tileSize), 
                                     backgroundColor,
                                     ' ');
                }
            }
            else 
            {
                trapFlag = 0;
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
                Tiles treasure;
                treasure.SetPic("resources/treas2.png", 1);
                screen.PutPixels(x, y, treasure.Pic(), '#');
                if (lifes < 5)
                {
                    lifes += 1;
                    Tiles life;
                    life.SetPic("resources/life.png", 1);
                    
                    int h = screen.Width() / tileSize - 1;
                    screen.PutPixels((h * tileSize), 
                                     (2 * lifes * tileSize), 
                                     life.Pic(),
                                     ' ');
                }
            }
            
            
            // Action for trap
            
            // UP
            if (screen.IsTrapY(coords.x, coords.y + tileSize + 1))
            {
                x = coords.x;
                y = coords.y + tileSize;
            }
            // DOWN
            else if (screen.IsTrapY(coords.x, coords.y - 1))
            {
                x = coords.x;
                y = coords.y - tileSize;
            }
            // LEFT
            else if (screen.IsTrapX(coords.x - 1, coords.y))
            {
                x = coords.x - tileSize;
                y = coords.y;
            }
            // RIGHT
            else if (screen.IsTrapX(coords.x + tileSize + 1, 
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
                Tiles trap;
                trap.SetPic("resources/trap2.png", 1);
                screen.PutPixels(x, y, trap.Pic(), '#');
                
                lifes -= 1;
                if (lifes == 0)
                {
                    Image img ("resources/game_over.png");
                    for (int i = 0; i < 320; ++i)
                    {
                        for (int j = 0; j < 320; ++j)
                        {
                            if (img.Data()[img.Width() * 
                                (319 - i) +  j].a == 0)
                            {
                                screen.PutPixel(j + tileSize, 
                                                i + tileSize, 
                                backgroundColor);
                            }
                            else
                            {
                                screen.PutPixel(j + tileSize, 
                                                i + tileSize, 
                                img.Data()[img.Width() * 
                                                       (319 - i) +  j]);
                            }
                        }
                    }
                    coords.x = -1;
                    coords.y = -1;
                }
                else
                {
                    lifeFlag = 1;
                }
                
                int x = screen.Width() / tileSize - 1;
                screen.PutPixels((x * tileSize), 
                                 (2 * (lifes + 1) * tileSize), 
                                 backgroundColor,
                                 ' ');
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
                Tiles treasure;
                treasure.SetPic("resources/door1.png", 13);
                screen.PutPixels(x, y, treasure.Pic(), 'x');
                newRoom = next;
                
                old_coords.x = coords.x;
                old_coords.y = coords.y;
                coords.x = screen.XCoord();
                coords.y = screen.YCoord();
            }
            
            
            // Action for Exit
            
            if (screen.IsExit(coords.x, coords.y))
            {
                Image img ("resources/win.png");
                for (int i = 0; i < 320; ++i)
                {
                    for (int j = 0; j < 320; ++j)
                    {
                        if (img.Data()[img.Width() * 
                            (319 - i) +  j].a == 0)
                        {
                            screen.PutPixel(j + tileSize, i + tileSize, 
                            backgroundColor);
                        }
                        else
                        {
                            screen.PutPixel(j + tileSize, i + tileSize, 
                            img.Data()[img.Width() * (319 - i) +  j]);
                        }
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
    if ((coords.x >= 0) && (coords.y >= 0) && (newRoom == 0))
    {
        Tiles person;
        
        if (Moved())
        {
            screen.PutPixels(old_coords.x, old_coords.y, 
                             backgroundColor, '.');
            old_coords = coords;
            position = (position + 1) % 8;
        }
        else 
        {
            position = 0;
        }
        
        Tiles treasure;
        treasure.SetPic(screen.Treas(screen.TreasCount() / 10), 1);
        screen.TreasNext();
        for (int i = 0; i < screen.TreasLength(); ++i)
        {
            if (screen.IsTreasure(screen.TreasCoordX(i), 
                                  screen.TreasCoordY(i)))
            {
                screen.PutPixels(screen.TreasCoordX(i), 
                                 screen.TreasCoordY(i), 
                                 treasure.Pic(), 'G');
            }
        }
        
        Tiles trap;
        trap.SetPic(screen.Trap(screen.TrapCount() / 10), 1);
        screen.TrapNext();
        for (int i = 0; i < screen.TrapLength(); ++i)
        {
            if (screen.IsTrap(screen.TrapCoordX(i), 
                              screen.TrapCoordY(i)))
            {
                screen.PutPixels(screen.TrapCoordX(i), 
                                 screen.TrapCoordY(i), 
                                 trap.Pic(), 'T');
            }
        }
        
        person.SetPic(positions[position / 2], 1);
        screen.PutPixels(coords.x, coords.y, person.Pic(), '.');
    }
}
