#include "raylib.h"
#include <math.h>

const int screenWidth = 800;
const int screenHeight = 450;
const float speedIncreaseFactor = 1.05f;
const int maxPaddleHeight = 100;
const int minPaddleHeight = 50;

typedef struct Paddle {
    Rectangle rect;
    int speed;
    int score;
} Paddle;

typedef struct Ball {
    Vector2 position;
    Vector2 speed;
    float radius;
} Ball;

typedef struct ScoreNotification {
    Vector2 position;
    int timer;
} ScoreNotification;

int main(void)
{
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight, "Pong Game");

    Paddle leftPaddle = { { 50, screenHeight / 2 - 50, 10, 100 }, 500, 0 };
    Paddle rightPaddle = { { screenWidth - 60, screenHeight / 2 - 50, 10, 100 }, 500, 0 };
    Ball ball = { { screenWidth / 2, screenHeight / 2 }, { 300, 300 }, 10 };

    int leftScore = 0;
    int rightScore = 0;
    bool gameStarted = false;
    ScoreNotification scoreNotification = { { screenWidth / 2, screenHeight / 2 }, 0 };

    SetTargetFPS(0);

    double lastUpdateTime = GetTime();
    double lastRenderTime = GetTime();
    double updateDeltaTime = 0.0;
    double renderDeltaTime = 0.0;
    int FPS = 0;

    while (!WindowShouldClose())
    {
        double currentTime = GetTime();
        updateDeltaTime = currentTime - lastUpdateTime;
        lastUpdateTime = currentTime;

        if (IsKeyDown(KEY_W) && leftPaddle.rect.y > 0) {
            leftPaddle.rect.y -= leftPaddle.speed * updateDeltaTime;
            gameStarted = true;
        }
        if (IsKeyDown(KEY_S) && leftPaddle.rect.y < screenHeight - leftPaddle.rect.height) {
            leftPaddle.rect.y += leftPaddle.speed * updateDeltaTime;
            gameStarted = true;
        }
        if (IsKeyDown(KEY_UP) && rightPaddle.rect.y > 0) {
            rightPaddle.rect.y -= rightPaddle.speed * updateDeltaTime;
            gameStarted = true;
        }
        if (IsKeyDown(KEY_DOWN) && rightPaddle.rect.y < screenHeight - rightPaddle.rect.height) {
            rightPaddle.rect.y += rightPaddle.speed * updateDeltaTime;
            gameStarted = true;
        }

        if (gameStarted) {
            ball.position.x += ball.speed.x * updateDeltaTime;
            ball.position.y += ball.speed.y * updateDeltaTime;
        }

        if (ball.position.y <= ball.radius) {
            ball.position.y = ball.radius;
            ball.speed.y *= -1;
            ball.speed.x *= speedIncreaseFactor;
            ball.speed.y *= speedIncreaseFactor;
        }
        if (ball.position.y >= screenHeight - ball.radius) {
            ball.position.y = screenHeight - ball.radius;
            ball.speed.y *= -1;
            ball.speed.x *= speedIncreaseFactor;
            ball.speed.y *= speedIncreaseFactor;
        }

        if (CheckCollisionCircleRec(ball.position, ball.radius, leftPaddle.rect)) {
            ball.position.x = leftPaddle.rect.x + leftPaddle.rect.width + ball.radius;
            ball.speed.x *= -1;
            ball.speed.x *= speedIncreaseFactor;
            ball.speed.y *= speedIncreaseFactor;
            if (IsKeyDown(KEY_W)) ball.speed.y -= 50;
            if (IsKeyDown(KEY_S)) ball.speed.y += 50;
        }
        if (CheckCollisionCircleRec(ball.position, ball.radius, rightPaddle.rect)) {
            ball.position.x = rightPaddle.rect.x - ball.radius;
            ball.speed.x *= -1;
            ball.speed.x *= speedIncreaseFactor;
            ball.speed.y *= speedIncreaseFactor;
            if (IsKeyDown(KEY_UP)) ball.speed.y -= 50;
            if (IsKeyDown(KEY_DOWN)) ball.speed.y += 50;
        }

        if (ball.position.x <= 0) {
            rightScore++;
            rightPaddle.score++;
            leftPaddle.score = 0;
            ball.position = (Vector2){ screenWidth / 2, screenHeight / 2 };
            ball.speed = (Vector2){ 300, 300 };
            gameStarted = false;
            scoreNotification.position = (Vector2){ 3 * screenWidth / 4 + 50, 20 };
            scoreNotification.timer = 30;
            if (rightPaddle.rect.height > minPaddleHeight) {
                rightPaddle.rect.height -= 10;
            }
            if (leftPaddle.rect.height < maxPaddleHeight && leftPaddle.score >= 2) {
                leftPaddle.rect.height += 10;
            }
        }
        if (ball.position.x >= screenWidth) {
            leftScore++;
            leftPaddle.score++;
            rightPaddle.score = 0;
            ball.position = (Vector2){ screenWidth / 2, screenHeight / 2 };
            ball.speed = (Vector2){ -300, -300 };
            gameStarted = false;
            scoreNotification.position = (Vector2){ screenWidth / 4 + 50, 20 };
            scoreNotification.timer = 30;
            if (leftPaddle.rect.height > minPaddleHeight) {
                leftPaddle.rect.height -= 10;
            }
            if (rightPaddle.rect.height < maxPaddleHeight && rightPaddle.score >= 2) {
                rightPaddle.rect.height += 10;
            }
        }

        currentTime = GetTime();
        renderDeltaTime = currentTime - lastRenderTime;
        lastRenderTime = currentTime;

        BeginDrawing();

        ClearBackground(BLACK);

        DrawRectangle(0, 0, screenWidth, screenHeight, WHITE);
        DrawRectangle(1, 1, screenWidth - 2, screenHeight - 2, BLACK);

        DrawRectangleRec(leftPaddle.rect, WHITE);
        DrawRectangleRec(rightPaddle.rect, WHITE);
        DrawCircleV(ball.position, ball.radius, DARKGRAY);

        DrawText(TextFormat("%d", leftScore), screenWidth / 4, 20, 40, LIGHTGRAY);
        DrawText(TextFormat("%d", rightScore), 3 * screenWidth / 4, 20, 40, LIGHTGRAY);

        if (scoreNotification.timer > 0) {
            float alpha = (float)scoreNotification.timer / 30;
            DrawText("+1", scoreNotification.position.x, scoreNotification.position.y, 40, Fade(LIGHTGRAY, alpha));
            scoreNotification.timer--;
        }

        FPS = GetFPS();
        DrawText(TextFormat("FPS: %d", FPS), 10, screenHeight - 20, 10, LIGHTGRAY);

        EndDrawing();

        SetWindowTitle(TextFormat("Pong Game -  FPS: %d", FPS));
    }

    CloseWindow();

    return 0;
}