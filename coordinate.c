#include <stdio.h>
#include <stdlib.h>

#include "coordinate.h"

Coordinate* coordinateCreate() {
    return (Coordinate*) malloc(sizeof(Coordinate));
}

void coordinateFill(Coordinate* coordinate, Vector2 position, int radius, Color color) {
    coordinate->vector = position;
    coordinate->radius = radius;
    coordinate->color = color;
}