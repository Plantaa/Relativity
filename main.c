#include <stdlib.h>
#include <stdio.h>

#include "raylib.h"
#include "math.h"
#include "coordinate.h"
#include "utils.h"
#include "alphabet.h"

void controlCamera(bool *isMoving, Vector2 *mouseDrag, Vector2 mousePosition, Camera2D *camera);
void drawEveryFrame(int currentScreenWidth, int currentScreenHeight, float angle, Coordinate *coordinates, int total);
void drawPrimarySystem(int currentScreenWidth, int currentScreenHeight, Color color);
void drawCoordinateSystem(float angle, int screenWidth, int screenHeight, Color color);
void drawAndSaveNewCoordinate(Coordinate *coordinates, Vector2 position, float angle, int *total, int *index, int *alphabet_index);
void drawSavedCoordinates(Coordinate *coordinates, int total, float angle);
void setSecondarySystemAngle(Vector2 position, float *angle);
Vector2 compensateMousePositionForCamera(Camera2D camera, Vector2 mousePosition);
void drawAngleMarker(Vector2 origin, float angleDegrees);
Coordinate *selectCoordinate(Vector2 mousePosition, Coordinate *coordinates, int total);
void drawLegend(int currentScreenWidth, int currentScreenHeight, Coordinate *coordinate);
void clearCoordinates(Coordinate *coordinates, int total);

int main(void)
{

    const int screenWidth = 900;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Parábola dos Geômetras no Reino de Emma");
    SetTargetFPS(60);

    // Camera config
    Camera2D camera = {
        .offset = {
            .x = screenWidth / 2,
            .y = screenHeight / 2},
        .target = {.x = 0.0f, .y = 0.0f},
        .rotation = 0.0f,
        .zoom = 1.0f};

    bool isMoving = false;
    Vector2 mouseDrag = {0};

    // Tracking of saved coordinates
    int index = 0;
    int total = 0;
    int alphabet_index = 0;
    Coordinate coordinates[26];
    float angle = 45 * (PI / 180);
    Coordinate *coordinateSelected = NULL;

    while (!WindowShouldClose())
    {

        int currentScreenWidth = GetScreenWidth();
        int currentScreenHeight = GetScreenHeight();
        Vector2 mousePosition = GetMousePosition();
        Vector2 mousePositionCompensated = compensateMousePositionForCamera(camera, mousePosition);

        controlCamera(&isMoving, &mouseDrag, mousePosition, &camera);

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);

            BeginMode2D(camera);

            if (IsKeyPressed(KEY_R))
            {
                setSecondarySystemAngle(mousePositionCompensated, &angle);
            }

            drawEveryFrame(currentScreenWidth, currentScreenHeight, angle, coordinates, total);

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                coordinateSelected = selectCoordinate(mousePositionCompensated, coordinates, total);
                if (!coordinateSelected)
                {
                    drawAndSaveNewCoordinate(coordinates, mousePositionCompensated, angle, &total, &index, &alphabet_index);
                }
            }

            EndMode2D();

            if (coordinateSelected)
            {
                drawLegend(currentScreenWidth, currentScreenHeight, coordinateSelected);
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

void drawEveryFrame(int currentScreenWidth, int currentScreenHeight, float angle, Coordinate *coordinates, int total)
{
    Vector2 origin = {0};
    drawAngleMarker(origin, radiansToDegrees(angle));
    drawPrimarySystem(currentScreenWidth, currentScreenHeight, BLACK);
    drawCoordinateSystem(angle, currentScreenWidth, currentScreenHeight, RED);
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

void drawAndSaveNewCoordinate(Coordinate *coordinates, Vector2 position, float angle, int *total, int *index, int *alphabet_index)
{
    if ((*total)++ >= 26)
    {
        (*total) = 26;
        return;
    }

    if ((*index) >= 26)
        (*index) = 0;

    Coordinate *newCoordinate = coordinates+(*index)++;
    coordinateFill(newCoordinate, alphabet[*alphabet_index], position, angle, 6, BLUE);

    DrawCircleV(newCoordinate->primaryPosition, newCoordinate->radius, BLUE);
    DrawText(newCoordinate->primaryLabel, newCoordinate->primaryPosition.x + 5, newCoordinate->primaryPosition.y + 5, 10, BLACK);
    DrawText(newCoordinate->secondaryLabel, newCoordinate->primaryPosition.x + 5, newCoordinate->primaryPosition.y + 17, 10, RED);
    DrawText(newCoordinate->nameLabel, newCoordinate->primaryPosition.x - 13, newCoordinate->primaryPosition.y - 13, 10, DARKBLUE);

    (*alphabet_index) += 1;
}

Coordinate *selectCoordinate(Vector2 mousePosition, Coordinate *coordinates, int total)
{
    Coordinate *coordinate = NULL;
    for (int i = 0; i < total; i++)
    {
        if (CheckCollisionPointCircle(mousePosition, coordinates[i].primaryPosition, coordinates[i].radius))
        {
            coordinate = &coordinates[i];
            coordinates[i].selected = true;
        }
        else
        {
            coordinates[i].selected = false;
        }
    }
    return coordinate;
}

void drawLegend(int currentScreenWidth, int currentScreenHeight, Coordinate *coordinate)
{
    int boxWidth = 300;
    int boxHeight = 100;
    int padding = 10;

    Rectangle legendBox = {
        .x = currentScreenWidth - (boxWidth + padding),
        .y = currentScreenHeight - (boxHeight + padding),
        .width = boxWidth,
        .height = boxHeight};

    DrawRectangleRec(legendBox, Fade(GRAY, 0.7f));
    DrawRectangleLinesEx(legendBox, 2, RED);

    DrawText("Legend: ", legendBox.x + 10, legendBox.y + 10, 10, BLACK);
    DrawText(coordinate->primaryLabel, legendBox.x + 10, legendBox.y + 30, 5, BLACK);
    DrawText(coordinate->secondaryLabel, legendBox.x + 10, legendBox.y + 40, 5, RED);
}

void drawSavedCoordinates(Coordinate *coordinates, int total, float angle)
{
    for (int i = 0; i < total; i++)
    {
        if (coordinates[i].active == true)
        {
            Color drawColor = coordinates[i].selected ? GREEN : coordinates[i].color;
            Vector2 primaryCoordinates = coordinates[i].primaryPosition;
            Vector2 secondaryCoordinates = coordinates[i].secondaryPosition;
            drawPrimaryComponents(primaryCoordinates, BLACK);
            drawSecondaryComponents(primaryCoordinates, secondaryCoordinates, angle, RED);
            DrawCircleV(primaryCoordinates, coordinates[i].radius, drawColor);
            DrawText(coordinates[i].primaryLabel, primaryCoordinates.x + 5, primaryCoordinates.y + 5, 10, BLACK);
            updateSecondaryLabel(coordinates + i, angle);
            DrawText(coordinates[i].secondaryLabel, primaryCoordinates.x + 5, primaryCoordinates.y + 17, 10, RED);
            DrawText(coordinates[i].nameLabel, coordinates[i].primaryPosition.x - 13, coordinates[i].primaryPosition.y - 13, 10, DARKBLUE);
        }
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
