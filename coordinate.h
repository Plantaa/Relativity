#ifndef COORDINATE_H_
#define COORDINATE_H_

#include <math.h>
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
void coordinateFill(Coordinate* coordinate, Vector2 position, float angle, int radius, Color color);
void updateSecondaryLabel(Coordinate* coordinate, float angle);

#endif
