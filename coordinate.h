#ifndef COORDINATE_H_
#define COORDINATE_H_
#ifndef RAYLIB_H_
#define RAYLIB_H_

#include "raylib.h"

typedef struct coordinate
{
    Vector2 vector;
    int radius;
    Color color;
}Coordinate;

Coordinate* coordinateCreate();
void coordinateFill(Coordinate* coordinate, int x, int y, int radius, Color color);

#endif
#endif