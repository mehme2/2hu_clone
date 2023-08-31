#ifndef _GAME_H
#define _GAME_H

#include "loadgl.h"
#include "numdef.h"
#include "platform.h"

typedef struct {
    int up, down, left, right, a, b;
} gameButtons;

void gameInit();
void gameTick(gameButtons *Input, double Delta);

#endif
