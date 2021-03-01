#include "Image.h"
#include "Tiles.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>


Image::Image(const std::string &a_path)
{
    if ((data = (Pixel*)stbi_load(a_path.c_str(), &width, 
                                  &height, &channels, 0)) != nullptr)
    {
        size = width * height * channels;
    }
}


Image::Image(int a_width, int a_height, int a_channels, 
             const std::string &a_file)
{
    data  = new Pixel [a_width * a_height];
    block = new bool  [a_width * a_height];

    if (data != nullptr)
    {
        width          = a_width;
        height         = a_height;
        size           = a_width * a_height * a_channels;
        channels       = a_channels;
        self_allocated = true;
    }

    ReadFile(a_file);
}


int Image::Save(const std::string &a_path)
{
    auto extPos = a_path.find_last_of('.');
    if (a_path.substr(extPos, std::string::npos) == ".png" || 
        a_path.substr(extPos, std::string::npos) == ".PNG")
    {
        stbi_write_png(a_path.c_str(), width, height, channels, 
                       data, width * channels);
    }
    else if (a_path.substr(extPos, std::string::npos) == ".jpg"  || 
             a_path.substr(extPos, std::string::npos) == ".JPG"  ||
             a_path.substr(extPos, std::string::npos) == ".jpeg" || 
             a_path.substr(extPos, std::string::npos) == ".JPEG")
    {
        stbi_write_jpg(a_path.c_str(), width, height, channels, 
                       data, 100);
    }
    else
    {
        std::cerr << "Unknown file extension: " 
                  << a_path.substr(extPos, std::string::npos) 
                  << "in file name" << a_path << std::endl;
        return 1;
    }

    return 0;
}


void Image::ReadFile(const std::string &a_file)
{
    char   sym;
    Pixel  pixCol = {0, 0, 0, 255};
    bool   blockFlag = true;
    
    std::ifstream file(a_file);
    if (!(file.is_open()))
    {
        throw std::out_of_range("File error");
    }

    for (int y = height / tileSize - 1; y >= 0; --y)
    {
        for (int x = 0; x < width / tileSize; ++x)
        {
            file.get(sym);
            switch (sym)
            {
                case ' ': // empty space
                    pixCol = {0, 0, 0, 255};
                    PutPixels((x * tileSize), (y * tileSize), pixCol);
                    break;
                case '#': // block
                    //pixCol = {0, 0, 255, 255};
                    PutPixels((x * tileSize), (y * tileSize), 
                               Wall(),         blockFlag);
                    break;
                case '.': // floor
                    pixCol = backgroundColor;
                    PutPixels((x * tileSize), (y * tileSize), pixCol);
                    break;
                case '@': // player
                    xCoordPlayer = x * tileSize;
                    yCoordPlayer = y * tileSize;
                    break;
                case 'x': // room exit
                    PutPixels((x * tileSize), (y * tileSize), Door());
                    break;
                case 'Q': // maze exit
                    pixCol = {128, 0, 128, 255};
                    PutPixels((x * tileSize), (y * tileSize), pixCol);
                    break;
                case 'G': // teasure
                    PutPixels((x * tileSize), (y * tileSize), Teasure());
                    break;
                default:
                    break;
            }
        }
        file.get(sym); // '\n'
    }

    file.close();
}

Image::~Image()
{
    if(self_allocated)
    {
        delete [] data;
        delete [] block;
    }
    else
    {
        stbi_image_free(data);
    }
}
