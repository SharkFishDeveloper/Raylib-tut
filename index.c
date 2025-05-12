#include "raylib.h"

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - interactive window");

    Vector2 ballPosition = { (float)screenWidth/2, (float)screenHeight/2 };
    int ballRadius = 30;
    int speed = 5;

    SetTargetFPS(60); // Set frame rate

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Input
        if (IsKeyDown(KEY_RIGHT)) ballPosition.x += speed;
        if (IsKeyDown(KEY_LEFT)) ballPosition.x -= speed;
        if (IsKeyDown(KEY_UP)) ballPosition.y -= speed;
        if (IsKeyDown(KEY_DOWN)) ballPosition.y += speed;

        if (IsKeyPressed(KEY_F)) ToggleFullscreen();

        // Drawing
        BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawText("Move the circle with arrow keys!", 10, 10, 20, DARKGRAY);
            DrawText("Press F to toggle fullscreen", 10, 40, 20, GRAY);
            DrawCircleV(ballPosition, ballRadius, SKYBLUE);
            DrawFPS(screenWidth - 90, 10);
        EndDrawing();
    }

    CloseWindow();        // Close window and OpenGL context
    return 0;
}
