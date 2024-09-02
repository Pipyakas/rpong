#include "raylib.h"

const int screenWidth = 800;
const int screenHeight = 450;

typedef struct Paddle {
    Rectangle rect;
    int speed;
} Paddle;

typedef struct Ball {
    Vector2 position;
    Vector2 speed;
    float radius;
} Ball;

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "Pong Game");

    Paddle leftPaddle = { { 50, screenHeight / 2 - 50, 10, 100 }, 500 };
    Paddle rightPaddle = { { screenWidth - 60, screenHeight / 2 - 50, 10, 100 }, 500 };
    Ball ball = { { screenWidth / 2, screenHeight / 2 }, { 300, 300 }, 10 };

    int leftScore = 0;
    int rightScore = 0;
    bool gameStarted = false;

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())
    {
        // Update
        //----------------------------------------------------------------------------------
        // Paddle movement
        if (IsKeyDown(KEY_W) && leftPaddle.rect.y > 0) {
            leftPaddle.rect.y -= leftPaddle.speed * GetFrameTime();
            gameStarted = true;
        }
        if (IsKeyDown(KEY_S) && leftPaddle.rect.y < screenHeight - leftPaddle.rect.height) {
            leftPaddle.rect.y += leftPaddle.speed * GetFrameTime();
            gameStarted = true;
        }
        if (IsKeyDown(KEY_UP) && rightPaddle.rect.y > 0) {
            rightPaddle.rect.y -= rightPaddle.speed * GetFrameTime();
            gameStarted = true;
        }
        if (IsKeyDown(KEY_DOWN) && rightPaddle.rect.y < screenHeight - rightPaddle.rect.height) {
            rightPaddle.rect.y += rightPaddle.speed * GetFrameTime();
            gameStarted = true;
        }

        // Ball movement
        if (gameStarted) {
            ball.position.x += ball.speed.x * GetFrameTime();
            ball.position.y += ball.speed.y * GetFrameTime();
        }

        // Ball collision with top and bottom walls
        if (ball.position.y <= ball.radius || ball.position.y >= screenHeight - ball.radius) {
            ball.speed.y *= -1;
        }

        // Ball collision with paddles
        if (CheckCollisionCircleRec(ball.position, ball.radius, leftPaddle.rect)) {
            ball.speed.x *= -1;
        }
        if (CheckCollisionCircleRec(ball.position, ball.radius, rightPaddle.rect)) {
            ball.speed.x *= -1;
        }

        // Ball out of bounds
        if (ball.position.x <= 0) {
            rightScore++;
            ball.position = (Vector2){ screenWidth / 2, screenHeight / 2 };
            ball.speed = (Vector2){ 300, 300 };
            gameStarted = false;
        }
        if (ball.position.x >= screenWidth) {
            leftScore++;
            ball.position = (Vector2){ screenWidth / 2, screenHeight / 2 };
            ball.speed = (Vector2){ -300, -300 };
            gameStarted = false;
        }

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawRectangleRec(leftPaddle.rect, BLACK);
        DrawRectangleRec(rightPaddle.rect, BLACK);
        DrawCircleV(ball.position, ball.radius, DARKGRAY);

        DrawText(TextFormat("%d", leftScore), screenWidth / 4, 20, 40, LIGHTGRAY);
        DrawText(TextFormat("%d", rightScore), 3 * screenWidth / 4, 20, 40, LIGHTGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}