#include<iostream>
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include<math.h>
#include<ctime>
#include<string>
#include<vector>
#include<utility>
#include<cstdlib>
#include<windows.h>

#include "RenderWindow.hpp"
#include "Entity.hpp"
#include "Utils.hpp"
#include "Bullet.hpp"
#include "PowerUp.hpp"

constexpr int SCREEN_WIDTH = 1080;
constexpr int SCREEN_HEIGHT = 720;
constexpr int rockWidth = 90;
constexpr int rockHeight = 90;

enum class GameState {
    MENU, PLAY, OPTION, BACK, PLAY_AGAIN
};

using namespace std;

Entity createRock(float x, float y, SDL_Texture* texture, int width, int height) {
    return Entity(x, y, width, height, texture);
}

bool checkBulletHitTank(Entity& bullet, Entity& tank) {
    return bullet.collidesWith(tank);
}

bool avoidTank(float x, float y,Entity& tank, float threshold)
{
    float distanceX = abs(x - tank.getX());
    float distanceY = abs(y - tank.getY());

    return (distanceX <= threshold && distanceY <= threshold);
}

bool avoidRock(float x, float y, vector<Entity>& rocks, float threshold)
{
    for (Entity& rock : rocks) {
        float distanceX = abs(x - rock.getX());
        float distanceY = abs(y - rock.getY());

        if (distanceX <= threshold && distanceY <= threshold) {
            return true;
        }
    }
    return false;
}

Entity RandRock(Entity& tank1, Entity& tank2, vector<Entity>& rocks, float threshold, SDL_Texture* rockTexture, int width, int height)
{
    float x, y;
    bool avoid;
    do {
        x = 50 + static_cast<float>(rand() % (900 - 50 - width));
        y = 50 + static_cast<float>(rand() % (600 - 50 - height));

        avoid = avoidTank(x, y, tank1, threshold) || avoidTank(x, y, tank2, threshold) || avoidRock(x, y, rocks, threshold);
    } while (avoid);

    if (x < 50 || x > 850 || y < 50 || y > 510) {
        return RandRock(tank1, tank2, rocks, threshold, rockTexture, width, height);
    }

    return createRock(x, y, rockTexture, width, height);
}

pair<float, float> Random_xy(Entity& tank1, Entity& tank2, vector<Entity>& rocks, float threshold) {
    float x, y;
    bool avoid;
    do {
        x = 50 + static_cast<float>(rand() % (850 - 50));
        y = 50 + static_cast<float>(rand() % (650 - 50));

        avoid = avoidTank(x, y, tank1, threshold) || avoidTank(x, y, tank2, threshold) || avoidRock(x, y, rocks, threshold);
    } while (avoid);

    return make_pair(x, y);
}

void updateHP(int life1, int life2, RenderWindow &window, Entity &FullBar1, Entity &OverHalf1, Entity &NearlyDie1, Entity &FullBar2, Entity &OverHalf2, Entity &NearlyDie2) {
    if (life1 == 3) window.render(FullBar1);
    else if (life1 == 2) window.render(OverHalf1);
    else if (life1 == 1) window.render(NearlyDie1);

    if (life2 == 3) window.render(FullBar2);
    else if (life2 == 2) window.render(OverHalf2);
    else if (life2 == 1) window.render(NearlyDie2);
}

void applyPowerUp(Bullet &bullet, Entity &_bullet) {
    bullet.setW(50);
    bullet.setH(37.5);
    _bullet.setWH(40,30);
    _bullet.setScale(2.5f,2.5f);
    bullet.setSpeed(1500);
}

void resetBullet(Bullet &bullet, Entity &_bullet) {
    bullet.setW(20);
    bullet.setH(15);
    _bullet.setWH(20,15);
    _bullet.setScale(1.0f,1.0f);
    bullet.setSpeed(700);
}

void resetGame(Entity& p1, Entity& p2, int& life1, int& life2) {
    p1.setXY(50, 50);
    p2.setXY(920, 600);
    life1 = 3;
    life2 = 3;
}

