#ifndef ASIDE_H_
#define ASIDE_H_

#include "raylib.h"

typedef struct aside
{
    Rectangle box;
    bool active;
} Aside;

Aside *asideCreate();
void asideDraw(Aside aside, Font font);
void asidePositionUpdate(Aside *aside, int screenWidth);
void asideFill(Aside aside, Font font);

#endif