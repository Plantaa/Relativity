#include <stdlib.h>
#include <stdio.h>

#include "raylib.h"
#include "math.h"
#include "coordinate.h"
#include "clear_button.h"
#include "utils.h"
#include "aside.h"

void controlCamera(bool *isMoving, Vector2 *mouseDrag, Vector2 mousePosition, Camera2D *camera);
void drawEveryFrame(int screenWidth, int ScreenHeight, float angle, Coordinate *coordinates, int total, Camera2D camera);
void drawPrimarySystem(int screenWidth, int ScreenHeight, Color color, Camera2D camera);
void drawCoordinateSystem(float angle, int screenWidth, int screenHeight, Color color);
void drawAndSaveNewCoordinate(Coordinate *coordinates, Vector2 position, float angle, int total);
void drawSavedCoordinates(Coordinate *coordinates, int total, float angle);
void setSecondarySystemAngle(Vector2 position, float *angle);
Vector2 compensateMousePositionForCamera(Camera2D camera, Vector2 mousePosition);
void drawAngleMarker(Vector2 origin, float angleDegrees);
Coordinate *selectCoordinate(Vector2 mousePosition, Coordinate *coordinates, int total);
void drawLegend(int screenWidth, int ScreenHeight, Coordinate *coordinate);
void clearCoordinates(Coordinate *coordinates, int total);
bool savedCoordinatesCheckCollision(Coordinate *coordinates, Vector2 mousePosition, int total);
void clearSelection(Coordinate *coordinates, int total);
void drawPlaceholderCoordinate(Coordinate *coordinate, Vector2 position, float angle);
void initializeCoordinates(Coordinate *coordinates, int total);
int findAvailableIndex(Coordinate *coordinates, int total);
void updateCameraOffset(Camera2D *camera, int screenWidth, int screenHeight);
void drawAxiiOrientationArrows(Camera2D camera, int screenWidth, int screenHeight, Color color);
void recenterCamera(Camera2D *camera);

int main(void)
{
    const int screenWidth = 900;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Parábola dos Geômetras no Reino de Emma");
    SetTargetFPS(60);

    Font font = LoadFontEx("./assets/fonts/Poppins-Regular.ttf", 32, NULL, 250);
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);

    Camera2D camera = {
        .offset = {
            .x = screenWidth / 2,
            .y = screenHeight / 2},
        .target = {.x = 0.0f, .y = 0.0f},
        .rotation = 0.0f,
        .zoom = 1.0f};

    bool isMoving = false;
    Vector2 mouseDrag = {0};

    int total = 26;
    Coordinate coordinates[total];
    initializeCoordinates(coordinates, total);
    Coordinate *coordinateSelected = NULL;
    Coordinate placeholderCoordinate;

    Aside aside = {
        .active = false,
        .box = {
            .x = screenWidth - 500,
            .y = 0,
            .height = screenHeight / 2,
            .width = 500}};

    ClearButton clearButton = {
        .box = {
            .x = 10,
            .y = screenHeight - 40,
            .width = 100,
            .height = 30},
        .xTextPadding = 30,
        .yTextPadding = 7,
        .fontSize = 15};

    float angle = -(1.0f / 4.0f) * PI;

    while (!WindowShouldClose())
    {
        int currentScreenWidth = GetScreenWidth();
        int currentScreenHeight = GetScreenHeight();
        Vector2 mousePosition = GetMousePosition();
        Vector2 mousePositionCompensated = compensateMousePositionForCamera(camera, mousePosition);

        updateCameraOffset(&camera, currentScreenWidth, currentScreenHeight);
        controlCamera(&isMoving, &mouseDrag, mousePosition, &camera);
        clearButtonPositionUpdate(&clearButton, currentScreenHeight);
        asidePositionUpdate(&aside, currentScreenWidth);

        bool isCoordinateSelected = (coordinateSelected && coordinateSelected->selected);
        bool isMouseOnClearButton = CheckCollisionPointRec(mousePosition, clearButton.box);
        bool isMouseOnSavedCoordinate = savedCoordinatesCheckCollision(coordinates, mousePositionCompensated, total);

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);

            clearButtonDraw(clearButton);

            if (isCoordinateSelected)
            {
                drawLegend(currentScreenWidth, currentScreenHeight, coordinateSelected);
                if (IsKeyPressed(KEY_DELETE)) coordinateSelected->active = false;
            }

            BeginMode2D(camera);
            {
                drawEveryFrame(currentScreenWidth, currentScreenHeight, angle, coordinates, total, camera);

                if (IsKeyDown(KEY_R))
                    setSecondarySystemAngle(mousePositionCompensated, &angle);

                if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && !(isMouseOnClearButton || isMouseOnSavedCoordinate))
                    drawPlaceholderCoordinate(&placeholderCoordinate, mousePositionCompensated, angle);

                else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
                {
                    if (isMouseOnClearButton) clearCoordinates(coordinates, total);

                    else if (isMouseOnSavedCoordinate)
                        coordinateSelected = selectCoordinate(mousePositionCompensated, coordinates, total);

                    else drawAndSaveNewCoordinate(coordinates, mousePositionCompensated, angle, total);
                }
            }
            EndMode2D();

            if (IsKeyPressed(KEY_F1)) aside.active = !aside.active;
            if (aside.active) asideDraw(aside, font);
        }
        EndDrawing();
    }
    UnloadFont(font);
    CloseWindow();
    return 0;
}

