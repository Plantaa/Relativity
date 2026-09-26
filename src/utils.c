#include "utils.h"

float calculateAngle(Vector2 start, Vector2 end)
{
    float deltaX = end.x - start.x;
    float deltaY = end.y - start.y;
    return atan2f(deltaY, deltaX);
}

float degreesToRadians(float degrees)
{
    return degrees / (180.f / PI);
}

float radiansToDegrees(float radians)
{
    return radians * (180.f / PI);
}

Vector2 convertToSystemA(Vector2 point, float radians)
{
    float angleCos = cosf(radians);
    float angleSin = sinf(radians);

    Vector2 pointA = {
        .x = point.x * angleCos - point.y * angleSin,
        .y = point.x * angleSin + point.y * angleCos};

    return pointA;
}

Vector2 convertToSystemB(Vector2 point, float angle)
{
    float angleCos = cosf(angle);
    float angleSin = sinf(angle);

    Vector2 pointB = {
        .x = point.x * angleCos + point.y * angleSin,
        .y = -point.x * angleSin + point.y * angleCos};

    return pointB;
}

void drawDottedLine(Vector2 start, Vector2 end, Color color)
{
    double deltaX = (end.x - start.x);
    double deltaY = (end.y - start.y);

    double dotX = start.x;
    double dotY = start.y;

    double dotNumber = sqrt(pow(deltaX, 2.L) + pow(deltaY, 2.L))/2;

    double deltaDotX = deltaX / dotNumber;
    double deltaDotY = deltaY / dotNumber;

    for (int i = 0; i < dotNumber; i++)
    {
        DrawPixel(dotX, dotY, color);
        dotX += deltaDotX;
        dotY += deltaDotY;
    }
}

double metersToFeet(double meters)
{
    return meters / 0.3048;
}
