#pragma once
#include "Entity.hpp"
#include <random>

class PowerUp : public Entity {
public:
    PowerUp(float x, float y, int width, int height, SDL_Texture* texture);
    bool isAlive();
    void setAlive(bool alive);
private:
    bool alive;
};