void drawEveryFrame(int screenWidth, int screenHeight, float angle, Coordinate *coordinates, int total, Camera2D camera)
{
    Vector2 origin = {0};
    drawAngleMarker(origin, radiansToDegrees(angle));
    drawPrimarySystem(screenWidth, screenHeight, BLACK, camera);
    drawCoordinateSystem(angle, screenWidth, screenHeight, RED);
    drawSavedCoordinates(coordinates, total, angle);
}

void drawPrimarySystem(int screenWidth, int screenHeight, Color color, Camera2D camera)
{
    Vector2 xAxisBegin = { camera.target.x - screenWidth, 0 };
    Vector2 xAxisEnd = { camera.target.x + screenWidth, 0 };

    Vector2 yAxisBegin = { 0, camera.target.y - screenHeight };
    Vector2 yAxisEnd = { 0, camera.target.y + screenHeight };

    DrawLineV(xAxisBegin, xAxisEnd, color);
    DrawLineV(yAxisBegin, yAxisEnd, color);

    drawAxiiOrientationArrows(camera, screenWidth, screenHeight, color);
}

void drawAndSaveNewCoordinate(Coordinate *coordinates, Vector2 position, float angle, int total)
{
    int index = findAvailableIndex(coordinates, total);
    if (index < 0) return;

    Coordinate *newCoordinate = coordinates + index;
    coordinateFill(newCoordinate, 'A' + index, position, angle, 6, BLUE);

    drawCoordinate(*newCoordinate, angle);
}

Coordinate *selectCoordinate(Vector2 mousePosition, Coordinate *coordinates, int total)
{
    Coordinate *coordinate = NULL;
    for (int i = 0; i < total; i++)
    {
        if (CheckCollisionPointCircle(mousePosition, coordinates[i].primaryPosition, coordinates[i].radius) && coordinates[i].active)
        {
            coordinate = &coordinates[i];
            coordinates[i].selected = true;
        }
        else coordinates[i].selected = false;
    }
    return coordinate;
}

void drawLegend(int screenWidth, int screenHeight, Coordinate *coordinate)
{
    int boxWidth = 300;
    int boxHeight = 100;
    int padding = 10;

    Rectangle legendBox = {
        .x = screenWidth - (boxWidth + padding),
        .y = screenHeight - (boxHeight + padding),
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
        if (!coordinates[i].active) continue;
        coordinates[i].color = coordinates[i].selected ? GREEN : BLUE;
        updateSecondaryLabel(coordinates + i, angle);
        drawCoordinate(coordinates[i], angle);
    }
}

