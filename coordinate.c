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

    coordinate->primaryLabel = (char *)malloc(sizeof(char) * 50);
    snprintf(coordinate->primaryLabel, 50, "(%.2fm, %.2fft)", position.x, -metersToFeet(position.y));

    coordinate->secondaryLabel = (char *)malloc(sizeof(char) * 50);
    snprintf(coordinate->secondaryLabel, 50, "(%.2fm, %.2fft)", coordinate->secondaryPosition.x, -metersToFeet(coordinate->secondaryPosition.y));

    coordinate->nameLabel = (char *)malloc(sizeof(char) * 2);
    snprintf(coordinate->nameLabel, 2, "%c", coordinate->name);

    coordinatePrint(coordinate);
}

void coordinateDraw(Coordinate coordinate, float angle)
{
    DrawCircleV(coordinate.primaryPosition, coordinate.radius, coordinate.color);
    coordinatePrimaryComponentsDraw(coordinate.primaryPosition, BLACK);
    coordinateSecondaryComponentsDraw(coordinate.primaryPosition, coordinate.secondaryPosition, angle, RED);
    DrawText(coordinate.primaryLabel, coordinate.primaryPosition.x + 5, coordinate.primaryPosition.y + 5, 10, BLACK);
    DrawText(coordinate.secondaryLabel, coordinate.primaryPosition.x + 5, coordinate.primaryPosition.y + 17, 10, RED);
    DrawText(coordinate.nameLabel, coordinate.primaryPosition.x - 13, coordinate.primaryPosition.y - 13, 10, DARKBLUE);
}

void coordinatePrimaryComponentsDraw(Vector2 point, Color color)
{
    Vector2 xComponent = (Vector2){point.x, 0};
    drawDottedLine(point, xComponent, color);

    Vector2 yComponent = (Vector2){0, point.y};
    drawDottedLine(point, yComponent, color);
}

void coordinateSecondaryComponentsDraw(Vector2 primaryPosition, Vector2 secondaryPosition, float angle, Color color)
{
    Vector2 xComponent = convertToSystemA((Vector2){secondaryPosition.x, 0}, angle);
    drawDottedLine(primaryPosition, xComponent, color);

    Vector2 yComponent = convertToSystemA((Vector2){0, secondaryPosition.y}, angle);
    drawDottedLine(primaryPosition, yComponent, color);
}

void coordinatePrint(Coordinate *coord)
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

void coordinateSecondaryLabelUpdate(Coordinate *coordinate, float angle)
{
    coordinate->secondaryPosition = convertToSystemB(coordinate->primaryPosition, angle);
    snprintf(coordinate->secondaryLabel, 50, "(%.2fm, %.2fft)", coordinate->secondaryPosition.x, -metersToFeet(coordinate->secondaryPosition.y));
}
