#include "utils.h"

float calculateAngle(Vector2 start, Vector2 end)
{
    float deltaX = end.x - start.x;
    float deltaY = end.y - start.y;
    return atan2f(deltaY, deltaX);
}

float radiansToDegrees(float radians)
{
    return radians*(180.f/PI);
}