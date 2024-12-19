#include <iostream>
#include <raylib.h>
#include <chrono>
#include <vector>


Color Green = Color{38, 185, 154, 255};
Color Dark_Green = Color{20, 160, 133, 255};
Color Light_Green = Color{129, 204, 184, 255};
Color Yellow = Color{243, 213, 91, 255};


class Ball
{
    public:
    float pos_x, pos_y;
    float radius;
    Color color;
    float vel_x, vel_y;

    Ball(float x, float y, float r, Color c, float vx, float vy)
    {
        pos_x = x;
        pos_y = y;
        radius = r;
        color = c;
        vel_x = vx;
        vel_y = vy;
    }
    void draw() { DrawCircle(pos_x, pos_y, radius, color); }

    void update(int* Player_score, int* CPU_score, float *player_paddle_vel, float *CPU_paddle_vel)
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
            int total_score = *Player_score + *CPU_score;
            vel_x *= -1;
            Reset(total_score, player_paddle_vel, CPU_paddle_vel);

        }
        if (pos_x - radius <= 0) // If the ball is out of bounds
        {
            *CPU_score += 1;
            int total_score = *Player_score + *CPU_score;
            vel_x *= -1;
            Reset(total_score, player_paddle_vel, CPU_paddle_vel);
        }
    }

    void Reset(int total_score, float *player, float *CPU)
    {
        pos_x = GetScreenWidth()/2;
        pos_y = GetScreenHeight()/2;

        int vel_choices[2] = {-1, 1};
        vel_x *= vel_choices[GetRandomValue(0, 1)];
        vel_y *= vel_choices[GetRandomValue(0, 1)];
        vel_x += total_score*0.02*vel_x/abs(vel_x);
        vel_y += total_score*0.02*vel_y/abs(vel_y);
        *player = 0.75*abs(vel_x)* (*player)/abs(*player);
        *CPU = 0.75*abs(vel_x) * (*CPU)/abs(*CPU);
    }
};

class Paddle // USER
{
    protected:
    void limit_movement_paddle()
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
    public:

    Color color;
    float vel_y;
    float pos_x, pos_y;
    float width, height;

    Paddle(float x, float y, float w, float h, Color c, float vy)
    {
        pos_x = x;
        pos_y = y;
        width = w;
        height = h;
        color = c;
        vel_y = vy;
    }
    void draw() { DrawRectangleRounded(Rectangle{pos_x, pos_y, width, height}, 0.8, 0, color); }

    void update()
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

};


class Paddle_CPU: public Paddle
{

public:
Paddle_CPU(float x, float y, float w, float h, Color c, float vy)
            : Paddle(x, y, w, h, c, vy) {} 

    void update( float ball_y)
    {
        if (pos_y+ height/2 < ball_y)
        {
            pos_y += vel_y;
        }

        else if (pos_y + height/2 > ball_y)
        {
            pos_y -= vel_y;
        }

        limit_movement_paddle();

    }

};

int main(void)
{
    std::cout<< "PONG!!";

    const int screen_width = 1280;
    const int screen_height = 800;
    // Initializing objects    
    Ball ball(screen_width/2, screen_height/2, 20, Yellow, -5, -5);
    Paddle paddle_player(10, (screen_height/2)-50, 20, 100, WHITE, 5);
    Paddle_CPU paddle_CPU(screen_width-20-10, (screen_height/2)-50, 20, 100, WHITE, 5);
    
    float y_player_prev = screen_height/2;
    float y_CPU_prev = (screen_height/2) - 50;

    // Scoring
    int Player_score = 0;
    int CPU_score = 0;


    InitWindow(screen_width, screen_height,"Pong"); //(width, height, title)
    /*
    GAME LOOP
1. Check for events example, quit, move
2. Update the positions based on the events
3. Draw based on the updated positions
    */ 
    SetTargetFPS(144); // sets the target frame rate. If not defined it runs as fast as possible




    while(!WindowShouldClose()) // WindowShouldClose() checks if ESC is pressed or the window is closed, retunrs true if it is and the game loop ends
    {
        BeginDrawing(); // creates a blank canvas/frame
        ClearBackground(Green); // Basically clears the frame for the next
        GetFrameTime();//
        DrawRectangle(0, 0, screen_width/2, screen_height, Dark_Green);

        // Debug vars
        DrawText(TextFormat("FPS: %f", 1/GetFrameTime()), 20, 20, 10, WHITE);
        DrawText(TextFormat("Vel: %f, %f", ball.vel_x, ball.vel_y), 20, 40, 10, WHITE);
        /*
        Raylib allows you to draw shapes, lines, text, images, etc.
        requires, position and SIZE and colour
        POSITION ==> TOP LEFT CORNER 0,0 and the it INCREASES as you go down and to the right
        */
        // Drawing static objects
        DrawLine (screen_width/2, 0, screen_width/2, screen_height, WHITE);


        // Update objects
        ball.update( &Player_score, &CPU_score, &paddle_player.vel_y, &paddle_CPU.vel_y);
        paddle_player.update();
        paddle_CPU.update(ball.pos_y);
        DrawText(TextFormat("%i", Player_score), screen_width/4-25, 20, 50, WHITE);
        DrawText(TextFormat("%i", CPU_score), 3*screen_width/4 - 25, 20, 50, WHITE);


        // Collision detection
        if(CheckCollisionCircleRec(Vector2{ball.pos_x, ball.pos_y}, ball.radius, Rectangle{paddle_player.pos_x, paddle_player.pos_y, paddle_player.width, paddle_player.height}))
        {
            //COllision detected
            ball.vel_x *= -1;
            // allowing the paddle movement to influence the ball
            float pos_y_diff = paddle_player.pos_y - y_player_prev;
            float vel_y = pos_y_diff/(GetFrameTime()*1000);
            if (vel_y != 0)
            {
                ball.vel_y += vel_y*0.1;
                // std::cout << "pos_y_diff: " << pos_y_diff << "vel_y: " << vel_y << "ball_vel_y: " << ball.vel_y << std::endl;
            }
            
        }
        if (CheckCollisionCircleRec(Vector2{ball.pos_x, ball.pos_y}, ball.radius, Rectangle{paddle_CPU.pos_x, paddle_CPU.pos_y, paddle_CPU.width, paddle_CPU.height}))
        {
            //COllision detected
            ball.vel_x *= -1;
            float pos_y_diff = paddle_CPU.pos_y - y_CPU_prev;
            float vel_y = pos_y_diff/(GetFrameTime()*1000);
            if (vel_y != 0)
            {
                ball.vel_y += vel_y*0.1;
                // std::cout << "pos_y_diff: " << pos_y_diff << "vel_y: " << vel_y << "ball_vel_y: " << ball.vel_y << std::endl;
            }
        }
        
        // Drawing the updated objects
        // Ball
        ball.draw();
        //Paddle
        paddle_player.draw();
        paddle_CPU.draw();
        y_CPU_prev = paddle_CPU.pos_y;
        y_player_prev = paddle_player.pos_y;
        

        EndDrawing(); // Ends the canvas drawing

    }

    CloseWindow(); // Closes the window duh
    return 0;
}
