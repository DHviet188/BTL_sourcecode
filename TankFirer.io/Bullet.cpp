#pragma once
#include<SDL.h>
#include<SDL_image.h>
#include<iostream>
#include<math.h>
#include "Bullet.hpp"
#include "Entity.hpp"

using namespace std;

Bullet::Bullet() : m_x(0), m_y(0), m_speed(0), m_alive(false), m_w(0), m_h(0)
{}

Bullet::Bullet(float x, float y, int speed, float angle, bool alive, float width, float height)
    : m_x(x), m_y(y), m_speed(speed), m_alive(alive), m_w(width), m_h(height)
{}

void Bullet::setAngle(float angle)
{
    float rad_angle = angle * (M_PI / 180.0f);
    m_speedX = cos(rad_angle) * m_speed;
    m_speedY = sin(rad_angle) * m_speed;
}

void Bullet::setPosition(float x, float y)
{
    m_x = x;
    m_y = y;
}

void Bullet::setSpeed(float speed)
{
    m_speed = speed;
}

void Bullet::setAlive(bool alive)
{
    m_alive = alive;
}

bool Bullet::isAlive()
{
    return m_alive;
}

float Bullet::getX()
{
    return m_x;
}

float Bullet::getY()
{
    return m_y;
}

float Bullet::getW()
{
    return m_w;
}

float Bullet::getH()
{
    return m_h;
}

float Bullet::setW(float width)
{
    this -> m_w = width;
}

float Bullet::setH(float height)
{
    this -> m_h = height;
}

float Bullet::getAngle()
{
    float angle = atan2(m_speedY, m_speedX) * (180.0f / M_PI);
    return angle;
}

float Bullet::getSpeedX()
{
    return m_speedX;
}

float Bullet::getSpeedY()
{
    return m_speedY;
}

void Bullet::update(Entity &bulletEntity, float timeStep, int screenWidth, int screenHeight)
{
    if (m_alive)
    {
        m_x += m_speedX * timeStep;
        m_y += m_speedY * timeStep;

        bulletEntity.setXY(m_x, m_y);
        bulletEntity.setWH(m_w, m_h);

        if (m_x < 0 || m_x > screenWidth || m_y < 0 || m_y > screenHeight)
        {
            m_alive = false;
        }
    }
}

