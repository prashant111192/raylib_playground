#include <iostream>
#include <raylib.h>


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

    void update(int* Player_score, int* CPU_score)
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
            Reset( total_score);

        }
        if (pos_x - radius <= 0) // If the ball is out of bounds
        {
            *CPU_score += 1;
            int total_score = *Player_score + *CPU_score;
            vel_x *= -1;
            Reset( total_score);
        }
    }

    void Reset(int total_score)
    {
        pos_x = GetScreenWidth()/2;
        pos_y = GetScreenHeight()/2;

        int vel_choices[2] = {-1, 1};
        vel_x *= vel_choices[GetRandomValue(0, 1)] + total_score*0.1;
        vel_y *= vel_choices[GetRandomValue(0, 1)] + total_score*0.1;
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
    SetTargetFPS(60); // sets the target frame rate. If not defined it runs as fast as possible

    while(!WindowShouldClose()) // WindowShouldClose() checks if ESC is pressed or the window is closed, retunrs true if it is and the game loop ends
    {
        BeginDrawing(); // creates a blank canvas/frame
        ClearBackground(Green); // Basically clears the frame for the next
        DrawRectangle(0, 0, screen_width/2, screen_height, Dark_Green);
        /*
        Raylib allows you to draw shapes, lines, text, images, etc.
        requires, position and SIZE and colour
        POSITION ==> TOP LEFT CORNER 0,0 and the it INCREASES as you go down and to the right
        */
        // Drawing static objects
        DrawLine (screen_width/2, 0, screen_width/2, screen_height, WHITE);


        // Update objects
        ball.update( &Player_score, &CPU_score);
        paddle_player.update();
        paddle_CPU.update(ball.pos_y);
        DrawText(TextFormat("%i", Player_score), screen_width/4-25, 20, 50, WHITE);
        DrawText(TextFormat("%i", CPU_score), 3*screen_width/4 - 25, 20, 50, WHITE);


        // Collision detection
        if(CheckCollisionCircleRec(Vector2{ball.pos_x, ball.pos_y}, ball.radius, Rectangle{paddle_player.pos_x, paddle_player.pos_y, paddle_player.width, paddle_player.height}))
        {
            //COllision detected
            ball.vel_x *= -1;
        }
        if (CheckCollisionCircleRec(Vector2{ball.pos_x, ball.pos_y}, ball.radius, Rectangle{paddle_CPU.pos_x, paddle_CPU.pos_y, paddle_CPU.width, paddle_CPU.height}))
        {
            //COllision detected
            ball.vel_x *= -1;
        }
        
        // Drawing the updated objects
        // Ball
        ball.draw();
        //Paddle
        paddle_player.draw();
        paddle_CPU.draw();

        EndDrawing(); // Ends the canvas drawing

    }

    CloseWindow(); // Closes the window duh
    return 0;
}