int main(int argc, char* args[])
{
    srand(time(0));
    if (SDL_Init(SDL_INIT_VIDEO) > 0)
    {
        cout << SDL_GetError() << endl;
    }
    if (!(IMG_Init(IMG_INIT_PNG)))
    {
        cout << SDL_GetError() << endl;
    }
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        cout << SDL_GetError() << endl;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        cout << Mix_GetError() << endl;
    }

    Mix_Chunk* tankFireSound = Mix_LoadWAV("gfx/tank_fire.wav");
    if (tankFireSound == nullptr) {
        cout << Mix_GetError() << endl;
    }

    Mix_Chunk* collideSound = Mix_LoadWAV("gfx/collide.wav");
    if (collideSound == nullptr) {
        cout << Mix_GetError() << endl;
    }

    Mix_Music* backgroundMusic = Mix_LoadMUS("gfx/background_music.mp3");
    if (backgroundMusic == nullptr) {
        cout << Mix_GetError() << endl;
    }
    Mix_Chunk* powerupSound = Mix_LoadWAV("gfx/PowerUp.wav");
    if (powerupSound == nullptr) {
        cout << Mix_GetError() << endl;
    }
    Mix_Chunk* tankmoveSound = Mix_LoadWAV("gfx/tank_move1.wav");
    if (tankmoveSound == nullptr) {
        cout << Mix_GetError() << endl;
    }


    Mix_PlayMusic(backgroundMusic, -1);

    RenderWindow window("Tank Battle.io", SCREEN_WIDTH, SCREEN_HEIGHT);
    int windowRefreshRate = 120;

    SDL_Texture* cubeTexture = window.loadTexture("gfx/menu1.png");
    SDL_Texture* p1Texture = window.loadTexture("gfx/tank_1.png");
    SDL_Texture* p2Texture = window.loadTexture("gfx/tank_2.png");
    SDL_Texture* bullet1Texture = window.loadTexture("gfx/bullet_1.png");
    SDL_Texture* bullet2Texture = window.loadTexture("gfx/bullet_2.png");
    SDL_Texture* rock1 = window.loadTexture("gfx/rock_1.png");
    SDL_Texture* rock2 = window.loadTexture("gfx/rock_2.png");
    SDL_Texture* rock3 = window.loadTexture("gfx/rock_3.png");
    SDL_Texture* PowerUpTexture = window.loadTexture("gfx/LuckyBox.png");
    SDL_Texture* FullBar1_img = window.loadTexture("gfx/FullBar1.png");
    SDL_Texture* OverHalf1_img = window.loadTexture("gfx/OverHalf1.png");
    SDL_Texture* NearlyDie1_img = window.loadTexture("gfx/NearlyDie1.png");
    SDL_Texture* FullBar2_img = window.loadTexture("gfx/FullBar2.png");
    SDL_Texture* OverHalf2_img = window.loadTexture("gfx/OverHalf2.png");
    SDL_Texture* NearlyDie2_img = window.loadTexture("gfx/NearlyDie2.png");
    SDL_Texture* Frame_Menu = window.loadTexture("gfx/Framemenu.png");
    SDL_Texture* Play_Button = window.loadTexture("gfx/play_button.png");
    SDL_Texture* Back_Button = window.loadTexture("gfx/back_button.png");
    SDL_Texture* Menu_img = window.loadTexture("gfx/menu.png");
    SDL_Texture* PlayAgain_img = window.loadTexture("gfx/play_again.png");
    SDL_Texture* Yes_img = window.loadTexture("gfx/Yes.png");
    SDL_Texture* No_img = window.loadTexture("gfx/No.png");
    SDL_Texture* mapTexture = window.loadTexture("gfx/desert.png");

    Entity cube(0,1080,5,5,cubeTexture);
    Entity mapgame(0,0,1080,720,mapTexture); mapgame.setAngle(0.0f);
    Entity p1(50, 50, 90, 80, p1Texture); p1.setAngle(0.0f);
    Entity p2(920, 600, 90, 80, p2Texture); p2.setAngle(0.0f);
    Entity bullet_1(p1.getX() + 90, p1.getY() + 40, 20, 15, bullet1Texture);
    Entity bullet_2(p2.getX(), p2.getY() + 40, 20, 15, bullet2Texture);
    Entity menu (0,0,1080,720,Menu_img); menu.setAngle(0.0f);
    Entity FrameMenu(300,250,480,420,Frame_Menu); FrameMenu.setAngle(0.0f);
    Entity PlayButton(390,350,320,70, Play_Button); PlayButton.setAngle(0.0f);
    Entity BackButton(390,500,320,70, Back_Button); BackButton.setAngle(0.0f);
    Entity FullBar1(0,0,190,40,FullBar1_img); FullBar1.setAngle(0.0f);
    Entity OverHalf1(0,0,190,40,OverHalf1_img); OverHalf1.setAngle(0.0f);
    Entity NearlyDie1(0,0,190,40,NearlyDie1_img); NearlyDie1.setAngle(0.0f);
    Entity FullBar2(890,680,190,40,FullBar2_img); FullBar2.setAngle(0.0f);
    Entity OverHalf2(890,680,190,40,OverHalf2_img); OverHalf2.setAngle(0.0f);
    Entity NearlyDie2(890,680,190,40,NearlyDie2_img); NearlyDie2.setAngle(0.0f);
    Entity play_again(0,0,1080,720,PlayAgain_img); play_again.setAngle(0.0f);
    Entity Yes(475,520,60,40,Yes_img); Yes.setAngle(0.0f);
    Entity No(560,520,50,50,No_img); No.setAngle(0.0f);
    float bulletSpeed = 700.0f;
    float angle1 = 0.0f;
    float angle2 = 180.0f;
    Bullet bullet1(p1.getX(), p1.getY(), bulletSpeed, angle1, false, 20, 15);
    Bullet bullet2(p2.getX(), p2.getY(), bulletSpeed, angle2, false, 20, 15);
    bullet1.setAngle(angle1);
    bullet2.setAngle(angle2);

    vector <PowerUp> powerups;
    vector <Entity> rocks;
    for (int i = 0; i < 6; i++) {
        SDL_Texture* rockTexture = (i % 3 == 0) ? rock1 : (i % 3 == 1) ? rock2 : rock3;
        int width = (i % 3 == 0) ? 115 : 130;
        int height = 90;

        Entity newRock = RandRock(p1, p2, rocks, 200, rockTexture, width, height);
        rocks.push_back(newRock);
    }

    SDL_Texture* p1_Win = window.loadTexture("gfx/player1Win.png");
    Entity p1Win(0, 0, 1080, 720, p1_Win); p1Win.setAngle(0.0f);
    if (p1_Win == nullptr) {
        cout << IMG_GetError() << endl;
    }

    SDL_Texture* p2_Win = window.loadTexture("gfx/player2Win.png");
    Entity p2Win(0, 0, 1080, 720, p2_Win); p2Win.setAngle(0.0f);
    if (p2_Win == nullptr) {
        cout << IMG_GetError() << endl;
    }

    ShowWindow(GetConsoleWindow(), SW_HIDE);
    bool gameRunning = true;
    bool player1Shoot = false;
    bool player2Shoot = false;
    bool rendermap = false;
    float startB1x = p1.getX() + 80, startB1y = p1.getY() + 32.5, startB2x = p2.getX(), startB2y = p2.getY() + 32.5;
    int life1 = 3;
    int life2 = 3;
    int reducedVolume = Mix_VolumeChunk(tankmoveSound, -1) / 2;

    SDL_Event event;

    GameState gameState = GameState ::MENU;

    const float timeStep = 1.0f/static_cast<float>(windowRefreshRate);
    float accumulator = 0.0f;
    float currentTime = utils::hireTimeInSeconds();
    float powerUpSpawnTimer = 0.0f;
    float powerUp_p1 = 0.0f;
    float powerUp_p2 = 0.0f;

    const Uint8 *key = SDL_GetKeyboardState(NULL);

    while(gameRunning)
    {
        int startTicks = SDL_GetTicks();
        float newTime = utils::hireTimeInSeconds();
        float frameTime = newTime - currentTime;
        currentTime = newTime;
        accumulator += frameTime;

        float prevX1 = p1.getX();
        float prevY1 = p1.getY();
        float prevX2 = p2.getX();
        float prevY2 = p2.getY();

        while (accumulator >= timeStep)
        {
            while(SDL_PollEvent(&event))
            {
                switch(event.type) {
                    case SDL_QUIT:
                        gameRunning = false;
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                        if (gameState == GameState::MENU)
                        {
                            int x,y;
                            SDL_GetMouseState(&x, &y);
                            if (PlayButton.contains(x, y)) {
                                gameState = GameState::PLAY;
                                rendermap = true;
                            }
                            else if (BackButton.contains(x, y))
                                gameRunning = false;
                        }
                        if (gameState == GameState::PLAY_AGAIN)
                        {
                            int x,y;
                            SDL_GetMouseState(&x,&y);
                            if (Yes.contains(x,y))
                            {
                                resetGame(p1,p2,life1,life2);
                                gameState = GameState::PLAY;
                            }
                            else if (No.contains(x,y))
                                gameRunning = false;
                        }
                      break;
                }
            }

            if (bullet1.isAlive()) {
                bullet1.update(bullet_1,timeStep, SCREEN_WIDTH, SCREEN_HEIGHT);
                bullet_1.setXY(bullet1.getX(), bullet1.getY());
                bullet_1.setAngle(bullet1.getAngle());
                for (Entity rock : rocks) {
                    if (bullet_1.collidesWith(rock)) {
                        Mix_PlayChannel(-1,collideSound,0);
                        bullet1.setAlive(false);
                        break;
                    }
                }
            }

            if (bullet2.isAlive()) {
                bullet2.update(bullet_2,timeStep, SCREEN_WIDTH, SCREEN_HEIGHT);
                bullet_2.setXY(bullet2.getX(), bullet2.getY());
                bullet_2.setAngle(bullet2.getAngle());
                for (Entity rock : rocks) {
                    if (bullet_2.collidesWith(rock)) {
                        Mix_PlayChannel(-1,collideSound,0);
                        bullet2.setAlive(false);
                        break;
                    }
                }
            }

            accumulator -= timeStep;
            if (key[SDL_SCANCODE_LEFT]) {
                if (p2.getX() > 0) {
//                    Mix_PlayChannel(-1,tankmoveSound,0);
                    p2.setXY(p2.getX() - 2, p2.getY());
                    for (Entity rock : rocks) {
                        if (p2.collidesWith(rock)) {
                            p2.setXY(prevX2, prevY2);
                            break;
                        }
                    }
                    p2.setAngle(0.0f);
                    if(!bullet2.isAlive()) bullet_2.setAngle(0.0f);
                    startB2x = p2.getX();
                    startB2y = p2.getY() + 32.5;
                    angle2 = 180.0f;
                }
            }
            else if (key[SDL_SCANCODE_RIGHT]) {
                if (p2.getX() < SCREEN_WIDTH - p2.getW()) {
//                    Mix_PlayChannel(-1,tankmoveSound,0);
                    p2.setXY(p2.getX() + 2, p2.getY());
                    for (Entity rock : rocks) {
                        if (p2.collidesWith(rock)) {
                            p2.setXY(prevX2, prevY2);
                            break;
                        }
                    }
                    p2.setAngle(180.0f);
                    if(!bullet2.isAlive()) bullet_2.setAngle(180.0f);
                    startB2x = p2.getX() + 80;
                    startB2y = p2.getY() + 32.5;
                    angle2 =0.0f;
                }
            }
            else if (key[SDL_SCANCODE_UP]) {
                if (p2.getY() > 0) {
//                    Mix_PlayChannel(-1,tankmoveSound,0);
                    p2.setXY(p2.getX(), p2.getY() - 2);
                    for (Entity rock : rocks) {
                        if (p2.collidesWith(rock)) {
                            p2.setXY(prevX2, prevY2);
                            break;
                        }
                    }
                    p2.setAngle(90.0f);
                    if(!bullet2.isAlive()) bullet_2.setAngle(90.0f);
                    startB2x = p2.getX() + 35;
                    startB2y = p2.getY();
                    angle2 = 270.0f;
                }
            }
            else if (key[SDL_SCANCODE_DOWN]) {
                if (p2.getY() < SCREEN_HEIGHT - p2.getH()) {
//                    Mix_PlayChannel(-1,tankmoveSound,0);
                    p2.setXY(p2.getX(), p2.getY() + 2);
                    for (Entity rock : rocks) {
                        if (p2.collidesWith(rock)) {
                            p2.setXY(prevX2, prevY2);
                            break;
                        }
                    }
                    p2.setAngle(270.0f);
                    if(!bullet2.isAlive()) bullet_2.setAngle(270.0f);
                    startB2x = p2.getX() + 35;
                    startB2y = p2.getY() + 75;
                    angle2 = 90.0f;
                }
            }
            if (key[SDL_SCANCODE_W]) {
                if (p1.getY() > 0) {
//                    Mix_PlayChannel(-1,tankmoveSound,0);
                    p1.setXY(p1.getX(), p1.getY() - 2);
                    for (Entity rock : rocks) {
                        if (p1.collidesWith(rock)) {
                            p1.setXY(prevX1, prevY1);
                            break;
                        }
                    }
                    p1.setAngle(270.0f);
                    if(!bullet1.isAlive()) bullet_1.setAngle(270.0f);
                    startB1x = p1.getX() + 35;
                    startB1y = p1.getY();
                    angle1 = 270.0f;
                }
            }
            else if(key[SDL_SCANCODE_A]) {
                if (p1.getX() > 0) {
//                    Mix_PlayChannel(-1,tankmoveSound,0);
                    p1.setXY(p1.getX() - 2, p1.getY());
                    for (Entity rock : rocks) {
                        if (p1.collidesWith(rock)) {
                            p1.setXY(prevX1, prevY1);
                            break;
                        }
                    }
                    p1.setAngle(180.0f);
                    if(!bullet1.isAlive()) bullet_1.setAngle(180.0f);
                    startB1x = p1.getX();
                    startB1y = p1.getY() + 32.5;
                    angle1 = 180.0f;
                }
            }
            else if(key[SDL_SCANCODE_D]) {
                if (p1.getX() < SCREEN_WIDTH - p1.getW()) {
//                    Mix_PlayChannel(-1,tankmoveSound,0);
                    p1.setXY(p1.getX() + 2, p1.getY());
                    for (Entity rock : rocks) {
                        if (p1.collidesWith(rock)) {
                            p1.setXY(prevX1, prevY1);
                            break;
                        }
                    }
                    p1.setAngle(0.0f);
                    if(!bullet1.isAlive()) bullet_1.setAngle(0.0f);
                    startB1x = p1.getX() + 80;
                    startB1y = p1.getY() + 32.5;
                    angle1 = 0.0f;
                }
            }
            else if(key[SDL_SCANCODE_S]) {
                if (p1.getY() < SCREEN_HEIGHT - p1.getH()) {
//                    Mix_PlayChannel(-1,tankmoveSound,0);
                    p1.setXY(p1.getX(), p1.getY() + 2);
                    for (Entity rock : rocks) {
                        if (p1.collidesWith(rock)) {
                            p1.setXY(prevX1, prevY1);
                            break;
                        }
                    }
                    p1.setAngle(90.0f);
                    if(!bullet1.isAlive()) bullet_1.setAngle(90.0f);
                    startB1x = p1.getX() + 35;
                    startB1y = p1.getY() + 75;
                    angle1 = 90.0f;
                }
            }
            if (key[SDL_SCANCODE_SPACE]) {
                if (!bullet1.isAlive()) {
                    bullet1.setAlive(true);
                    bullet1.setPosition(startB1x, startB1y);
                    bullet1.setAngle(angle1);
                    Mix_PlayChannel(-1, tankFireSound, 0);
                    break;
                }
            }
            if (key[SDL_SCANCODE_RETURN]) {
                if (!bullet2.isAlive()) {
                    bullet2.setAlive(true);
                    bullet2.setPosition(startB2x, startB2y);
                    bullet2.setAngle(angle2);
                    Mix_PlayChannel(-1, tankFireSound, 0);
                    break;
                }
            }
        }

        if (powerUpSpawnTimer >= 10.0f) {
            powerUpSpawnTimer = 0.0f;
            auto random = Random_xy(p1,p2,rocks,100);
            powerups.emplace_back(random.first, random.second, 50, 50, PowerUpTexture);
        }

        for (PowerUp &powerup : powerups) {
            if (powerup.isAlive()) {
                if (powerup.collidesWith(p2)) {
                    Mix_PlayChannel(-1, powerupSound,0);
                    powerup.setAlive(false);
                    applyPowerUp(bullet2, bullet_2);
                }
                if (powerup.collidesWith(p1)) {
                    Mix_PlayChannel(-1, powerupSound,0);
                    powerup.setAlive(false);
                    applyPowerUp(bullet1, bullet_1);
                }
            }
        }
        if (powerUp_p1 >= 5.0f) {
            powerUp_p1 = 0.0f;
            resetBullet(bullet1, bullet_1);
        }

        if (powerUp_p2 >= 5.0f) {
            powerUp_p2 = 0.0f;
            resetBullet(bullet2, bullet_2);
        }

        const float alpha = accumulator/timeStep;

        if (checkBulletHitTank(bullet_2, p1)) {
            life1--;
            bullet2.setAlive(false);
            bullet_2.setXY(-100, -100);
            Mix_PlayChannel(-1,collideSound,0);
        }
        if (checkBulletHitTank(bullet_1, p2)) {
            life2--;
            bullet1.setAlive(false);
            bullet_1.setXY(-100, -100);
            Mix_PlayChannel(-1,collideSound,0);
        }

        if (gameState == GameState::MENU)
        {
            window.clear();
            window.render(menu);
            window.render(FrameMenu);
            window.render(PlayButton);
            window.render(BackButton);
            window.display();
        }
        else if (gameState == GameState::PLAY_AGAIN)
        {
            window.clear();
            window.render(play_again);
            window.render(Yes);
            window.render(No);
            window.display();
        }

        else if (gameState == GameState::PLAY)
        {
            window.clear();

            window.render(mapgame);

            powerUpSpawnTimer += timeStep;
            powerUp_p1 += frameTime;
            powerUp_p2 += frameTime;

            for (Entity rock: rocks) {
                rock.setAngle(0.0f);
                window.render(rock);
            }

            if (bullet1.isAlive()) window.render(bullet_1);
            if (bullet2.isAlive()) window.render(bullet_2);

            for (PowerUp powerup: powerups) {
                if (powerup.isAlive())
                    window.render(powerup);
            }

            window.render(p1);
            window.render(p2);
            updateHP(life1, life2, window, FullBar1, OverHalf1, NearlyDie1, FullBar2, OverHalf2, NearlyDie2);

            window.display();

            int frameTicks = SDL_GetTicks() - startTicks;
            if (frameTicks < 1000 / windowRefreshRate)
                SDL_Delay(1000 / windowRefreshRate - frameTicks);

            bool p1Won = false, p2Won = false;
            if (life1 == 0) {
                p2Win.setXY(0,0);
                window.clear();
                window.render(p2Win);
                window.display();
                SDL_Delay(3000);
                gameState = GameState::PLAY_AGAIN;
            }
            if (life2 == 0) {
                p1Win.setXY(0,0);
                window.clear();
                window.render(p1Win);
                window.display();
                SDL_Delay(3000);
                gameState = GameState::PLAY_AGAIN;
            }
        }
    }

    Mix_FreeChunk(tankFireSound);
    Mix_FreeChunk(collideSound);
    Mix_FreeMusic(backgroundMusic);

    SDL_DestroyTexture(cubeTexture);
    SDL_DestroyTexture(mapTexture);
    SDL_DestroyTexture(Menu_img);
    SDL_DestroyTexture(p1Texture);
    SDL_DestroyTexture(p2Texture);
    SDL_DestroyTexture(bullet1Texture);
    SDL_DestroyTexture(bullet2Texture);
    SDL_DestroyTexture(rock1);
    SDL_DestroyTexture(rock2);
    SDL_DestroyTexture(rock3);
    SDL_DestroyTexture(p1_Win);
    SDL_DestroyTexture(p2_Win);
    SDL_DestroyTexture(FullBar1_img);
    SDL_DestroyTexture(OverHalf1_img);
    SDL_DestroyTexture(NearlyDie1_img);
    SDL_DestroyTexture(FullBar2_img);
    SDL_DestroyTexture(OverHalf2_img);
    SDL_DestroyTexture(NearlyDie2_img);
    SDL_DestroyTexture(PlayAgain_img);
    SDL_DestroyTexture(Yes_img);
    SDL_DestroyTexture(No_img);
    SDL_DestroyTexture(Play_Button);
    SDL_DestroyTexture(Back_Button);

    window.cleanUp();
    Mix_CloseAudio();
    SDL_Quit();
    return 0;
}
