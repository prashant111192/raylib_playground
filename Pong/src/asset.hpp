
#include <iostream>
#include <raylib.h>
#include <chrono>
#include <vector>

class Ball
{
    public:
    float pos_x, pos_y;
    float radius;
    Color color;
    float vel_x, vel_y;

    Ball(float x, float y, float r, Color c, float vx, float vy);
    void draw();
    void update(int* Player_score, int* CPU_score);
    void Reset();
    void level_update(float *player, float *CPU);
};

class Paddle // USER
{
    protected:
    void limit_movement_paddle();
    public:

    Color color;
    float vel_y;
    float pos_x, pos_y;
    float width, height;

    Paddle(float x, float y, float w, float h, Color c, float vy);
    void draw();
    void update();
    void paddle_level_upgrade(int *paddle_height);
};


class Paddle_CPU: public Paddle
{

public:
Paddle_CPU(float x, float y, float w, float h, Color c, float vy)
            : Paddle(x, y, w, h, c, vy) {} 

    void update( float ball_y);
};