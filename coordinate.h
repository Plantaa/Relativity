#ifndef COORDINATE_H_
#define COORDINATE_H_

#include <math.h>
#include "raylib.h"

typedef struct coordinate
{
    Vector2 primaryPosition;
    Vector2 secondaryPostion;
    int radius;
    Color color;
    char* primaryLabel;
    char* secondaryLabel;
    bool selected;
}Coordinate;

Coordinate* coordinateCreate();
void coordinateFill(Coordinate* coordinate, Vector2 position, float angle, int radius, Color color);
void updateSecondaryLabel(Coordinate* coordinate, float angle);
void drawComponentsSystem(Vector2 point, float theta, Color color);

#endif
