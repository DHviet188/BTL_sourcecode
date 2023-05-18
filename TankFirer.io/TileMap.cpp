/*#pragma once
#include <SDL.h>
#include "TileMap.hpp"
#include "RenderWindow.hpp"
#include<vector>

TileMap::TileMap(int width, int height) {
    this->width = width;
    this->height = height;
    this->tileSize = 90;
    this->_map = vector<vector<int>>(height, vector<int>(width, 0));
}

void TileMap::setTile(int x, int y, int type) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        _map[y][x] = type;
    }
}

void TileMap::render(RenderWindow& window) {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int type = 0;
            int type = _map[y][x];
            if (type > 0) {
                SDL_Texture* rockTexture;
                switch (type) {
                    case 1:
                        rockTexture = ;
                        break;
                    case 2:
                        rockTexture = ;
                        break;
                    case 3:
                        rockTexture = ;
                        break;
                }
                Entity rock(x * tileSize, y * tileSize, tileSize, tileSize, rockTexture);
                window.render(rock);
            }
        }
    }
}
*/
