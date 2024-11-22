#ifndef COORDINATE_H_
#define COORDINATE_H_

#include <math.h>
#include "raylib.h"

typedef struct coordinate
{
    Vector2 primaryPosition;
    Vector2 secondaryPosition;
    int radius;
    Color color;
    char* primaryLabel;
    char* secondaryLabel;
    char name;
    bool active;
}Coordinate;

Coordinate* coordinateCreate();
void coordinateFill(Coordinate* coordinate, const char name, Vector2 position, float angle, int radius, Color color);
void updateSecondaryLabel(Coordinate* coordinate, float angle);
void drawComponentsSystem(Vector2 point, float theta, Color color);
void printCoordinate(Coordinate* coordinate);

#endif
