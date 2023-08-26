#ifndef _GAME_H
#define _GAME_H

#include "gl.h"
#include "numdef.h"
#include "platform.h"

typedef struct {
    int Up, Down, Left, Right, A, B;
} GameButtons;

void GameInit();
void GameTick(GameButtons *Input, double Delta);

#endif
