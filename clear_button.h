#ifndef CLEAR_BUTTON_H_
#define CLEAR_BUTTON_H_

#include "raylib.h"

typedef struct clear_button
{
    Rectangle box;
    int xTextPadding;
    int yTextPadding;
    int fontSize;

} ClearButton;

ClearButton *clearButtonCreate();
void clearButtonFill(ClearButton *button, Rectangle box, int xTextPadding, int yTextPadding, int fontSize);
void clearButtonDraw(ClearButton button);
void clearButtonUpdatePostition(ClearButton *button, int screenHeight);

#endif