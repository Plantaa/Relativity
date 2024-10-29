#include "utils.h"

float calculateAngle(Vector2 start, Vector2 end)
{
    float deltaX = end.x - start.x;
    float deltaY = end.y - start.y;
    return atan2f(deltaY, deltaX);
}

float radiansToDegrees(float radians)
{
    return radians * (180.f / PI);
}

// Function to convert coordinates from System B to System A
Vector2 convertToSystemA(Vector2 point, float angle)
{
    float angleCos = cosf(angle);
    float angleSin = sinf(angle);

    Vector2 pointA = {
        .x = point.x * angleCos - point.y * angleSin,
        .y = point.x * angleSin + point.y * angleCos};

    return pointA;
}

// Function to convert coordinates from System A to System B
Vector2 convertToSystemB(Vector2 point, float angle)
{
    float angleCos = cosf(angle);
    float angleSin = sinf(angle);

    Vector2 pointB = {
        .x = point.x * angleCos + point.y * angleSin,
        .y = -point.x * angleSin + point.y * angleCos};

    return pointB;
}

void drawDottedLine(Vector2 start, Vector2 end, int dotNumber, Color color)
{
    double deltaX = (end.x - start.x);
    double deltaY = (end.y - start.y);

    double dotX = start.x;
    double dotY = start.y;

    double deltaDotX = deltaX / dotNumber;
    double deltaDotY = deltaY / dotNumber;

    for (int i = 0; i < dotNumber; i++)
    {
        DrawPixel(dotX, dotY, color);
        dotX += deltaDotX;
        dotY += deltaDotY;
    }
}