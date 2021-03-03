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


Image::Image(int a_width, int a_height, int a_channels)
{
    data   = new Pixel [a_width * a_height];
    object = new char  [a_width * a_height];

    types = new char [20];
    files = new std::string [20];
    
    std::ifstream file("map.txt");
    if (!(file.is_open()))
    {
        throw std::out_of_range("File error");
    }
    
    for (int i = 0; i < 20; ++i)
    {
        file.get(types[i]);
    }
    
    char sym;
    file.get(sym); // '\n'
    
    for (int i = 0; i < 20; ++i)
    {
        getline(file, files[i]);
    }
    
    file.close();

    if (data != nullptr)
    {
        width          = a_width;
        height         = a_height;
        size           = a_width * a_height * a_channels;
        channels       = a_channels;
        self_allocated = true;
    }

    ReadFile(Room(), Type());
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


void Image::ReadFile(const std::string &a_file, const char a_type)
{
    Tiles wall;
    Tiles door;
    Tiles treasure;
            
    switch(a_type)
    {
        case 'A':
            wall.SetPic("resources/blue1.png", 3);
            door.SetPic("resources/door.png", 13);
            treasure.SetPic("resources/tr.png", 60);
            break;
        case 'B':
            wall.SetPic("resources/red1.png", 3);
            door.SetPic("resources/door.png", 13);
            treasure.SetPic("resources/tr.png", 60);
            break;
        case 'C':
            wall.SetPic("resources/blue1.png", 3);
            door.SetPic("resources/door.png", 13);
            treasure.SetPic("resources/tr.png", 60);
            break;
        case 'D':
            wall.SetPic("resources/blue1.png", 3);
            door.SetPic("resources/door.png", 13);
            treasure.SetPic("resources/tr.png", 60);
            break;
        default:
            break;
    }
    
    char   sym;
    Pixel  pixCol = {0, 0, 0, 255};
    
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
                    pixCol = {255, 0, 0, 255};
                    PutPixels((x * tileSize), 
                              (y * tileSize), 
                              pixCol, 
                              sym);
                    break;
                case '#': // block
                    PutPixels((x * tileSize), 
                              (y * tileSize), 
                              wall.Pic(),
                              sym);
                    break;
                case '.': // floor
                    pixCol = backgroundColor;
                    PutPixels((x * tileSize), 
                              (y * tileSize), 
                              pixCol,
                              sym);
                    break;
                case '@': // player
                    xCoordPlayer = x * tileSize;
                    yCoordPlayer = y * tileSize;
                    break;
                case 'x': // room exit
                    PutPixels((x * tileSize), 
                              (y * tileSize), 
                              door.Pic(),
                              sym);
                    break;
                case 'Q': // maze exit
                    pixCol = {128, 0, 128, 255};
                    PutPixels((x * tileSize), 
                              (y * tileSize), 
                              pixCol,
                              sym);
                    break;
                case 'G': // teasure
                    PutPixels((x * tileSize), 
                              (y * tileSize), 
                              treasure.Pic(),
                              sym);
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
        delete [] object;
        delete [] types;
        delete [] files;
    }
    else
    {
        stbi_image_free(data);
    }
}
