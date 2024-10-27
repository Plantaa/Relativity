#include "raylib.h"
#include "math.h"

#include "coordinate.h"
#include "utils.h"

void drawEveryFrame(int currentScreenWidth, int currentScreenHeight, Coordinate *coordinates, int total);
void drawBaseSystem(int currentScreenWidth, int currentScreenHeight);
void drawSecondarySystem(int currentScreenWidth, int currentScreenHeight);
void drawAndSaveNewCoordinate(Coordinate *coordinates, Vector2 mousePosition, int total, int index);
void drawSavedCoordinates(Coordinate *coordinates, int total);

int main(void)
{

    const int screenWidth = 900;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Relative Systems");

    SetTargetFPS(60);

    // Tracking of saved coordinates
    int index = 0;
    int total = 0;
    Coordinate coordinates[5];

    while (!WindowShouldClose()) // When "ESC" -> Close
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        int currentScreenWidth = GetScreenWidth();
        int currentScreenHeight = GetScreenHeight();
        Vector2 mousePosition = GetMousePosition();

        drawEveryFrame(currentScreenWidth, currentScreenHeight, coordinates, total);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            drawAndSaveNewCoordinate(coordinates, mousePosition, total, index);
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}

void drawEveryFrame(int currentScreenWidth, int currentScreenHeight, Coordinate *coordinates, int total)
{
    drawBaseSystem(currentScreenWidth, currentScreenHeight);
    drawSecondarySystem(currentScreenWidth, currentScreenHeight);
    drawSavedCoordinates(coordinates, total);
}

void drawBaseSystem(int currentScreenWidth, int currentScreenHeight)
{
    Vector2 x1AxisBegin = {
        0,
        currentScreenHeight / 2};

    Vector2 x1AxisEnd = {
        currentScreenWidth,
        currentScreenHeight / 2};

    Vector2 y1AxisBegin = {
        currentScreenWidth / 2,
        0};

    Vector2 y1AxisEnd = {
        currentScreenWidth / 2,
        currentScreenHeight};

    DrawLineV(x1AxisBegin, x1AxisEnd, BLACK);
    DrawLineV(y1AxisBegin, y1AxisEnd, BLACK);
}

void drawSecondarySystem(int currentScreenWidth, int currentScreenHeight)
{
    Vector2 x2EndPosition = {currentScreenWidth, 0};
    Vector2 center = {currentScreenWidth / 2.0f, currentScreenHeight / 2.0f};

    float angle = calculateAngle(center, x2EndPosition);
    float axisLength = (currentScreenWidth < currentScreenHeight ? currentScreenWidth : currentScreenHeight);

    Vector2 x2AxisBegin = {
        center.x + cos(angle) * (axisLength * -1),
        center.y + sin(angle) * (axisLength * -1)};

    Vector2 x2AxisEnd = {
        center.x + cos(angle) * axisLength,
        center.y + sin(angle) * axisLength};

    Vector2 y2AxisBegin = {
        center.x - sin(angle) * (axisLength * -1),
        center.y + cos(angle) * (axisLength * -1)};

    Vector2 y2AxisEnd = {
        center.x - sin(angle) * axisLength,
        center.y + cos(angle) * axisLength};

    DrawLineV(x2AxisBegin, x2AxisEnd, RED);
    DrawLineV(y2AxisBegin, y2AxisEnd, RED);
}

void drawAndSaveNewCoordinate(Coordinate *coordinates, Vector2 mousePosition, int total, int index)
{
    if (total++ > 5)
        total = 5;
    if (index >= 5)
        index = 0;

    Coordinate newCoordinate = {
        mousePosition,
        6,
        BLUE};
    coordinates[index++] = newCoordinate;
    DrawCircleV(newCoordinate.vector, 6, BLUE);
}

void drawSavedCoordinates(Coordinate *coordinates, int total)
{
    for (int i = 0; i < total; i++)
    {
        DrawCircleV(coordinates[i].vector, coordinates[i].radius, coordinates[i].color);
    }
}