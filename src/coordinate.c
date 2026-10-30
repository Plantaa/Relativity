#include <stdio.h>
#include <stdlib.h>

#include "coordinate.h"
#include "utils.h"

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
}

/* Sizes are divided by zoom so points and labels keep the same size on screen */
static void coordinateTextDraw(const char *text, Vector2 position, float xOffset, float yOffset, float zoom, Color color)
{
    float fontSize = 10.f / zoom;
    Vector2 textPosition = {position.x + xOffset / zoom, position.y + yOffset / zoom};
    DrawTextEx(GetFontDefault(), text, textPosition, fontSize, 1.f / zoom, color);
}

void coordinateDraw(Coordinate coordinate, float angle, bool labels, float zoom)
{
    DrawCircleV(coordinate.primaryPosition, coordinate.radius / zoom, coordinate.color);
    coordinatePrimaryComponentsDraw(coordinate.primaryPosition, BLACK, zoom);
    coordinateSecondaryComponentsDraw(coordinate.primaryPosition, coordinate.secondaryPosition, angle, RED, zoom);
    coordinateTextDraw(coordinate.nameLabel, coordinate.primaryPosition, -13, -13, zoom, DARKBLUE);
    if (!labels)
    {
        return;
    }
    coordinateTextDraw(coordinate.primaryLabel, coordinate.primaryPosition, 5, 5, zoom, BLACK);
    coordinateTextDraw(coordinate.secondaryLabel, coordinate.primaryPosition, 5, 17, zoom, RED);
}

void coordinatePrimaryComponentsDraw(Vector2 point, Color color, float zoom)
{
    Vector2 xComponent = (Vector2){point.x, 0};
    drawDottedLine(point, xComponent, color, zoom);

    Vector2 yComponent = (Vector2){0, point.y};
    drawDottedLine(point, yComponent, color, zoom);
}

void coordinateSecondaryComponentsDraw(Vector2 primaryPosition, Vector2 secondaryPosition, float angle, Color color, float zoom)
{
    Vector2 xComponent = convertToSystemA((Vector2){secondaryPosition.x, 0}, angle);
    drawDottedLine(primaryPosition, xComponent, color, zoom);

    Vector2 yComponent = convertToSystemA((Vector2){0, secondaryPosition.y}, angle);
    drawDottedLine(primaryPosition, yComponent, color, zoom);
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

void coordinateLabelUpdate(Coordinate * const coordinate, float angle)
{
    snprintf(coordinate->primaryLabel, 50, "(%.2fm, %.2fft)", coordinate->primaryPosition.x, -metersToFeet(coordinate->primaryPosition.y));
    coordinate->secondaryPosition = convertToSystemB(coordinate->primaryPosition, angle);
    snprintf(coordinate->secondaryLabel, 50, "(%.2fm, %.2fft)", coordinate->secondaryPosition.x, -metersToFeet(coordinate->secondaryPosition.y));
}
