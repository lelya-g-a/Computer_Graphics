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
    
    int x = width / tileSize - 1;
    
    for (int y = height / tileSize - 1; y >= 0; --y)
    {
        PutPixels((x * tileSize), 
                  (y * tileSize), 
                  backgroundColor,
                  sym);
    }
    
    Tiles lifes;
    lifes.SetPic("resources/life.png", 1);
    
    for (int y = 1; y < 4; ++y)
    {
        PutPixels((x * tileSize), 
                  (2 * y * tileSize), 
                  lifes.Pic(),
                  ' ');
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
    Tiles empty;
    Tiles wall;
    Tiles door;
    Tiles treasure;
    Tiles trapP;
    Tiles trP;
    Tiles lightP;
    Tiles exit;
    
    door.SetPic("resources/door.png", 13);
    treasure.SetPic(treas[0], 1);
    trapP.SetPic(trap[0], 1);
    trP.SetPic(tr[0], 1);
    lightP.SetPic(light[0], 1);
    exit.SetPic("resources/exit.png", 2);
            
    switch(a_type)
    {
        case 'A':
            empty.SetPic("resources/ice_empty.png", 1);
            wall.SetPic("resources/ice.png", 1);
            break;
        case 'B':
            empty.SetPic("resources/fire_empty.png", 1);
            wall.SetPic("resources/fire.png", 1);
            break;
        case 'C':
            empty.SetPic("resources/stone_empty.png", 1);
            wall.SetPic("resources/stone.png", 1);
            break;
        case 'D':
            empty.SetPic("resources/grass_empty.png", 1);
            wall.SetPic("resources/grass.png", 1);
            break;
        default:
            break;
    }
    
    char   sym;
    Pixel  pixCol = {0, 0, 0, 255};
    
    treasLength = 0;
    treasCoord.clear();
    
    trapLength = 0;
    trapCoord.clear();
    
    trLength = 0;
    trCoord.clear();
    
    lightLength = 0;
    lightCoord.clear();
    
    std::ifstream file(a_file);
    if (!(file.is_open()))
    {
        throw std::out_of_range("File error");
    }

    for (int y = height / tileSize - 1; y >= 0; --y)
    {
        for (int x = 0; x < width / tileSize - 1; ++x)
        {
            file.get(sym);
            switch (sym)
            {
                case ' ': // empty space
                    PutPixels((x * tileSize), 
                              (y * tileSize), 
                              empty.Pic(), 
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
                    pixCol = backgroundColor;
                    PutPixels((x * tileSize), 
                              (y * tileSize), 
                              pixCol,
                              '.');
                    break;
                case 'x': // room exit
                    PutPixels((x * tileSize), 
                              (y * tileSize), 
                              wall.Pic(),
                              sym);
                    PutPixels((x * tileSize), 
                              (y * tileSize), 
                              door.Pic(),
                              sym);
                    break;
                case 'Q': // maze exit
                    PutPixels((x * tileSize), 
                              (y * tileSize), 
                              exit.Pic(),
                              sym);
                    break;
                case 'G': // teasure
                    treasCoord.push_back(x * tileSize);
                    treasCoord.push_back(y * tileSize);
                    treasLength += 1;
                    
                    PutPixels((x * tileSize), 
                              (y * tileSize), 
                              backgroundColor,
                              sym);
                    PutPixels((x * tileSize), 
                              (y * tileSize), 
                              treasure.Pic(),
                              sym);
                    break;
                case 'T': // trap
                    trapCoord.push_back(x * tileSize);
                    trapCoord.push_back(y * tileSize);
                    trapLength += 1;
                    
                    PutPixels((x * tileSize), 
                              (y * tileSize), 
                              backgroundColor,
                              sym);
                    PutPixels((x * tileSize), 
                              (y * tileSize), 
                              trapP.Pic(),
                              sym);
                    break;
                case 'E': // trap
                    trCoord.push_back(x * tileSize);
                    trCoord.push_back(y * tileSize);
                    trLength += 1;
                    
                    PutPixels((x * tileSize), 
                              (y * tileSize), 
                              backgroundColor,
                              sym);
                    PutPixels((x * tileSize), 
                              (y * tileSize), 
                              trP.Pic(),
                              sym);
                    break;
                case 'L': // light
                    lightCoord.push_back(x * tileSize);
                    lightCoord.push_back(y * tileSize);
                    lightLength += 1;
                    
                    PutPixels((x * tileSize), 
                              (y * tileSize), 
                              backgroundColor,
                              sym);
                    PutPixels((x * tileSize), 
                              (y * tileSize), 
                              lightP.Pic(),
                              sym);
                    break;
                default:
                    break;
            }
        }
        file.get(sym); // '\n'
    }
    
    firstFlag = 1;
    
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
