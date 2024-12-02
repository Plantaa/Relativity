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
    int titleFontSize = 22;
    int descriptionFontSize = 16;
    int fontSpacing = 1;
    Vector2 titleTextPosition = { .x = justifyLeft, .y = textHeight };
    Vector2 descriptionTextPostition = { .x = justifyLeft, .y = textHeight + gap };

    DrawTextEx(font, "[F1]", titleTextPosition, titleFontSize, fontSpacing, BLACK);
    DrawTextEx(font, "Abrir/Fechar este menu de ajuda", descriptionTextPostition, descriptionFontSize, fontSpacing, GRAY);
    titleTextPosition.y += 46;
    descriptionTextPostition.y += 46;

    DrawTextEx(font, "[Botão direito + Mouse]", titleTextPosition, titleFontSize, fontSpacing, BLACK);
    DrawTextEx(font, "Aperte e segure para mover a tela", descriptionTextPostition, descriptionFontSize, fontSpacing, GRAY);
    titleTextPosition.y += 46;
    descriptionTextPostition.y += 46;

    DrawTextEx(font, "[Botão Esquerdo + Mouse]", titleTextPosition, titleFontSize, fontSpacing, BLACK);
    DrawTextEx(font, "Segure para posicionar uma coordenada, e solte para criar", descriptionTextPostition, descriptionFontSize, fontSpacing, GRAY);
    titleTextPosition.y += 46;
    descriptionTextPostition.y += 46;

    DrawTextEx(font, "[R]", titleTextPosition, titleFontSize, fontSpacing, BLACK);
    DrawTextEx(font, "Define o ângulo do segundo eixo", descriptionTextPostition, descriptionFontSize, fontSpacing, GRAY);
    titleTextPosition.y += 46;
    descriptionTextPostition.y += 46;

    DrawTextEx(font, "[Del]", titleTextPosition, titleFontSize, fontSpacing, BLACK);
    DrawTextEx(font, "Remove uma coordenada selecionada", descriptionTextPostition, descriptionFontSize, fontSpacing, GRAY);
    titleTextPosition.y += 46;
    descriptionTextPostition.y += 46;

    DrawTextEx(font, "[Espaço]", titleTextPosition, titleFontSize, fontSpacing, BLACK);
    DrawTextEx(font, "Recentraliza a câmera", descriptionTextPostition, descriptionFontSize, fontSpacing, GRAY);
    
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

