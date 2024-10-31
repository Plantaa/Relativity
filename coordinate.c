#include <stdio.h>
#include <stdlib.h>

#include "coordinate.h"
#include "utils.h"

Coordinate *coordinateCreate()
{
    return (Coordinate *)malloc(sizeof(Coordinate));
}

void coordinateFill(Coordinate *coordinate, Vector2 position, float angle, int radius, Color color)
{
    coordinate->primaryPosition = position;
    coordinate->secondaryPostion = convertToSystemB(position, angle);
    coordinate->radius = radius;
    coordinate->color = color;

    coordinate->primaryLabel = (char *)malloc(sizeof(char) * 20);
    snprintf(coordinate->primaryLabel, 20, "(%.2f, %.2f)", position.x, -position.y);

    coordinate->secondaryLabel = (char *)malloc(sizeof(char) * 20);
    snprintf(coordinate->secondaryLabel, 20, "(%.2f, %.2f)", coordinate->secondaryPostion.x, coordinate->secondaryPostion.y);
}

void updateSecondaryLabel(Coordinate *coordinate, float angle)
{
    coordinate->secondaryPostion = convertToSystemB(coordinate->primaryPosition, angle);
    snprintf(coordinate->secondaryLabel, 20, "(%.2f, %.2f)", coordinate->secondaryPostion.x, -coordinate->secondaryPostion.y);
}

// Function to draw the components for System
void drawComponentsSystem(Vector2 point, float theta, Color color)
{
    int dotNumber = 50;

    // Project x component along the rotated x-axis
    Vector2 xComponentB = convertToSystemB((Vector2){point.x, 0}, theta);
    drawDottedLine(point, xComponentB, dotNumber, color);

    // Project y component along the rotated y-axis
    Vector2 yComponentB = convertToSystemB((Vector2){0, point.y}, theta);
    drawDottedLine(point, yComponentB, dotNumber, color);
}
