#ifndef COORDINATE_SYSTEM_H_
#define COORDINATE_SYSTEM_H_

#include "raylib.h"
#include "axis.h"
#include "triangle.h"

typedef struct coordinate_system
{
    Vector2 origin;
    Axis xAxis;
    Axis yAxis;
    Color axiiColor;
    double angle;
}CoordinateSystem;

void coordinateSystemDraw(CoordinateSystem coordinateSystem);
void coordinateSystemValuesUpdate(CoordinateSystem *coordinateSystem, Vector2 screenDimensions, Camera2D camera);
void coordinateSystemAngleUpdate(CoordinateSystem *coordinateSystem, Vector2 screenDimensions, double angle);

#endif