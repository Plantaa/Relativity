#include <stdlib.h>
#include <math.h>

#include "coordinate_system.h"

void coordinateSystemDraw(CoordinateSystem coordinateSystem, float zoom)
{
    float thickness = 1.4f / zoom;
    DrawLineEx(coordinateSystem.xAxis.begin, coordinateSystem.xAxis.end, thickness, coordinateSystem.axiiColor);
    DrawLineEx(coordinateSystem.yAxis.begin, coordinateSystem.yAxis.end, thickness, coordinateSystem.axiiColor);
}

void coordinateSystemValuesUpdate(CoordinateSystem *coordinateSystem, Camera2D camera)
{
    /* Visible world area grows as the camera zooms out */
    float screenWidth = GetScreenWidth() / camera.zoom;
    float screenHeight = GetScreenHeight() / camera.zoom;

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