void controlCamera(bool *isMoving, Vector2 *mouseDrag, Vector2 mousePosition, Camera2D *camera)
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
    {
        *isMoving = true;
        *mouseDrag = mousePosition;
    }

    if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) *isMoving = false;

    if (*isMoving)
    {
        Vector2 dragDelta = {mouseDrag->x - mousePosition.x, mouseDrag->y - mousePosition.y};

        camera->target.x += dragDelta.x;
        camera->target.y += dragDelta.y;

        *mouseDrag = mousePosition;
    }

    if (IsKeyPressed(KEY_SPACE)) recenterCamera(camera);
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

void clearCoordinates(Coordinate *coordinates, int total)
{
    for (int i = 0; i < total; i++) coordinates[i].active = false;
}

bool savedCoordinatesCheckCollision(Coordinate *coordinates, Vector2 mousePosition, int total)
{
    for (int i = 0; i < total; i++)
        if (CheckCollisionPointCircle(mousePosition, coordinates[i].primaryPosition, coordinates[i].radius) && coordinates[i].active)
            return true;
    return false;
}

void clearSelection(Coordinate *coordinates, int total)
{
    for (int i = 0; i < total; i++) coordinates[i].selected = false;
}

void drawPlaceholderCoordinate(Coordinate *coordinate, Vector2 position, float angle)
{
    coordinateFill(coordinate, '?', position, angle, 6, BLUE);
    drawCoordinate(*coordinate, angle);
}

void initializeCoordinates(Coordinate *coordinates, int total)
{
    for (int i = 0; i < total; i++)
    {
        coordinateFill(coordinates + i, 'A' + i, (Vector2){0}, 0.f, 6, BLUE);
        coordinates[i].active = false;
    }
}

int findAvailableIndex(Coordinate *coordinates, int total)
{
    for (int i = 0; i < total; i++)
        if (!coordinates[i].active) return i;
    return -1;
}

void updateCameraOffset(Camera2D *camera, int screenWidth, int screenHeight)
{
    camera->offset = (Vector2){
            .x = screenWidth / 2,
            .y = screenHeight / 2};
}

void drawAxiiOrientationArrows(Camera2D camera, int screenWidth, int screenHeight, Color color)
{
    double triangleSide = 12;
    double widthOffset = screenWidth/2;
    double heightOffset = -screenHeight/2;

    double xAxisTriangleTip = camera.target.x + widthOffset;
    double xAxisTriangleBase = xAxisTriangleTip - triangleSide;
    DrawTriangle(
        (Vector2){xAxisTriangleTip, 0},
        (Vector2){xAxisTriangleBase, -triangleSide/2},
        (Vector2){xAxisTriangleBase, triangleSide/2},
        DARKGRAY);
    DrawTriangleLines(
        (Vector2){xAxisTriangleTip, 0},
        (Vector2){xAxisTriangleBase, -triangleSide/2},
        (Vector2){xAxisTriangleBase, triangleSide/2},
        color);
    
    double yAxisTriangleTip = camera.target.y + heightOffset;
    double yAxisTriangleBase = yAxisTriangleTip + triangleSide;
    DrawTriangle(
        (Vector2){0, yAxisTriangleTip},
        (Vector2){-triangleSide/2, yAxisTriangleBase},
        (Vector2){triangleSide/2, yAxisTriangleBase},
        DARKGRAY);
    DrawTriangleLines(
        (Vector2){0, yAxisTriangleTip},
        (Vector2){-triangleSide/2, yAxisTriangleBase},
        (Vector2){triangleSide/2, yAxisTriangleBase},
        color);
}

void recenterCamera(Camera2D *camera)
{
    camera->target = (Vector2){0};
}
