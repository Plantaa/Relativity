#ifndef COORDINATE_SYSTEM_H_
#define COORDINATE_SYSTEM_H_

#include "raylib.h"
#include "axis.h"

typedef struct coordinate_system
{
    Vector2 origin;
    Axis xAxis;
    Axis yAxis;
    Color axiiColor;
    double angle;
}CoordinateSystem;

void coordinateSystemDraw(CoordinateSystem coordinateSystem);
void coordinateSystemValuesUpdate(CoordinateSystem *coordinateSystem, Camera2D camera);
void coordinateSystemAngleUpdate(CoordinateSystem *coordinateSystem, double angle);

#endif