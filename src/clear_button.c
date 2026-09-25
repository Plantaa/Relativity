#include <stdlib.h>

#include "clear_button.h"

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