#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <vector>

constexpr int tileSize = 16;

struct Pixel
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

constexpr Pixel backgroundColor = {0, 0, 0, 255};

struct Image
{
    explicit Image(const std::string &a_path);
    Image(int a_width, int a_height, int a_channels);

    int    Save     (const std::string &a_path);

    int    Width    () const {return width;}
    int    Height   () const {return height;}
    int    Channels () const {return channels;}
    size_t Size     () const {return size;}
    Pixel* Data     ()       {return data;}
    int    XCoord   ()       {return xCoordPlayer;}
    int    YCoord   ()       {return yCoordPlayer;}
    //int    CurrRoom ()       {return currRoom;}
    char        Type ()       {return types[currRoom];}
    std::string Room ()       {return files[currRoom];}
    
    void NewRoom (int num) {currRoom += num;}


    Pixel GetPixel(int x, int y) 
    {
        return data[width * y + x];
    }

    
    void  PutPixel(int x, int y, const Pixel &pix) 
    {
        data[width * y + x] = pix;
    }


    void PutPixels(int x, int y, const Pixel &pix, char type)
    {
        for (int size_y = 0; size_y < tileSize; ++size_y)
        {
            for (int size_x = 0; size_x < tileSize; ++size_x)
            {
                data  [width * (y + size_y) + (x + size_x)] = pix;
                object[width * (y + size_y) + (x + size_x)] = type;
            }
        }
    }

    void PutPixels(int x, int y, Pixel* pix, char type)
    {
        for (int size_y = 0; size_y < tileSize; ++size_y)
        {
            for (int size_x = 0; size_x < tileSize; ++size_x)
            {
                data  [width * (y + size_y) + (x + size_x)] = 
                    pix[tileSize * size_y + size_x];
                object[width * (y + size_y) + (x + size_x)] = type;
            }
        }
    }
    
    
    // Is there a block on the top or bottom
    bool IsBlockY(int x, int y)
    {
        return ((object[width * y + x] == '#')                || 
                (object[width * y + x + tileSize - 1] == '#') ||
                (object[width * y + x] == 'x')                || 
                (object[width * y + x + tileSize - 1] == 'x') ||
                (object[width * y + x] == 'L')                || 
                (object[width * y + x + tileSize - 1] == 'L') ||
                (object[width * y + x] == 'Q')                || 
                (object[width * y + x + tileSize - 1] == 'Q') ||
                (object[width * y + x] == 'T')                || 
                (object[width * y + x + tileSize - 1] == 'T') ||
                (object[width * y + x] == 'E')                || 
                (object[width * y + x + tileSize - 1] == 'E') ||
                (object[width * y + x] == 'G')                || 
                (object[width * y + x + tileSize - 1] == 'G'));
    }

    // Is there a block on the right or left
    bool IsBlockX(int x, int y)
    {
        return ((object[width * y + x] == '#')                  || 
                (object[width * (y + tileSize - 1) + x] == '#') ||
                (object[width * y + x] == 'x')                  || 
                (object[width * (y + tileSize - 1) + x] == 'x') ||
                (object[width * y + x] == 'L')                  || 
                (object[width * (y + tileSize - 1) + x] == 'L') ||
                (object[width * y + x] == 'Q')                  || 
                (object[width * (y + tileSize - 1) + x] == 'Q') ||
                (object[width * y + x] == 'T')                  || 
                (object[width * (y + tileSize - 1) + x] == 'T') ||
                (object[width * y + x] == 'E')                  || 
                (object[width * (y + tileSize - 1) + x] == 'E') ||
                (object[width * y + x] == 'G')                  || 
                (object[width * (y + tileSize - 1) + x] == 'G'));
    }
    

    // Is there a treasure on the top or bottom
    bool IsTreasureY(int x, int y)
    {
        return ((object[width * y + x] == 'G') && 
                (object[width * y + x + tileSize - 1] == 'G'));
    }

    // Is there a treasure on the right or left
    bool IsTreasureX(int x, int y)
    {
        return ((object[width * y + x] == 'G') && 
                (object[width * (y + tileSize - 1) + x] == 'G'));
    }
    
    // Is there a trap on the top or bottom
    bool IsTrapY(int x, int y)
    {
        return ((object[width * y + x] == 'T') && 
                (object[width * y + x + tileSize - 1] == 'T'));
    }

    // Is there a trap on the right or left
    bool IsTrapX(int x, int y)
    {
        return ((object[width * y + x] == 'T') && 
                (object[width * (y + tileSize - 1) + x] == 'T'));
    }
    
    // Is there an empty treasure on the top or bottom
    bool IsTrY(int x, int y)
    {
        return ((object[width * y + x] == 'E') && 
                (object[width * y + x + tileSize - 1] == 'E'));
    }

    // Is there an empty treasure on the right or left
    bool IsTrX(int x, int y)
    {
        return ((object[width * y + x] == 'E') && 
                (object[width * (y + tileSize - 1) + x] == 'E'));
    }
    
