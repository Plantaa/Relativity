#ifndef COORDINATE_H_
#define COORDINATE_H_

#include <math.h>
#include "raylib.h"

typedef struct coordinate
{
    char name;
    Vector2 primaryPosition;
    Vector2 secondaryPosition;
    int radius;
    Color color;
    char *primaryLabel;
    char *secondaryLabel;
    char *nameLabel;
    bool selected;
    bool active;
} Coordinate;

void coordinateFill(Coordinate *coordinate, const char name, Vector2 position, float angle, int radius, Color color);
void coordinateDraw(Coordinate coordinate, float angle, bool labels);
void coordinatePrimaryComponentsDraw(Vector2 point, Color color);
void coordinateSecondaryComponentsDraw(Vector2 primaryPosition, Vector2 secondaryPosition, float angle, Color color);
void coordinatePrint(Coordinate *coordinate);
void coordinateSecondaryLabelUpdate(Coordinate *coordinate, float angle);

#endif
