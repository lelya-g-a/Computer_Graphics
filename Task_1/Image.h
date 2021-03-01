#pragma once

#include <string>
#include <fstream>
#include <iostream>

constexpr int tileSize = 16;

struct Pixel
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
};

constexpr Pixel backgroundColor{0, 0, 0, 255};

struct Image
{
    explicit Image(const std::string &a_path);
    Image(int a_width, int a_height, int a_channels, 
          const std::string &a_file);

    int    Save     (const std::string &a_path);

    int    Width    () const {return width;}
    int    Height   () const {return height;}
    int    Channels () const {return channels;}
    size_t Size     () const {return size;}
    Pixel* Data     ()       {return data;}


    Pixel GetPixel(int x, int y) 
    {
        return data[width * y + x];
    }

    
    void  PutPixel(int x, int y, const Pixel &pix) 
    {
        data[width * y + x] = pix;
    }


    void PutPixels(int x, int y, const Pixel &pix, bool isBlock = 0)
    {
        for (int size_y = 0; size_y < tileSize; ++size_y)
        {
            for (int size_x = 0; size_x < tileSize; ++size_x)
            {
                data [width * (y + size_y) + (x + size_x)] = pix;
                block[width * (y + size_y) + (x + size_x)] = isBlock;
            }
        }
    }

    void PutPixels(int x, int y, Pixel* pix, bool isBlock = 0)
    {
        for (int size_y = 0; size_y < tileSize; ++size_y)
        {
            for (int size_x = 0; size_x < tileSize; ++size_x)
            {
                data [width * (y + size_y) + (x + size_x)] = 
                    pix[tileSize * size_y + size_x];
                block[width * (y + size_y) + (x + size_x)] = isBlock;
            }
        }
    }
    
    
    // Is there a block on the top or bottom
    bool IsBlockY(int x, int y)
    {
        return (block[width * y + x] || block[width * y + x + tileSize - 1]);
    }


    // Is there a block on the right or left
    bool IsBlockX(int x, int y)
    {
        return (block[width * y + x] || block[width * (y + tileSize - 1) + x]);
    }


    int XCoord()
    {
        return xCoordPlayer;
    }


    int YCoord()
    {
        return yCoordPlayer;
    }


    ~Image();

private:
    int    width          = -1;
    int    height         = -1;
    int    channels       = 3;
    size_t size           = 0;
    Pixel* data           = nullptr;
    bool*  block          = nullptr;
    bool   self_allocated = false;

    int    xCoordPlayer   = -1;
    int    yCoordPlayer   = -1;

    void ReadFile (const std::string &a_file);
};
