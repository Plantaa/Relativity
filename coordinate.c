#include <stdio.h>
#include <stdlib.h>

#include "coordinate.h"

Coordinate *coordinateCreate()
{
    return (Coordinate *)malloc(sizeof(Coordinate));
}

void coordinateFill(Coordinate *coordinate, Vector2 position, float angle, int radius, Color color)
{
    coordinate->vector = position;
    coordinate->radius = radius;
    coordinate->color = color;

    coordinate->primaryLabel = (char *)malloc(sizeof(char) * 20);
    snprintf(coordinate->primaryLabel, 20, "(%.2f, %.2f)", position.x, -position.y);

    coordinate->secondaryLabel = (char *)malloc(sizeof(char) * 20);
    updateSecondaryLabel(coordinate, angle);
}

void updateSecondaryLabel(Coordinate* coordinate, float angle)
{
    float angleSin = sinf(angle);
    float angleCos = cosf(angle);

    Vector2 secondarySystemCoordinates = {
        .x = coordinate->vector.x * angleCos + coordinate->vector.y * angleSin,
        .y = -coordinate->vector.x * angleSin + coordinate->vector.y * angleCos};
    snprintf(coordinate->secondaryLabel, 20, "(%.2f, %.2f)", secondarySystemCoordinates.x, -secondarySystemCoordinates.y);
}
