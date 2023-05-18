#pragma once
#include <SDL.h>
#include "RenderWindow.hpp"
#include<vector>

class TileMap
{
public:
    TileMap(int width, int height);
    void setTile(int x, int y, int type);
    void render(RenderWindow& window);
private:
    int width,height, tileSize;
};
