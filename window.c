/*******************************************************************************************
*
*   raylib [core] example - Basic window
*
*   Welcome to raylib!
*
*   To test examples, just press F6 and execute raylib_compile_execute script
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   Example originally created with raylib 1.0, last time updated with raylib 1.0
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2013-2024 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "math.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------

float CalculateAngle(Vector2 start, Vector2 end)
{
    float deltaX = end.x - start.x;
    float deltaY = end.y - start.y;
    return atan2f(deltaY, deltaX);  // Função integrada para calcular ângulo
}

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(90);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------


    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        int currentScreenHeight = GetScreenHeight();
        int currentScreenWidth = GetScreenWidth();

        Vector2 mousePosition = GetMousePosition();
        Vector2 center = { currentScreenWidth / 2.0f, currentScreenHeight / 2.0f};

        float angle = CalculateAngle(center, mousePosition);
        float axisLength = (currentScreenWidth < currentScreenHeight ? currentScreenWidth : currentScreenHeight);

        Vector2 xAxisEnd = {
            center.x + cos(angle) * axisLength,
            center.y + sin(angle) * axisLength
        };

        Vector2 yAxisEnd = {
            center.x - sin(angle) * axisLength,
            center.y + cos(angle) * axisLength
        };

        ClearBackground(RAYWHITE);

        // Draw x1 axis
        DrawLine(0, currentScreenHeight/2, currentScreenWidth, currentScreenHeight/2, BLACK);
        // Draw y1 axis
        DrawLine(currentScreenWidth/2, 0, currentScreenWidth/2, currentScreenHeight, BLACK);


        // Draw x1 axis
        DrawLineV(center, xAxisEnd, RED);
        DrawLineV(center, yAxisEnd, RED);
        // DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}