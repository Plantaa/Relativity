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
    coordinate->selected = false;

    coordinate->primaryLabel = (char *)malloc(sizeof(char) * 20);
    snprintf(coordinate->primaryLabel, 20, "(%.2f, %.2f)", position.x, -position.y);

    coordinate->secondaryLabel = (char *)malloc(sizeof(char) * 20);
    snprintf(coordinate->secondaryLabel, 20, "(%.2f, %.2f)", coordinate->secondaryPosition.x, coordinate->secondaryPosition.y);

    coordinate->nameLabel = (char *)malloc(sizeof(char) * 2);
    snprintf(coordinate->nameLabel, 2, "%c", coordinate->name);

    printCoordinate(coordinate);
}

void drawCoordinate(Coordinate coordinate)
{
    DrawCircleV(coordinate.primaryPosition, coordinate.radius, coordinate.color);
    DrawText(coordinate.primaryLabel, coordinate.primaryPosition.x + 5, coordinate.primaryPosition.y + 5, 10, BLACK);
    DrawText(coordinate.secondaryLabel, coordinate.primaryPosition.x + 5, coordinate.primaryPosition.y + 17, 10, RED);
    DrawText(coordinate.nameLabel, coordinate.primaryPosition.x - 13, coordinate.primaryPosition.y - 13, 10, DARKBLUE);
}

// Function to draw the components for System
void drawPrimaryComponents(Vector2 point, Color color)
{
    int dotNumber = 50;

    // Project x component along the rotated x-axis
    Vector2 xComponent = (Vector2){point.x, 0};
    drawDottedLine(point, xComponent, dotNumber, color);

    // Project y component along the rotated y-axis
    Vector2 yComponent = (Vector2){0, point.y};
    drawDottedLine(point, yComponent, dotNumber, color);
}

void drawSecondaryComponents(Vector2 primaryPosition, Vector2 secondaryPosition, float angle, Color color)
{
    int dotNumber = 50;

    Vector2 xComponent = convertToSystemA((Vector2){secondaryPosition.x, 0}, angle);
    drawDottedLine(primaryPosition, xComponent, dotNumber, color);

    Vector2 yComponent = convertToSystemA((Vector2){0, secondaryPosition.y}, angle);
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

void updateSecondaryLabel(Coordinate *coordinate, float angle)
{
    coordinate->secondaryPosition = convertToSystemB(coordinate->primaryPosition, angle);
    snprintf(coordinate->secondaryLabel, 20, "(%.2f, %.2f)", coordinate->secondaryPosition.x, -coordinate->secondaryPosition.y);
}
