#include <stdlib.h>

#include "aside.h"

Aside *asideCreate()
{
    return (Aside *)malloc(sizeof(Aside));
}

void asideOpen(Aside *aside)
{
    aside->active = true;
}

void asideClose(Aside *aside)
{
    aside->active = false;
}

void asideFill(Aside aside, Font font) 
{
    int gap = 24;
    int textHeight = aside.box.y + gap;
    int justifyLeft = aside.box.x + 3;
    int fontSizeTitle = 22;
    int fontSizeDescription = 16;
    Vector2 textPositionTitle = { .x = justifyLeft, .y = textHeight };
    Vector2 textPositionDescription = { .x = justifyLeft, .y = textHeight + gap };

    DrawTextEx(font, "[F1]", textPositionTitle, fontSizeTitle, 2, BLACK);
    DrawTextEx(font, "Abrir/Fechar este menu de ajuda", textPositionDescription, fontSizeDescription, 2, GRAY);
    textPositionTitle.y += 46;
    textPositionDescription.y += 46;

    DrawTextEx(font, "[Botão direito + Mouse]", textPositionTitle, fontSizeTitle, 2, BLACK);
    DrawTextEx(font, "Aperte e segure para mover a tela", textPositionDescription, fontSizeDescription, 2, GRAY);
    textPositionTitle.y += 46;
    textPositionDescription.y += 46;

    DrawTextEx(font, "[Botão Esquerdo + Mouse]", textPositionTitle, fontSizeTitle, 2, BLACK);
    DrawTextEx(font, "Segure para posicionar uma coordenada e solte para criar", textPositionDescription, fontSizeDescription, 2, GRAY);
    textPositionTitle.y += 46;
    textPositionDescription.y += 46;

    DrawTextEx(font, "[R]", textPositionTitle, fontSizeTitle, 2, BLACK);
    DrawTextEx(font, "Defini o ângulo do 2 eixo", textPositionDescription, fontSizeDescription, 2, GRAY);
    textPositionTitle.y += 46;
    textPositionDescription.y += 46;

    DrawTextEx(font, "[Del]", textPositionTitle, fontSizeTitle, 2, BLACK);
    DrawTextEx(font, "Remove uma coordenada selecionada", textPositionDescription, fontSizeDescription, 2, GRAY);
}


void asideDraw(Aside aside, Font font)
{
    DrawRectangleRec(aside.box, WHITE);
    asideFill(aside, font);
}

void asidePositionUpdate(Aside *aside, int screenWidth)
{
    aside->box.x = screenWidth - aside->box.width;
}