    // Is there a door on the top or bottom
    bool IsDoorY(int x, int y)
    {
        return ((object[width * y + x] == 'x') && 
                (object[width * y + x + tileSize - 1] == 'x'));
    }
    
    // Is there a door on the right or left
    bool IsDoorX(int x, int y)
    {
        return ((object[width * y + x] == 'x') && 
                (object[width * (y + tileSize - 1) + x] == 'x'));
    }
    
    // Is there an epty space on the top or bottom
    bool IsEmptyY(int x, int y)
    {
        return ((object[width * y + x] == ' ') || 
                (object[width * y + x + tileSize - 1] == ' '));
    }
    
    // Is there an empty space on the right or left
    bool IsEmptyX(int x, int y)
    {
        return ((object[width * y + x] == ' ') || 
                (object[width * (y + tileSize - 1) + x] == ' '));
    }
    
    // Is there an exit near the player
    bool IsExit(int x, int y)
    {
        return (((object[width * (y - 1) + x] == 'Q') && 
                 (object[width * (y - 1) + x + tileSize - 1] == 'Q')) ||
                ((object[width * (y + tileSize) + x] == 'Q') && 
                 (object[width * (y + tileSize) + x + 
                                               tileSize - 1] == 'Q')) ||
                ((object[width * y + (x - 1)] == 'Q') && 
                 (object[width * (y + tileSize - 1) + 
                                                    (x - 1)] == 'Q')) ||
                ((object[width * y + (x + tileSize)] == 'Q') && 
                 (object[width * (y + tileSize - 1) + 
                                             (x + tileSize)] == 'Q')));
    }
    
    void ReadFile (const std::string &a_file, const char a_type);

    
    int TreasCoordX   (int num) {return treasCoord[num * 2];}
    int TreasCoordY   (int num) {return treasCoord[num * 2 + 1];}
    std::string Treas (int num) {return treas[num];}
    int TreasLength   ()        {return treasLength;}
    void TreasNext    ()        {treasCount = (treasCount + 1) % 20;}
    int TreasCount    ()        {return treasCount;}
    
    bool IsTreasure(int x, int y)
    {
        return (object[width * y + x] == 'G');
    }
    
    
    int TrapCoordX   (int num) {return trapCoord[num * 2];}
    int TrapCoordY   (int num) {return trapCoord[num * 2 + 1];}
    std::string Trap (int num) {return trap[num];}
    int TrapLength   ()        {return trapLength;}
    void TrapNext    ()        {trapCount = (trapCount + 1) % 20;}
    int TrapCount    ()        {return trapCount;}
    
    bool IsTrap(int x, int y)
    {
        return (object[width * y + x] == 'T');
    }
    
    
    int TrCoordX   (int num) {return trCoord[num * 2];}
    int TrCoordY   (int num) {return trCoord[num * 2 + 1];}
    std::string Tr (int num) {return tr[num];}
    int TrLength   ()        {return trLength;}
    void TrNext    ()        {trCount = (trCount + 1) % 20;}
    int TrCount    ()        {return trCount;}
    
    bool IsTr(int x, int y)
    {
        return (object[width * y + x] == 'E');
    }
    
    
    int LightCoordX   (int num) {return lightCoord[num * 2];}
    int LightCoordY   (int num) {return lightCoord[num * 2 + 1];}
    std::string Light (int num) {return light[num];}
    int LightLength   ()        {return lightLength;}
    void LightNext    ()        {lightCount = (lightCount + 1) % 40;}
    int LightCount    ()        {return lightCount;}
    
    
    bool IsFirst()
    {
        bool tmp = firstFlag;
        firstFlag = 0;
        return tmp;
    }

    ~Image();

private:
    int    width          = -1;
    int    height         = -1;
    int    channels       = 3;
    size_t size           = 0;
    Pixel* data           = nullptr;
    char*  object         = nullptr;
    bool   self_allocated = false;
    int    xCoordPlayer   = -1;
    int    yCoordPlayer   = -1;
    
    // For Game Map
    char*        types    = nullptr;
    std::string* files    = nullptr;
    int          currRoom = 0;
    
    std::vector <int> treasCoord;
    std::string treas [2] = {"resources/treas0.png",
                             "resources/treas1.png"};
    int treasCount  = 0;
    int treasLength = 0;
    
    std::vector <int> trapCoord;
    std::string trap [2] = {"resources/trap0.png",
                            "resources/trap1.png"};
    int trapCount  = 0;
    int trapLength = 0;
    
    std::vector <int> trCoord;
    std::string tr [2] = {"resources/tr0.png",
                          "resources/tr1.png"};
    int trCount  = 0;
    int trLength = 0;
    
    std::vector <int> lightCoord;
    std::string light [4] = {"resources/light0.png",
                             "resources/light1.png",
                             "resources/light2.png",
                             "resources/light3.png"};
    int lightCount  = 0;
    int lightLength = 0;
    
    bool firstFlag = 0;
};
