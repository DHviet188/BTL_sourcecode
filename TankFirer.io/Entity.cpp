#include "Entity.hpp"
#include <SDL.h>
#include<SDL_image.h>
#include<math.h>

Entity::Entity(float p_x, float p_y, float p_w, float p_h, SDL_Texture* p_tex)
: x(p_x), y(p_y), w(p_w), h(p_h), tex(p_tex), alive(true)
{
    currentFrame.x = 0;
    currentFrame.y = 0;
    currentFrame.w = p_w;
    currentFrame.h = p_h;
    _scaleX = 1.0f;
    _scaleY = 1.0f;
}

bool Entity::isAlive() {
    return alive;
}

void Entity::setAlive(bool _alive) {
    alive = _alive;
}

bool Entity::collidesWith(Entity& other)
{
    float dx = x + w / 2 - (other.x + other.w / 2);
    float dy = y + h / 2 - (other.y + other.h / 2);
    float distance = sqrt(dx * dx + dy * dy);

    return distance < (w / 2 + other.w / 2);
}

float Entity::getX()
{
    return x;
}

float Entity::getY()
{
    return y;
}

float Entity::getW()
{
    return w;
}

float Entity::getH()
{
    return h;
}

SDL_Texture* Entity::getTex()
{
    return tex;
}

SDL_Rect Entity::getCurrentFrame()
{
    return currentFrame;
}

void Entity::setXY(float p_x,float p_y)
{
    this -> x = p_x;
    this -> y = p_y;
}

void Entity::setWH(float p_w, float p_h)
{
    this -> w = p_w;
    this -> h = p_h;
}

float Entity::getAngle()
{
    return angle;
}

void Entity::setAngle(float _angle)
{
    this -> angle = _angle;
}

float Entity::getScaleX() {
    return _scaleX;
}

float Entity::getScaleY()  {
    return _scaleY;
}

void Entity::setScale(float scaleX, float scaleY)
{
    _scaleX = scaleX;
    _scaleY = scaleY;
}

bool Entity:: contains(int x, int y) {
    return x >= getX() && x <= getX() + getW() && y >= getY() && y <= getY() + getH();
}
