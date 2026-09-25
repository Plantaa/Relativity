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

void clearButtonDraw(ClearButton button, Font font);
void clearButtonPositionUpdate(ClearButton *button, int screenHeight);

#endif