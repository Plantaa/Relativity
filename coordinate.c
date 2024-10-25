#include <stdio.h>
#include <stdlib.h>

#include "coordinate.h"

Coordinate* coordinateCreate() {
    return (Coordinate*) malloc(sizeof(Coordinate));
}

void coordinateFill(Coordinate* coordinate, int x, int y, int radius, Color color) {
    coordinate->x = x;
    coordinate->y = y;
    coordinate->radius = radius;
    coordinate->color = color;
}