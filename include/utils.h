#ifndef UTILS_H_
#define UTILS_H_

#include "raylib.h"
#include "math.h"

float calculateAngle(Vector2 start, Vector2 end);
float radiansToDegrees(float radians);
Vector2 convertToSystemA(Vector2 point, float angle);
Vector2 convertToSystemB(Vector2 point, float angle);
void drawDottedLine(Vector2 start, Vector2 end, Color color);
double metersToFeet(double meters);

#endif