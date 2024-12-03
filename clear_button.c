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

void clearButtonDraw(ClearButton button, Font font)
{
    Vector2 textPosition = { .x = button.box.x + button.xTextPadding, .y = button.box.y + button.yTextPadding };
    DrawRectangleRec(button.box, WHITE);
    DrawRectangleLinesEx(button.box, 2, RED);
    DrawTextEx(font, "Limpar", textPosition, button.fontSize, 1 ,BLACK);
}

void clearButtonPositionUpdate(ClearButton *button, int screenHeight)
{
    button->box.y = screenHeight - 40;
}