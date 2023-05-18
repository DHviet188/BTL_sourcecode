#include "PowerUp.hpp"

using namespace std;

PowerUp::PowerUp(float x, float y, int width, int height, SDL_Texture* texture)
    : Entity(x, y, width, height, texture), alive(true) {}

bool PowerUp::isAlive()
{
    return alive;
}

void PowerUp::setAlive(bool _alive)
{
    alive = _alive;
}
