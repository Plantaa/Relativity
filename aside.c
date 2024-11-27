#include <stdlib.h>

#include "aside.h"

Aside *asideCreate()
{
    return (Aside *)malloc(sizeof(Aside));
}

void openAside(Aside *aside)
{
    aside->active = true;
}

void closeAside(Aside *aside)
{
    aside->active = false;
}

void asideDraw(Aside aside)
{
    DrawRectangleRec(aside.box, Fade(BLACK, 0.5f));
    DrawRectangleRec(aside.box, Fade(WHITE, 0.7f));
}

void updateAsidePosition(Aside *aside, int screenWidth)
{
    aside->box.x = screenWidth - aside->box.width;
}
