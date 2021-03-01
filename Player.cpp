#include "Player.h"
#include <iostream>

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
            break;
        case MovementDir::DOWN:
            if (!screen.IsBlockY(coords.x, coords.y - 1))
            {
                old_coords.y  = coords.y;
                coords.y     -= move_dist;
            }
            break;
        case MovementDir::LEFT:
            if (!screen.IsBlockX(coords.x - 1, coords.y))
            {
                old_coords.x  = coords.x;
                coords.x     -= move_dist;
            }
            break;
        case MovementDir::RIGHT:
            if (!screen.IsBlockX(coords.x + tileSize + 1, coords.y))
            {
                old_coords.x  = coords.x;
                coords.x     += move_dist;
            }
            break;
        default:
            break;
    }
}


void Player::Draw(Image &screen)
{
    if (Moved())
    {
        screen.PutPixels(old_coords.x, old_coords.y, backgroundColor, 0);
        old_coords = coords;
    }
    screen.PutPixels(coords.x, coords.y, Person(), 0);
}
