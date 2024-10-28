#include <stdlib.h>
#include <stdio.h>

#include "raylib.h"
#include "math.h"

#include "coordinate.h"
#include "utils.h"

void controlCamera(bool* isMoving, Vector2* mouseDrag, Vector2 mousePosition, Camera2D* camera);
void drawEveryFrame(int currentScreenWidth, int currentScreenHeight, Coordinate *coordinates, int total);
void drawPrimarySystem(int currentScreenWidth, int currentScreenHeight);
void drawSecondarySystem(int currentScreenWidth, int currentScreenHeight);
void drawAndSaveNewCoordinate(Coordinate *coordinates, Vector2 mousePosition, Camera2D camera, int* total, int* index);
void drawSavedCoordinates(Coordinate *coordinates, int total);

int main(void)
{

    const int screenWidth = 900;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Relative Systems");
    SetTargetFPS(60);

    // Camera config
    Camera2D camera = {
        .offset = {
            .x = screenWidth / 2.0f,
            .y = screenHeight / 2.0f
        },
        .target = {
            .x = screenWidth / 2.0f,
            .y = screenHeight / 2.0f
        },
        .rotation = 0.0f,
        .zoom = 1.0f
    };
    
    // Camera state
    bool isMoving = false;
    Vector2 mouseDrag = {0};

    // Tracking of saved coordinates
    int index = 0;
    int total = 0;
    Coordinate coordinates[5];

    while (!WindowShouldClose()) // When "ESC" -> Close
    {

        int currentScreenWidth = GetScreenWidth();
        int currentScreenHeight = GetScreenHeight();
        Vector2 mousePosition = GetMousePosition();

        controlCamera(&isMoving, &mouseDrag, mousePosition, &camera);

        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode2D(camera);

        drawEveryFrame(currentScreenWidth, currentScreenHeight, coordinates, total);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            drawAndSaveNewCoordinate(coordinates, mousePosition, camera, &total, &index);
        }

        EndMode2D();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

void drawEveryFrame(int currentScreenWidth, int currentScreenHeight, Coordinate *coordinates, int total)
{
    drawPrimarySystem(currentScreenWidth, currentScreenHeight);
    drawSecondarySystem(currentScreenWidth, currentScreenHeight);
    drawSavedCoordinates(coordinates, total);
}

void drawPrimarySystem(int currentScreenWidth, int currentScreenHeight)
{
    Vector2 xAxisBegin = {
        0,
        currentScreenHeight / 2};

    Vector2 xAxisEnd = {
        currentScreenWidth,
        currentScreenHeight / 2};

    Vector2 yAxisBegin = {
        currentScreenWidth / 2,
        0};

    Vector2 yAxisEnd = {
        currentScreenWidth / 2,
        currentScreenHeight};

    DrawLineV(xAxisBegin, xAxisEnd, BLACK);
    DrawLineV(yAxisBegin, yAxisEnd, BLACK);
}

void drawSecondarySystem(int currentScreenWidth, int currentScreenHeight)
{
    Vector2 xEndPosition = {currentScreenWidth, 0};
    Vector2 center = {currentScreenWidth / 2.0f, currentScreenHeight / 2.0f};

    float angle = calculateAngle(center, xEndPosition);
    float angleDegrees = radiansToDegrees(angle);
    float axisLength = (currentScreenWidth < currentScreenHeight ? currentScreenWidth : currentScreenHeight);

    Vector2 xAxisBegin = {
        center.x + cos(angle) * (axisLength * -1),
        center.y + sin(angle) * (axisLength * -1)};

    Vector2 xAxisEnd = {
        center.x + cos(angle) * axisLength,
        center.y + sin(angle) * axisLength};

    Vector2 yAxisBegin = {
        center.x - sin(angle) * (axisLength * -1),
        center.y + cos(angle) * (axisLength * -1)};

    Vector2 yAxisEnd = {
        center.x - sin(angle) * axisLength,
        center.y + cos(angle) * axisLength};

    DrawLineV(xAxisBegin, xAxisEnd, RED);
    DrawLineV(yAxisBegin, yAxisEnd, RED);

    // Angle marker
    char markerText[10];
    snprintf(markerText, 10, "%f", -angleDegrees);
    DrawCircleSectorLines(center, 50, 0.f, angleDegrees, 30, GRAY);
    DrawText(markerText, center.x + 60, center.y - 20, 10, GRAY);
}

void drawAndSaveNewCoordinate(Coordinate *coordinates, Vector2 mousePosition, Camera2D camera, int* total, int* index)
{
    if ((*total)++ >= 5)
        (*total) = 5;
    if ((*index) >= 5)
        (*index) = 0;
    
    // Compensate mouse position with camera position
    Vector2 coordinatePosition = {
        mousePosition.x + (camera.target.x - camera.offset.x),
        mousePosition.y + (camera.target.y - camera.offset.y)
    };

    Coordinate newCoordinate = {
        coordinatePosition,
        6,
        BLUE};
    coordinates[(*index)++] = newCoordinate;
    DrawCircleV(newCoordinate.vector, 6, BLUE);
}

void drawSavedCoordinates(Coordinate *coordinates, int total)
{
    for (int i = 0; i < total; i++)
    {
        DrawCircleV(coordinates[i].vector, coordinates[i].radius, coordinates[i].color);
    }
}

void controlCamera(bool* isMoving, Vector2* mouseDrag, Vector2 mousePosition, Camera2D* camera)
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
        {
            *isMoving = true;
            *mouseDrag = mousePosition;
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT))
        {
            *isMoving = false;
        }

        if (*isMoving)
        {
            Vector2 dragDelta = {mouseDrag->x - mousePosition.x, mouseDrag->y - mousePosition.y};

            camera->target.x += dragDelta.x;
            camera->target.y += dragDelta.y;

            *mouseDrag = mousePosition;
        }
}
