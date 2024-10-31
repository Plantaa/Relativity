#include <stdlib.h>
#include <stdio.h>

#include "raylib.h"
#include "math.h"

#include "coordinate.h"
#include "utils.h"

void controlCamera(bool *isMoving, Vector2 *mouseDrag, Vector2 mousePosition, Camera2D *camera);
void drawEveryFrame(int currentScreenWidth, int currentScreenHeight, float angle, Coordinate *coordinates, int total);
void drawPrimarySystem(int currentScreenWidth, int currentScreenHeight, Color color);
void drawCoordinateSystem(float angle, int screenWidth, int screenHeight, Color color);
void drawAndSaveNewCoordinate(Coordinate *coordinates, Vector2 position, float angle, int *total, int *index);
void drawSavedCoordinates(Coordinate *coordinates, int total, float angle);
void setSecondarySystemAngle(Vector2 position, float *angle);
Vector2 compensateMousePositionForCamera(Camera2D camera, Vector2 mousePosition);
void drawAngleMarker(Vector2 origin, float angleDegrees);
bool selectCoordinate(Vector2 mousePosition, Coordinate *coordinates, int total);

int main(void)
{

    const int screenWidth = 900;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Relative Systems");
    SetTargetFPS(60);

    // Camera config
    Camera2D camera = {
        .offset = {
            .x = screenWidth / 2,
            .y = screenHeight / 2},
        .target = {.x = 0.0f, .y = 0.0f},
        .rotation = 0.0f,
        .zoom = 1.0f};

    // Camera state
    bool isMoving = false;
    Vector2 mouseDrag = {0};

    // Tracking of saved coordinates
    int index = 0;
    int total = 0;
    Coordinate coordinates[5];
    float angle = 45 * (PI / 180);

    while (!WindowShouldClose()) // When "ESC" -> Close
    {

        int currentScreenWidth = GetScreenWidth();
        int currentScreenHeight = GetScreenHeight();
        Vector2 mousePosition = GetMousePosition();

        controlCamera(&isMoving, &mouseDrag, mousePosition, &camera);

        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode2D(camera);

        Vector2 mousePositionCompensated = compensateMousePositionForCamera(camera, mousePosition);

        if (IsKeyPressed(KEY_R))
        {
            setSecondarySystemAngle(mousePositionCompensated, &angle);
        }

        drawEveryFrame(currentScreenWidth, currentScreenHeight, angle, coordinates, total);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            bool found = selectCoordinate(mousePositionCompensated, coordinates, total);
            if (!found)
            {
                drawAndSaveNewCoordinate(coordinates, mousePositionCompensated, angle, &total, &index);
            }
        }

        EndMode2D();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

void drawEveryFrame(int currentScreenWidth, int currentScreenHeight, float angle, Coordinate *coordinates, int total)
{
    Vector2 origin = {0};
    drawPrimarySystem(currentScreenWidth, currentScreenHeight, BLACK);
    drawCoordinateSystem(angle, currentScreenWidth, currentScreenHeight, RED);
    drawAngleMarker(origin, radiansToDegrees(angle));
    drawSavedCoordinates(coordinates, total, angle);
}

void drawPrimarySystem(int currentScreenWidth, int currentScreenHeight, Color color)
{
    Vector2 xAxisBegin = {
        -currentScreenWidth,
        0};

    Vector2 xAxisEnd = {
        currentScreenWidth,
        0};

    Vector2 yAxisBegin = {
        0,
        -currentScreenHeight};

    Vector2 yAxisEnd = {
        0,
        currentScreenHeight};

    DrawLineV(xAxisBegin, xAxisEnd, color);
    DrawTriangle(
        (Vector2){0, -currentScreenHeight / 2},
        (Vector2){-10, (-currentScreenHeight + 20) / 2},
        (Vector2){10, (-currentScreenHeight + 20) / 2},
        color);
    DrawLineV(yAxisBegin, yAxisEnd, color);
}

void drawAndSaveNewCoordinate(Coordinate *coordinates, Vector2 position, float angle, int *total, int *index)
{
    if ((*total)++ >= 5)
        (*total) = 5;
    if ((*index) >= 5)
        (*index) = 0;

    Coordinate *newCoordinate = coordinateCreate();
    coordinateFill(newCoordinate, position, angle, 6, BLUE);

    coordinates[(*index)++] = *newCoordinate;
    DrawCircleV(newCoordinate->primaryPosition, 6, BLUE);
    DrawText(newCoordinate->primaryLabel, newCoordinate->primaryPosition.x + 5, newCoordinate->primaryPosition.y + 5, 10, BLACK);
    DrawText(newCoordinate->secondaryLabel, newCoordinate->primaryPosition.x + 10, newCoordinate->primaryPosition.y + 10, 10, RED);
}

bool selectCoordinate(Vector2 mousePosition, Coordinate *coordinates, int total)
{
    bool found = false;
    for (int i = 0; i < total; i++)
    {
        if (CheckCollisionPointCircle(mousePosition, coordinates[i].primaryPostion, coordinates[i].radius))
        {
            found = true;
            coordinates[i].selected = true;
        }
        else
        {
            coordinates[i].selected = false;
        }
    }
    return found;
}

bool selectCoordinate(Vector2 mousePosition, Coordinate *coordinates, int total)
{
    bool found = false;
    for (int i = 0; i < total; i++)
    {
        if (CheckCollisionPointCircle(mousePosition, coordinates[i].primaryPostion, coordinates[i].radius))
        {
            found = true;
            coordinates[i].selected = true;
        }
        else
        {
            coordinates[i].selected = false;
        }
    }
    return found;
}

void drawSavedCoordinates(Coordinate *coordinates, int total, float angle)
{
    for (int i = 0; i < total; i++)
    {
        Color drawColor = coordinates[i].selected ? GREEN : coordinates[i].color;

        Vector2 primaryCoordinates = coordinates[i].primaryPosition;
        Vector2 secondaryCoordinates = coordinates[i].secondaryPostion;
        drawComponentsSystem(primaryCoordinates, 0, BLACK);
        DrawCircleV(primaryCoordinates, coordinates[i].radius, drawColor);
        DrawText(coordinates[i].primaryLabel, primaryCoordinates.x + 5, primaryCoordinates.y + 5, 10, BLACK);
        updateSecondaryLabel(coordinates + i, angle);
        DrawText(coordinates[i].secondaryLabel, primaryCoordinates.x + 5, primaryCoordinates.y + 17, 10, RED);
    }
}

void controlCamera(bool *isMoving, Vector2 *mouseDrag, Vector2 mousePosition, Camera2D *camera)
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

void setSecondarySystemAngle(Vector2 position, float *angle)
{
    *angle = calculateAngle((Vector2){.x = 0.0f, .y = 0.0f}, position);
}

Vector2 compensateMousePositionForCamera(Camera2D camera, Vector2 mousePosition)
{
    Vector2 cameraCompensation = {
        .x = camera.target.x - camera.offset.x,
        .y = camera.target.y - camera.offset.y};

    return (Vector2){
        .x = mousePosition.x + cameraCompensation.x,
        .y = mousePosition.y + cameraCompensation.y};
}

void drawCoordinateSystem(float angle, int screenWidth, int screenHeight, Color color)
{
    float angleSin = sin(angle);
    float angleCos = cos(angle);
    float axisLength = screenWidth * screenHeight;

    Vector2 xAxisBegin = {
        angleCos * axisLength,
        angleSin * axisLength};

    Vector2 xAxisEnd = {
        -angleCos * axisLength,
        -angleSin * axisLength};

    Vector2 yAxisBegin = {
        angleSin * axisLength,
        -angleCos * axisLength};

    Vector2 yAxisEnd = {
        -angleSin * axisLength,
        angleCos * axisLength};

    DrawLineV(xAxisBegin, xAxisEnd, color);
    DrawLineV(yAxisBegin, yAxisEnd, color);
}

void drawAngleMarker(Vector2 origin, float angleDegrees)
{
    char markerText[10];
    snprintf(markerText, 10, "%f", -angleDegrees);
    DrawCircleSectorLines(origin, 50, 0.f, angleDegrees, 30, GRAY);
    DrawText(markerText, 60, 20, 10, GRAY);
}
