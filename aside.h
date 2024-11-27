#ifndef ASIDE_H_
#define ASIDE_H_

#include "raylib.h"

typedef struct aside
{
    Rectangle box;
    bool active;
} Aside;

Aside *asideCreate();
void openAside(Aside *aside);
void closeAside(Aside *aside);
void asideDraw(Aside aside);
void updateAsidePosition(Aside *aside, int screenWidth);

#endif