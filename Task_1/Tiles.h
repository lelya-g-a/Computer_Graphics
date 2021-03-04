#pragma once

#include "Image.h"
#include <string>

class Tiles
{
private:
    Pixel*      picture = nullptr;
    std::string file;
    int         scale;

public:
    void SetPic(const std::string a_file, int a_scale) 
    {
        picture = new Pixel [tileSize * tileSize];
        file    = a_file;
        scale   = a_scale;
    }

    Pixel* Pic()
    {
        Image img(file);
        for (int i = 0; i < tileSize; ++i)
        {
            for (int j = 0; j < tileSize; ++j)
            {
                picture[tileSize * i + j] = backgroundColor;
                if (img.Data()[scale * img.Width() * (tileSize - 1 - i) 
                    + scale * j].a != 0) 
                {
                    picture[tileSize * i + j] = img.Data()
                        [scale * img.Width() * (tileSize - 1 - i) + 
                         scale * j];
                }
            }
        }
        return picture;
    }
    
    ~Tiles()
    {
        delete [] picture;
    }
};
