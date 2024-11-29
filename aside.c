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

void asideFill(Aside aside) 
{
    int gap = 10;
    int textHeight = aside.box.y + (gap);
    int justifyLeft = aside.box.x + 3;
    int fontSizeTitle = 10;
    int fontSizeDescription = 5;

    DrawText("[F1]", justifyLeft, textHeight, fontSizeTitle, BLACK);
    DrawText("Abrir/Fechar este menu de ajuda", justifyLeft, textHeight + gap, fontSizeDescription, GRAY);
    textHeight += 30;

    DrawText("[Botão direito + Mouse]", justifyLeft, textHeight, fontSizeTitle, BLACK);
    DrawText("Aperte e segure para mover a tela", justifyLeft, textHeight + gap, fontSizeDescription, GRAY);
    textHeight += 30;

    DrawText("[Botão Esquerdo + Mouse]", justifyLeft, textHeight, fontSizeTitle, BLACK);
    DrawText("Segure para posicionar uma coordenada e solte para criar", justifyLeft, textHeight + gap, fontSizeDescription, GRAY);
    textHeight += 30;

    DrawText("[R]", justifyLeft, textHeight, fontSizeTitle, BLACK);
    DrawText("Defini o ângulo do 2 eixo", justifyLeft, textHeight + gap, fontSizeDescription, GRAY);
    textHeight += 30;

    DrawText("[Del]", justifyLeft, textHeight, fontSizeTitle, BLACK);
    DrawText("Remove uma coordenada selecionada", justifyLeft, textHeight + gap, fontSizeDescription, GRAY);
}


void asideDraw(Aside aside)
{
    DrawRectangleRec(aside.box, WHITE);
    asideFill(aside);
}

void asidePositionUpdate(Aside *aside, int screenWidth)
{
    aside->box.x = screenWidth - aside->box.width;
}

