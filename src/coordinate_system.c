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

void coordinateSystemValuesUpdate(CoordinateSystem *coordinateSystem, Vector2 screenDimensions, Camera2D camera)
{
    coordinateSystem->xAxis = (Axis){
        .begin = { camera.target.x - screenDimensions.x, 0 },
        .end = { camera.target.x + screenDimensions.x, 0 }};
    coordinateSystem->yAxis = (Axis){
        .begin = { 0, camera.target.y - screenDimensions.y },
        .end = { 0, camera.target.y + screenDimensions.y }};
}

void coordinateSystemAngleUpdate(CoordinateSystem *coordinateSystem, Vector2 screenDimensions, double angle)
{
    coordinateSystem->angle = angle;
    double angleSin = sin(angle);
    double angleCos = cos(angle);

    double axiiLength = screenDimensions.x * screenDimensions.y;

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
