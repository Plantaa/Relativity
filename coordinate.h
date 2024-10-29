#ifndef COORDINATE_H_
#define COORDINATE_H_

#include "raylib.h"

typedef struct coordinate
{
    Vector2 vector;
    int radius;
    Color color;
    char* primaryLabel;
    char* secondaryLabel;
}Coordinate;

Coordinate* coordinateCreate();
void coordinateFill(Coordinate* coordinate, Vector2 position, int radius, Color color);

#endif
