#include <stdlib.h>
#include <stdio.h>

#include "math.h"
#include "raylib.h"

#include "aside.h"
#include "axis.h"
#include "clear_button.h"
#include "coordinate.h"
#include "coordinate_system.h"
#include "legend.h"
#include "utils.h"

#define MIN_ZOOM 0.1f
#define MAX_ZOOM 10.0f
#define ZOOM_STEP 1.1f

void clearCoordinates(Coordinate *coordinates, int total);
void dragCoordinate(Coordinate *coordinate, float zoom);
void drawAngleMarker(Vector2 origin, float angleDegrees, float zoom);
void drawAside(Aside *aside, Font font, int screenWidth);
void drawAndSaveNewCoordinate(Coordinate *coordinates, Vector2 position, float angle, int total, bool labels, float zoom);
void drawEveryFrame(CoordinateSystem primarySystem, CoordinateSystem secondarySystem, Coordinate *coordinates, int total, bool labels, float zoom);
void drawSavedCoordinates(Coordinate *coordinates, int total, float angle, bool labels, float zoom);
void drawPlaceholderCoordinate(Coordinate *coordinate, Vector2 position, float angle, bool labels, float zoom);
void initializeCoordinates(Coordinate *coordinates, int total);
int findAvailableIndex(const Coordinate *const coordinates, int total);
int getSavedCoordinateMouseHover(const Coordinate *const coordinates, int total, Vector2 mousePosition, float zoom);
void updateCameraOffset(Camera2D *camera);
void zoomCameraAtPoint(Camera2D *camera, Vector2 screenPoint, float wheel);

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

    /* Load background image */
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
            .height = screenHeight / 2 + 50,
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

    bool isCoordinateDragging = false;
    bool labels = false;

    while (!WindowShouldClose())
    {
        updateCameraOffset(&camera);

        float mouseWheel = GetMouseWheelMove();
        if (mouseWheel != 0)
        {
            zoomCameraAtPoint(&camera, GetMousePosition(), mouseWheel);
        }

        Vector2 mousePosition = GetMousePosition();
        Vector2 mousePositionCompensated = GetScreenToWorld2D(mousePosition, camera);
        Vector2 mouseDelta = GetMouseDelta();

        coordinateSystemValuesUpdate(&primarySystem, camera);
        clearButtonPositionUpdate(&clearButton, GetScreenHeight());
        asidePositionUpdate(&aside, GetScreenWidth());
        legendPositionUpdate(&legend);

        bool isMouseOnClearButton = CheckCollisionPointRec(mousePosition, clearButton.box);
        int savedCoordinateMouseHover = getSavedCoordinateMouseHover(coordinates, total, mousePositionCompensated, camera.zoom);
        bool isMouseOnCanvas = savedCoordinateMouseHover < 0 && !isMouseOnClearButton;
        bool isMouseOnSelectedCoordinate = coordinateSelected >= 0 && coordinateSelected == savedCoordinateMouseHover;
        bool isMouseDragging = mouseDelta.x || mouseDelta.y;

        if (IsKeyPressed(KEY_L))
        {
            labels = !labels;
        }

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);

            BeginMode2D(camera);
            {
                DrawTextureV(backgroundTexture, (Vector2){origin.x - GetScreenWidth() / 2, origin.y - GetScreenHeight() / 2}, WHITE);
                drawEveryFrame(primarySystem, secondarySystem, coordinates, total, labels, camera.zoom);

                if (IsKeyDown(KEY_R))
                {
                    float newAngle = calculateAngle((Vector2){}, mousePositionCompensated);
                    coordinateSystemAngleUpdate(&secondarySystem, newAngle);
                }

                if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
                {
                    Vector2 mouseDrag = GetMouseDelta();

                    camera.target.x -= mouseDrag.x / camera.zoom;
                    camera.target.y -= mouseDrag.y / camera.zoom;
                }

                if (IsKeyPressed(KEY_SPACE))
                {
                    camera.target = (Vector2){GetScreenWidth() * 0.092, (GetScreenHeight() / -20)};
                    camera.zoom = 1.0f;
                }

                if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
                {
                    if (isCoordinateDragging)
                    {
                        dragCoordinate(coordinates + coordinateSelected, camera.zoom);
                    }
                    else if (isMouseOnSelectedCoordinate && isMouseDragging)
                    {
                        isCoordinateDragging = true;
                    }
                    else if (isMouseOnCanvas)
                    {
                        drawPlaceholderCoordinate(&placeholderCoordinate, mousePositionCompensated, secondarySystem.angle, labels, camera.zoom);
                        coordinates[coordinateSelected].selected = false;
                        coordinateSelected = -1;
                    }
                }

                if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
                {
                    isCoordinateDragging = false;
                    if (savedCoordinateMouseHover >= 0)
                    {
                        if (isMouseOnSelectedCoordinate)
                        {   
                            coordinates[coordinateSelected].selected = false;
                            coordinateSelected = -1;
                        }
                        else
                        {
                            coordinates[coordinateSelected].selected = false;
                            coordinateSelected = savedCoordinateMouseHover;
                            coordinates[coordinateSelected].selected = true;
                        }
                    }
                    else if (isMouseOnClearButton)
                    {
                        clearCoordinates(coordinates, total);
                        coordinates[coordinateSelected].selected = false;
                        coordinateSelected = -1;
                    }
                    else {
                        drawAndSaveNewCoordinate(coordinates, mousePositionCompensated, secondarySystem.angle, total, labels, camera.zoom);
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
        {
            TakeScreenshot("screenshot.png");
        }
        EndDrawing();
    }
    UnloadTexture(backgroundTexture);
    UnloadFont(font);
    CloseWindow();
    return 0;
}

void drawEveryFrame(CoordinateSystem primarySystem, CoordinateSystem secondarySystem, Coordinate *coordinates, int total, bool labels, float zoom)
{
    drawAngleMarker((Vector2){0}, radiansToDegrees(secondarySystem.angle), zoom);
    coordinateSystemDraw(primarySystem, zoom);
    coordinateSystemDraw(secondarySystem, zoom);
    drawSavedCoordinates(coordinates, total, secondarySystem.angle, labels, zoom);
}

void drawAndSaveNewCoordinate(Coordinate *coordinates, Vector2 position, float angle, int total, bool labels, float zoom)
{
    int index = findAvailableIndex(coordinates, total);
    if (index < 0)
    {
        return;
    }

    Coordinate *newCoordinate = coordinates + index;
    coordinateFill(newCoordinate, 'A' + index, position, angle, 6, BLUE);

    coordinateDraw(*newCoordinate, angle, labels, zoom);
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

void drawSavedCoordinates(Coordinate *coordinates, int total, float angle, bool labels, float zoom)
{
    for (int i = 0; i < total; i++)
    {
        if (!coordinates[i].active)
        {
            continue;
        }
        coordinates[i].color = coordinates[i].selected ? GREEN : BLUE;
        coordinateLabelUpdate(coordinates + i, angle);
        coordinateDraw(coordinates[i], angle, labels, zoom);
    }
}

void drawAngleMarker(Vector2 origin, float angleDegrees, float zoom)
{
    char markerText[10];
    snprintf(markerText, 10, "%.2f", -angleDegrees);
    DrawCircleSectorLines(origin, 50 / zoom, 0.f, angleDegrees, 30, DARKGRAY);
    DrawTextEx(GetFontDefault(), markerText, (Vector2){origin.x + 60 / zoom, origin.y + 20 / zoom}, 10 / zoom, 1 / zoom, DARKGRAY);
}

void clearCoordinates(Coordinate *coordinates, int total)
{
    for (int i = 0; i < total; i++)
    {
        coordinates[i].active = false;
    }
}

void drawPlaceholderCoordinate(Coordinate *coordinate, Vector2 position, float angle, bool labels, float zoom)
{
    coordinateFill(coordinate, '?', position, angle, 6, BLUE);
    coordinateDraw(*coordinate, angle, labels, zoom);
}

void initializeCoordinates(Coordinate *coordinates, int total)
{
    for (int i = 0; i < total; i++)
    {
        coordinateFill(coordinates + i, 'A' + i, (Vector2){0}, 0.f, 6, BLUE);
        coordinates[i].active = false;
    }
}

int findAvailableIndex(const Coordinate *const coordinates, int total)
{
    for (int i = 0; i < total; i++)
    {
        if (!coordinates[i].active)
        {
            return i;
        }
    }
    return -1;
}

void updateCameraOffset(Camera2D *camera)
{
    camera->offset = (Vector2){
        .x = GetScreenWidth() / 2,
        .y = GetScreenHeight() / 2};
}

/* Zooms keeping the world point under screenPoint fixed on screen */
void zoomCameraAtPoint(Camera2D *camera, Vector2 screenPoint, float wheel)
{
    Vector2 worldBefore = GetScreenToWorld2D(screenPoint, *camera);

    camera->zoom *= powf(ZOOM_STEP, wheel);
    camera->zoom = fminf(fmaxf(camera->zoom, MIN_ZOOM), MAX_ZOOM);

    Vector2 worldAfter = GetScreenToWorld2D(screenPoint, *camera);
    camera->target.x += worldBefore.x - worldAfter.x;
    camera->target.y += worldBefore.y - worldAfter.y;
}

void selectCoordinate(Coordinate *const coordinates, int *const coordinateSelected)
{
}

int getSavedCoordinateMouseHover(const Coordinate *const coordinates, int total, Vector2 mousePosition, float zoom)
{
    for (size_t i = 0; i < total; i++)
    {
        if (coordinates[i].active && CheckCollisionPointCircle(mousePosition, coordinates[i].primaryPosition, coordinates[i].radius / zoom))
        {
            return i;
        }
    }
    return -1;
}

void dragCoordinate(Coordinate *coordinate, float zoom)
{
    Vector2 mouseDelta = GetMouseDelta();
    coordinate->primaryPosition.x += mouseDelta.x / zoom;
    coordinate->primaryPosition.y += mouseDelta.y / zoom;
}
