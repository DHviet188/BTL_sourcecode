#pragma once
#include<SDL.h>
#include<SDL_image.h>
#include<iostream>
#include "Entity.hpp"

class Bullet {
public:
    Bullet();
    Bullet(float x, float y, int speed, float angle, bool alive, float width, float height);
    void setAngle(float angle);
    void setPosition(float x, float y);
    void setSpeed(float speed);
    void setAlive(bool alive);
    bool isAlive();
    float getX();
    float getY();
    float getW();
    float getH();
    float setW(float width);
    float setH(float height);
    float getAngle();
    float getSpeedX();
    float getSpeedY();
    void update(Entity &bulletEntity, float timeStep, int screenWidth, int screenHeight);

private:
    float m_x, m_y, m_speedX, m_speedY, m_w, m_h;
    float m_speed;
    bool m_alive;
};
