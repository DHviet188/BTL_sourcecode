#pragma once
#include <SDL.h>
#include<SDL_image.h>

class Entity
{
public:
    Entity(float p_x, float p_y, float p_w, float p_h, SDL_Texture* p_tex);
    bool collidesWith(Entity& other);
    bool isAlive();
    void setAlive(bool alive);
    float getX();
    float getY();
    float getW();
    float getH();
    SDL_Texture* getTex();
    SDL_Rect getCurrentFrame();
    void setWH(float p_w, float p_h);
    void setXY(float x , float y);
    float getAngle();
    void setAngle(float _angle);
    float getScaleX();
    float getScaleY();
    void setScale(float scaleX, float scaleY);
    bool contains(int x, int y);
private:
    float x,y,w,h,_scaleX,_scaleY;
    float angle;
    bool alive;
    SDL_Rect currentFrame;
    SDL_Texture* tex;
};
