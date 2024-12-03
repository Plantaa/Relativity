#include <stdlib.h>
#include <stdio.h>

#include "raylib.h"
#include "math.h"
#include "coordinate.h"
#include "clear_button.h"
#include "utils.h"
#include "aside.h"
#include "legend.h"
#include "axis.h"
#include "coordinate_system.h"

void controlCamera(bool *isMoving, Vector2 *mouseDrag, Vector2 mousePosition, Camera2D *camera);
void drawEveryFrame(CoordinateSystem primarySystem, CoordinateSystem secondarySystem, Coordinate *coordinates, int total);
void drawAndSaveNewCoordinate(Coordinate *coordinates, Vector2 position, float angle, int total);
void drawSavedCoordinates(Coordinate *coordinates, int total, float angle);
Vector2 compensateMousePositionForCamera(Camera2D camera, Vector2 mousePosition);
void drawAngleMarker(Vector2 origin, float angleDegrees);
int selectCoordinate(Vector2 mousePosition, Coordinate *coordinates, int total);
void clearCoordinates(Coordinate *coordinates, int total);
bool savedCoordinatesCheckCollision(Coordinate *coordinates, Vector2 mousePosition, int total);
void clearSelection(Coordinate *coordinates, int total);
void drawPlaceholderCoordinate(Coordinate *coordinate, Vector2 position, float angle);
void initializeCoordinates(Coordinate *coordinates, int total);
int findAvailableIndex(Coordinate *coordinates, int total);
void updateCameraOffset(Camera2D *camera, Vector2 screenDimensions);
void drawAxiiOrientationArrows(Camera2D camera, int screenWidth, int screenHeight, Color color);
void drawAside(Aside *aside, Font font, int screenWidth);

int main(void)
{
    const int screenWidth = 900;
    const int screenHeight = 600;
    Vector2 screenDimensions = {.x = screenWidth, .y = screenHeight};
    InitWindow(screenWidth, screenHeight, "Parábola dos Geômetras no Reino de Emma");
    SetTargetFPS(60);

    Font font = LoadFontEx("./assets/fonts/Poppins-Regular.ttf", 32, NULL, 250);
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);

    Vector2 origin = {0};

    Camera2D camera = {
        .offset = {
            .x = screenWidth / 2,
            .y = screenHeight / 2},
        .target = origin,
        .rotation = 0.0f,
        .zoom = 1.0f};

    bool isMoving = false;
    Vector2 mouseDrag = {0};

    float angle = -(1.0f / 4.0f) * PI;

    CoordinateSystem primarySystem = { .origin = origin, .axiiColor = BLACK };
    CoordinateSystem secondarySystem = { .origin = origin, .axiiColor = RED };
    coordinateSystemAngleUpdate(&primarySystem, screenDimensions, 0);
    coordinateSystemAngleUpdate(&secondarySystem, screenDimensions, angle);

    int total = 26;
    Coordinate coordinates[total];
    initializeCoordinates(coordinates, total);
    int coordinateSelected = -1;
    Coordinate placeholderCoordinate;

    Aside aside = {
        .active = false,
        .box = {
            .x = screenWidth - 500,
            .y = 0,
            .height = screenHeight / 2,
            .width = 500}};

    Legend legend = {
        .active = false,
        .box = {
            .x = screenWidth - (300 + 10),
            .y = screenHeight - (100 + 10),
            .width = 300,
            .height = 100}};

    ClearButton clearButton = {
        .box = {
            .x = 10,
            .y = screenHeight - 40,
            .width = 100,
            .height = 30},
        .xTextPadding = 30,
        .yTextPadding = 7,
        .fontSize = 15};

    while (!WindowShouldClose())
    {
        Vector2 currentScreenDimensions = { .x = GetScreenWidth(), .y = GetScreenHeight() };
        Vector2 mousePosition = GetMousePosition();
        Vector2 mousePositionCompensated = compensateMousePositionForCamera(camera, mousePosition);

        coordinateSystemValuesUpdate(&primarySystem, currentScreenDimensions, camera);
        updateCameraOffset(&camera, currentScreenDimensions);
        controlCamera(&isMoving, &mouseDrag, mousePosition, &camera);
        clearButtonPositionUpdate(&clearButton, currentScreenDimensions.y);
        asidePositionUpdate(&aside, currentScreenDimensions.x);
        legendPositionUpdate(&legend, currentScreenDimensions);

        bool isMouseOnClearButton = CheckCollisionPointRec(mousePosition, clearButton.box);
        bool isMouseOnSavedCoordinate = savedCoordinatesCheckCollision(coordinates, mousePositionCompensated, total);

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);

            clearButtonDraw(clearButton, font);

            BeginMode2D(camera);
            {
                drawEveryFrame(primarySystem, secondarySystem, coordinates, total);

                if (IsKeyDown(KEY_R))
                    coordinateSystemAngleUpdate(&secondarySystem, currentScreenDimensions, calculateAngle(origin, mousePositionCompensated));
                if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && !(isMouseOnClearButton || isMouseOnSavedCoordinate))
                    drawPlaceholderCoordinate(&placeholderCoordinate, mousePositionCompensated, secondarySystem.angle);

                else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
                {
                    if (isMouseOnClearButton)
                    {
                        clearCoordinates(coordinates, total);
                        coordinateSelected = -1;
                    }
                    else if (isMouseOnSavedCoordinate)
                    {
                        clearSelection(coordinates, total);
                        coordinateSelected = selectCoordinate(mousePositionCompensated, coordinates, total);
                        coordinates[coordinateSelected].selected = true;
                    }
                    else drawAndSaveNewCoordinate(coordinates, mousePositionCompensated, secondarySystem.angle, total);
                }
            }
            EndMode2D();
            drawAside(&aside, font, currentScreenDimensions.x);
            if (coordinateSelected >= 0)
            {
                legendDraw(legend, coordinates + coordinateSelected, font);
                if (IsKeyPressed(KEY_DELETE)) {
                    coordinates[coordinateSelected].active = false;
                    coordinateSelected = -1;
                }
            }
        }
        EndDrawing();
    }
    UnloadFont(font);
    CloseWindow();
    return 0;
}

