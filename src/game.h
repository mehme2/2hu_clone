#ifndef _GAME_H
#define _GAME_H

#include "numdef.h"

typedef struct {
    int Up, Down, Left, Right, A, B;
} GameButtons;

void *LoadFile(const char *Filename, u64 *Size);
void FreeFile(void *Pointer);
void *AllocateMemory(long Size);
void FreeMemory(void *Pointer);
void ResizeWindow(int width, int height);

void GameInit();
void GameTick(GameButtons *Input, double Delta);

#endif
