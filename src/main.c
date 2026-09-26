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

void drawEveryFrame(CoordinateSystem primarySystem, CoordinateSystem secondarySystem, Coordinate *coordinates, int total);
void drawAndSaveNewCoordinate(Coordinate *coordinates, Vector2 position, float angle, int total);
void drawSavedCoordinates(Coordinate *coordinates, int total, float angle);
void drawAngleMarker(Vector2 origin, float angleDegrees);
void clearCoordinates(Coordinate *coordinates, int total);
void clearSelection(Coordinate *coordinates, int total);
void drawPlaceholderCoordinate(Coordinate *coordinate, Vector2 position, float angle);
void initializeCoordinates(Coordinate *coordinates, int total);
int findAvailableIndex(Coordinate *coordinates, int total);
void updateCameraOffset(Camera2D *camera);
void drawAxiiOrientationArrows(Camera2D camera, int screenWidth, int screenHeight, Color color);
void drawAside(Aside *aside, Font font, int screenWidth);
int getSavedCoordinateMouseHover(Coordinate *coordinates, int total, Vector2 mousePosition);
void dragCoordinate(Coordinate *coordinate);

int main(void)
{
    const float screenWidth = 1200;
    const float screenHeight = 800;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);

    InitWindow(screenWidth, screenHeight, "Parábola dos Geômetras no Reino de Emma");

    SetTargetFPS(60);

    Vector2 origin = {
        screenWidth * 0.092,
        screenHeight / -20};

    Camera2D camera = {
        .offset = {
            .x = screenWidth / 2,
            .y = screenHeight / 2},
        .target = origin,
        .rotation = 0.0f,
        .zoom = 1.0f};

    /* Load backgroud image */
    Image background = LoadImage("assets/Oreum.png");
    ImageResize(&background, screenWidth, screenHeight);
    Texture2D backgroundTexture = LoadTextureFromImage(background);
    UnloadImage(background);

    /* Load font */
    Font font = LoadFontEx("./assets/fonts/Poppins-Regular.ttf", 32, NULL, 250);
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);

    float angle = -(1.0f / 4.0f) * PI;

    CoordinateSystem primarySystem = {
        .origin = origin,
        .axiiColor = BLACK};

    CoordinateSystem secondarySystem = {
        .origin = origin,
        .axiiColor = RED};

    coordinateSystemAngleUpdate(&primarySystem, 0);
    coordinateSystemAngleUpdate(&secondarySystem, angle);

    int total = 26;
    /* VLAs not supported in Windows */
    Coordinate coordinates[26];
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

    bool isDragging = false;
    bool isMoving = false;

    while (!WindowShouldClose())
    {
        Vector2 mousePosition = GetMousePosition();
        Vector2 mousePositionCompensated = GetScreenToWorld2D(mousePosition, camera);

        coordinateSystemValuesUpdate(&primarySystem, camera);
        updateCameraOffset(&camera);
        clearButtonPositionUpdate(&clearButton, GetScreenHeight());
        asidePositionUpdate(&aside, GetScreenWidth());
        legendPositionUpdate(&legend);

        bool isMouseOnClearButton = CheckCollisionPointRec(mousePosition, clearButton.box);
        int savedCoordinateMouseHover = getSavedCoordinateMouseHover(coordinates, total, mousePositionCompensated);

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);

            BeginMode2D(camera);
            {
                DrawTextureV(backgroundTexture, (Vector2){origin.x - GetScreenWidth() / 2, origin.y - GetScreenHeight() / 2}, WHITE);
                drawEveryFrame(primarySystem, secondarySystem, coordinates, total);

                if (IsKeyDown(KEY_R))
                {
                    coordinateSystemAngleUpdate(&secondarySystem, calculateAngle((Vector2){0}, mousePositionCompensated));
                }

                if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
                {
                    isMoving = true;
                    Vector2 mouseDrag = GetMouseDelta();

                    if (isMoving)
                    {
                        camera.target.x -= mouseDrag.x;
                        camera.target.y -= mouseDrag.y;
                    }

                }
                
                if (IsKeyPressed(KEY_SPACE))
                {
                    camera.target = (Vector2){GetScreenWidth() * 0.092, (GetScreenHeight() / -20)};
                }

                else if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT))
                {
                    isMoving = false;
                }

                if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
                {
                    if (isDragging)
                    {
                        dragCoordinate(coordinates + coordinateSelected);
                    }
                    else if ((savedCoordinateMouseHover >= 0) && (coordinateSelected == savedCoordinateMouseHover))
                    {
                        isDragging = true;
                    }
                    else if (savedCoordinateMouseHover < 0 && !isMouseOnClearButton)
                    {
                        drawPlaceholderCoordinate(&placeholderCoordinate, mousePositionCompensated, secondarySystem.angle);
                    }
                }

                if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
                {
                    isDragging = false;
                    if (isMouseOnClearButton)
                    {
                        clearCoordinates(coordinates, total);
                        coordinateSelected = -1;
                    }
                    else if (savedCoordinateMouseHover >= 0)
                    {
                        if (coordinateSelected == savedCoordinateMouseHover)
                        {
                            coordinates[coordinateSelected].selected = !coordinates[coordinateSelected].selected;
                            coordinateSelected = coordinates[coordinateSelected].selected ? coordinateSelected : -1;
                        }
                        else
                        {
                            coordinates[coordinateSelected].selected = false;
                            coordinateSelected = savedCoordinateMouseHover;
                            coordinates[coordinateSelected].selected = true;
                        }
                    }
                    else
                    {
                        drawAndSaveNewCoordinate(coordinates, mousePositionCompensated, secondarySystem.angle, total);
                    }
                }
            }
            EndMode2D();

            clearButtonDraw(clearButton, font);
            drawAside(&aside, font, GetScreenWidth());

            if (coordinateSelected >= 0)
            {
                legendDraw(legend, coordinates + coordinateSelected, font);
                if (IsKeyPressed(KEY_DELETE))
                {
                    coordinates[coordinateSelected].active = false;
                    coordinateSelected = -1;
                }
            }
        }
        if (IsKeyPressed(KEY_S))
            TakeScreenshot("screenshot.png");
        EndDrawing();
    }
    UnloadTexture(backgroundTexture);
    UnloadFont(font);
    CloseWindow();
    return 0;
}

