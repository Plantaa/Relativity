#include <stdlib.h>
#include <math.h>

#include "coordinate_system.h"

CoordinateSystem *coordinateSystemCreate()
{
    return calloc(1, sizeof(CoordinateSystem));
}

void coordinateSystemFill(CoordinateSystem *coordinateSystem, Vector2 origin, Axis xAxis, Axis yAxis, double angle, Color axiiColor)
{
    coordinateSystem->origin = origin;
    coordinateSystem->xAxis = xAxis;
    coordinateSystem->yAxis = yAxis;
    coordinateSystem->angle = angle;
    coordinateSystem->axiiColor = axiiColor;
}

void coordinateSystemDraw(CoordinateSystem coordinateSystem)
{
    DrawLineEx(coordinateSystem.xAxis.begin, coordinateSystem.xAxis.end, 1.4f, coordinateSystem.axiiColor);
    DrawLineEx(coordinateSystem.yAxis.begin, coordinateSystem.yAxis.end, 1.4f, coordinateSystem.axiiColor);
}

void coordinateSystemValuesUpdate(CoordinateSystem *coordinateSystem, Camera2D camera)
{
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    coordinateSystem->xAxis = (Axis){
        .begin = {
            .x = camera.target.x - screenWidth,
            .y = 0},
        .end = {
            .x = camera.target.x + screenWidth,
            .y = 0}};

    coordinateSystem->yAxis = (Axis){
        .begin = {
            .x = 0,
            .y = camera.target.y - screenHeight},
        .end = {
            .x = 0,
            .y = camera.target.y + screenHeight}};
}

void coordinateSystemAngleUpdate(CoordinateSystem *coordinateSystem, double angle)
{
    coordinateSystem->angle = angle;
    double angleSin = sin(angle);
    double angleCos = cos(angle);

    double axiiLength = GetScreenWidth() * GetScreenHeight();

    coordinateSystem->xAxis = (Axis){
        .begin = {
            .x = angleCos * axiiLength,
            .y = angleSin * axiiLength,
        },
        .end = {
            .x = -angleCos * axiiLength,
            .y = -angleSin * axiiLength,
        },
    };
    coordinateSystem->yAxis = (Axis){
        .begin = {
            .x = angleSin * axiiLength,
            .y = -angleCos * axiiLength,
        },
        .end = {
            .x = -angleSin * axiiLength,
            .y = angleCos * axiiLength,
        },
    };
}
