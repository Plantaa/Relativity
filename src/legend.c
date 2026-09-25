#include <stdio.h>
#include <stdlib.h>

#include "legend.h"
#include "coordinate.h"
#include "utils.h"

Legend *legend()
{
    return (Legend *)malloc(sizeof(Legend));
}

void legendDraw(Legend legend, const Coordinate* const coordinate, Font font) 
{
    int fontSize = 16;
    int fontSpace = 1;

    DrawRectangleRec(legend.box, WHITE);
    DrawRectangleLinesEx(legend.box, 2, RED);

    DrawTextEx(
        font, 
        "Legenda: ",
        (Vector2) {.x = legend.box.x + 10, .y = legend.box.y + 10},
        fontSize,
        fontSpace,
        BLACK);

    DrawTextEx(
        font, 
        coordinate->nameLabel,
        (Vector2) {.x = legend.box.x + 10, .y = legend.box.y + 23},
        fontSize + 4,
        fontSpace,
        BLUE);
    DrawTextEx(
        font, 
        coordinate->primaryLabel,
        (Vector2) {.x = legend.box.x + 10, .y = legend.box.y + 40},
        fontSize,
        fontSpace,
        BLACK);
    DrawTextEx(
        font, 
        coordinate->secondaryLabel,
        (Vector2) {.x = legend.box.x + 10, .y = legend.box.y + 53},
        fontSize,
        fontSpace,
        RED);

    double distance = sqrt(pow(coordinate->primaryPosition.x, 2) + pow(coordinate->primaryPosition.y, 2));
    char result[100];
    snprintf(result, sizeof(result), "Dist Origem: %.2fm, %.2fft", distance, metersToFeet(distance));
        DrawTextEx(
        font, 
        result,
        (Vector2) {.x = legend.box.x + 10, .y = legend.box.y + 70},
        fontSize,
        fontSpace,
        BLACK);
}

void legendPositionUpdate(Legend *legend) 
{
    legend->box.x = GetScreenWidth() - legend->box.width;
    legend->box.y = GetScreenHeight() - legend->box.height;
}