void drawEveryFrame(CoordinateSystem primarySystem, CoordinateSystem secondarySystem, Coordinate *coordinates, int total)
{
    drawAngleMarker(secondarySystem.origin, radiansToDegrees(secondarySystem.angle));
    coordinateSystemDraw(primarySystem);
    coordinateSystemDraw(secondarySystem);
    drawSavedCoordinates(coordinates, total, secondarySystem.angle);
}

void drawAndSaveNewCoordinate(Coordinate *coordinates, Vector2 position, float angle, int total)
{
    int index = findAvailableIndex(coordinates, total);
    if (index < 0) return;

    Coordinate *newCoordinate = coordinates + index;
    coordinateFill(newCoordinate, 'A' + index, position, angle, 6, BLUE);

    coordinateDraw(*newCoordinate, angle);
}

void drawAside(Aside *aside, Font font, int screenWidth) {
    if (IsKeyPressed(KEY_F1)) aside->active = !aside->active;
    if (aside->active) {
        asideDraw(*aside, font);
    } else {
        DrawTextEx(font, "[F1] - Abrir Menu", (Vector2) { .x = screenWidth - 130, .y = 0 }, 16, 1, BLACK);
    };
}

int selectCoordinate(Vector2 mousePosition, Coordinate *coordinates, int total)
{
    for (int i = 0; i < total; i++)
    {
        if (coordinates[i].active && CheckCollisionPointCircle(mousePosition, coordinates[i].primaryPosition, coordinates[i].radius) && coordinates[i].selected == false)
        {
            if(coordinates[i].selected == true) {
                coordinates[i].selected = false;
                return -1;
            }

            coordinates[i].selected = true;
            return i;
        } 
    }
    return -1;
}

void drawSavedCoordinates(Coordinate *coordinates, int total, float angle)
{
    for (int i = 0; i < total; i++)
    {
        if (!coordinates[i].active) continue;
        coordinates[i].color = coordinates[i].selected ? GREEN : BLUE;
        coordinateSecondaryLabelUpdate(coordinates + i, angle);
        coordinateDraw(coordinates[i], angle);
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

    if (IsKeyPressed(KEY_SPACE)) camera->target = (Vector2){0};
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
    coordinateDraw(*coordinate, angle);
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

void updateCameraOffset(Camera2D *camera, Vector2 screenDimensions)
{
    camera->offset = (Vector2){
            .x = screenDimensions.x / 2,
            .y = screenDimensions.y / 2};
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
