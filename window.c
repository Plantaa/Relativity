#include "raylib.h"
#include "math.h"

float CalculateAngle(Vector2 start, Vector2 end)
{
    float deltaX = end.x - start.x;
    float deltaY = end.y - start.y;
    return atan2f(deltaY, deltaX);
}

int main(void)
{
    
    const int screenWidth = 800;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60);
    

    while (!WindowShouldClose()) // When "ESC" -> Close
    {
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


        // Draw xy2 axis
        DrawLineV(center, xAxisEnd, RED);
        DrawLineV(center, yAxisEnd, RED);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}