#ifndef LEGEND_H_
#define LEGEND_H_

#include "raylib.h"
#include "coordinate.h"

typedef struct legend
{
    Rectangle box;
    bool active;
} Legend;

void legendDraw(Legend legend, const Coordinate* const coordinate, Font font);
void legendPositionUpdate(Legend *legend, Vector2 screenDimension);

#endif
