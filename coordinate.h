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

Coordinate *coordinateCreate();
void coordinateFill(Coordinate *coordinate, const char name, Vector2 position, float angle, int radius, Color color);
void drawCoordinate(Coordinate coordinate);
void drawPrimaryComponents(Vector2 point, Color color);
void drawSecondaryComponents(Vector2 primaryPosition, Vector2 secondaryPosition, float angle, Color color);
void printCoordinate(Coordinate *coordinate);
void updateSecondaryLabel(Coordinate *coordinate, float angle);

#endif
