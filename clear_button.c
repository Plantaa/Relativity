#include <stdlib.h>

#include "clear_button.h"

ClearButton *clearButtonCreate()
{
    return (ClearButton*)malloc(sizeof(ClearButton));
}

void clearButtonFill(ClearButton *button, Rectangle box, int xTextPadding, int yTextPadding, int fontSize)
{
    button->box = box;
    button->xTextPadding = xTextPadding;
    button->yTextPadding = yTextPadding;
    button->fontSize = fontSize;
}

void clearButtonDraw(ClearButton button)
{
    int xTextPosition = button.box.x + button.xTextPadding;
    int yTextPosition = button.box.y + button.yTextPadding;
    DrawRectangleRec(button.box, Fade(GRAY, 0.7f));
    DrawRectangleLinesEx(button.box, 2, RED);
    DrawText("Clear", xTextPosition, yTextPosition, button.fontSize, BLACK);
}

void clearButtonUpdatePostition(ClearButton *button, int screenHeight)
{
    button->box.y = screenHeight - 40;
}