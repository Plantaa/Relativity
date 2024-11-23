#include <stdio.h>
#include <stdlib.h>

#include "coordinate.h"
#include "utils.h"

Coordinate *coordinateCreate()
{
    return (Coordinate *)malloc(sizeof(Coordinate));
}

void coordinateFill(Coordinate *coordinate, const char name, Vector2 position, float angle, int radius, Color color)
{
    coordinate->primaryPosition = position;
    coordinate->secondaryPosition = convertToSystemB(position, angle);
    coordinate->radius = radius;
    coordinate->color = color;
    coordinate->name = name;
    coordinate->active = true;

    coordinate->primaryLabel = (char *)malloc(sizeof(char) * 20);
    snprintf(coordinate->primaryLabel, 20, "(%.2f, %.2f)", position.x, -position.y);

    coordinate->secondaryLabel = (char *)malloc(sizeof(char) * 20);
    snprintf(coordinate->secondaryLabel, 20, "(%.2f, %.2f)", coordinate->secondaryPosition.x, coordinate->secondaryPosition.y);

    printCoordinate(coordinate);

}

void updateSecondaryLabel(Coordinate *coordinate, float angle)
{
    coordinate->secondaryPosition = convertToSystemB(coordinate->primaryPosition, angle);
    snprintf(coordinate->secondaryLabel, 20, "(%.2f, %.2f)", coordinate->secondaryPosition.x, -coordinate->secondaryPosition.y);
}

// Function to draw the components for System
void drawPrimaryComponents(Vector2 point, Color color)
{
    int dotNumber = 50;

    // Project x component along the rotated x-axis
    Vector2 xComponent = (Vector2){point.x, 0};
    drawDottedLine(point, xComponent, dotNumber, color);

    // Project y component along the rotated y-axis
    Vector2 yComponentB = (Vector2){0, point.y};
    drawDottedLine(point, yComponentB, dotNumber, color);
}

void drawSecondaryComponents(Vector2 primaryPosition, Vector2 secondaryPosition, float angle, Color color)
{
    int dotNumber = 50;

    Vector2 xComponentB = convertToSystemA((Vector2){secondaryPosition.x, 0}, angle);
    drawDottedLine(primaryPosition, xComponentB, dotNumber, color);

    Vector2 yComponent = convertToSystemA((Vector2) {0, secondaryPosition.y}, angle);
    drawDottedLine(primaryPosition, yComponent, dotNumber, color);
}

void printCoordinate(Coordinate *coord)
{
    printf("Coordinate Details:\n");
    printf("  Name: %c\n", coord->name);
    printf("  Active: %s\n", coord->active ? "Yes" : "No");
    printf("  Primary Position: (%.2f, %.2f)\n", coord->primaryPosition.x, coord->primaryPosition.y);
    printf("  Secondary Position: (%.2f, %.2f)\n", coord->secondaryPosition.x, coord->secondaryPosition.y);
    printf("  Radius: %d\n", coord->radius);
    printf("  Color: (R: %d, G: %d, B: %d, A: %d)\n", coord->color.r, coord->color.g, coord->color.b, coord->color.a);
    printf("  Primary Label: %s\n", coord->primaryLabel != NULL ? coord->primaryLabel : "N/A");
    printf("  Secondary Label: %s\n", coord->secondaryLabel != NULL ? coord->secondaryLabel : "N/A");
    printf("\n");
}