void drawEveryFrame(CoordinateSystem primarySystem, CoordinateSystem secondarySystem, Coordinate *coordinates, int total)
{
    drawAngleMarker((Vector2){0}, radiansToDegrees(secondarySystem.angle));
    coordinateSystemDraw(primarySystem);
    coordinateSystemDraw(secondarySystem);
    drawSavedCoordinates(coordinates, total, secondarySystem.angle);
}

void drawAndSaveNewCoordinate(Coordinate *coordinates, Vector2 position, float angle, int total)
{
    int index = findAvailableIndex(coordinates, total);
    if (index < 0)
        return;

    Coordinate *newCoordinate = coordinates + index;
    coordinateFill(newCoordinate, 'A' + index, position, angle, 6, BLUE);

    coordinateDraw(*newCoordinate, angle);
}

void drawAside(Aside *aside, Font font, int screenWidth)
{
    if (IsKeyPressed(KEY_F1))
        aside->active = !aside->active;
    if (aside->active)
    {
        asideDraw(*aside, font);
    }
    else
    {
        DrawTextEx(font, "[F1] - Abrir Menu", (Vector2){.x = screenWidth - 130, .y = 0}, 16, 1, BLACK);
    };
}

void drawSavedCoordinates(Coordinate *coordinates, int total, float angle)
{
    for (int i = 0; i < total; i++)
    {
        if (!coordinates[i].active)
            continue;
        coordinates[i].color = coordinates[i].selected ? GREEN : BLUE;
        coordinateSecondaryLabelUpdate(coordinates + i, angle);
        coordinateDraw(coordinates[i], angle);
    }
}

void drawAngleMarker(Vector2 origin, float angleDegrees)
{
    char markerText[10];
    snprintf(markerText, 10, "%.2f", -angleDegrees);
    DrawCircleSectorLines(origin, 50, 0.f, angleDegrees, 30, DARKGRAY);
    DrawText(markerText, 60, 20, 10, DARKGRAY);
}

void clearCoordinates(Coordinate *coordinates, int total)
{
    for (int i = 0; i < total; i++)
        coordinates[i].active = false;
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
        if (!coordinates[i].active)
            return i;
    return -1;
}

void updateCameraOffset(Camera2D *camera)
{
    camera->offset = (Vector2){
        .x = GetScreenWidth() / 2,
        .y = GetScreenHeight() / 2};
}

int getSavedCoordinateMouseHover(Coordinate *coordinates, int total, Vector2 mousePosition)
{
    for (size_t i = 0; i < total; i++)
    {
        if (coordinates[i].active && CheckCollisionPointCircle(mousePosition, coordinates[i].primaryPosition, coordinates[i].radius))
        {
            return i;
        }
    }
    return -1;
}

void dragCoordinate(Coordinate *coordinate)
{
    Vector2 mouseDelta = GetMouseDelta();
    coordinate->primaryPosition.x += mouseDelta.x;
    coordinate->primaryPosition.y += mouseDelta.y;
}
