    #include "asset.hpp"

    Ball::Ball(float x, float y, float r, Color c, float vx, float vy)
    {
        pos_x = x;
        pos_y = y;
        radius = r;
        color = c;
        vel_x = vx;
        vel_y = vy;
    }
    void Ball::draw() { DrawCircle(pos_x, pos_y, radius, color); }

    void Ball::update(int* Player_score, int* CPU_score)
    {
        pos_x += vel_x;
        pos_y += vel_y;

        if (pos_y + radius >= GetScreenHeight() || pos_y - radius <= 0) 
        {
            vel_y *= -1;
        }

        if (pos_x + radius >= GetScreenWidth())
        {
            *Player_score += 1;
            vel_x *= -1;
            Reset();

        }
        if (pos_x - radius <= 0) // If the ball is out of bounds
        {
            *CPU_score += 1;
            vel_x *= -1;
            Reset();
        }
    }

    void Ball::Reset()
    {
        pos_x = GetScreenWidth()/2;
        pos_y = GetScreenHeight()/2;

        int vel_choices[2] = {-1, 1};
        vel_x *= vel_choices[GetRandomValue(0, 1)];
        vel_y *= vel_choices[GetRandomValue(0, 1)];
    }

    void Ball::level_update(float *player, float *CPU)
    {
        vel_x *= 1.05;
        vel_y *= 1.05;
    }

    void Paddle::limit_movement_paddle()
    {
        if (pos_y + height > GetScreenHeight()) // Checks if the paddle is out of bounds
        {
            pos_y = GetScreenHeight() - height; // Lower bound, the position of the paddle is fixed and not v set to 0
        }
        if (pos_y < 0) // Upper bound
        {
            pos_y = 0;
        }
    }
    Paddle::Paddle(float x, float y, float w, float h, Color c, float vy)
    {
        pos_x = x;
        pos_y = y;
        width = w;
        height = h;
        color = c;
        vel_y = vy;
    }

    void Paddle::draw() { DrawRectangleRounded(Rectangle{pos_x, pos_y, width, height}, 0.8, 0, color); }

    void Paddle::update()
    {
        if (IsKeyDown(KEY_W)) // Using IsKeyDown to check if a partiucular input ios provided
        {
            pos_y -= vel_y; // this is for a frame, so the vel is multipled with the frame speeed to give position s=v*t
        }
        
        if (IsKeyDown(KEY_S))
        {
            pos_y += vel_y;
        }

        limit_movement_paddle();

    }
    
    void Paddle::paddle_level_upgrade(int *paddle_height)
    {
        height = *paddle_height;
    }

    void Paddle_CPU::update( float ball_y)
    {
        if (pos_y+ height/2 < ball_y)
        {
            pos_y += vel_y;
        }

        else if (pos_y + height/2 >= ball_y)
        {
            pos_y -= vel_y;
        }

        limit_movement_paddle();

    }