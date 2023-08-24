#ifndef _GAME_H
#define _GAME_H

#include "gl.h"
#include "numdef.h"

typedef struct {
    int Up, Down, Left, Right, A, B;
} GameButtons;

extern void *LoadFile(const char *Filename, u64 *Size);
extern void FreeFile(void *Pointer);
extern void *AllocateMemory(long Size);
extern void FreeMemory(void *Pointer);
extern void ResizeWindow(int width, int height);
void SetWindowTitle(const char *Title);

void GameInit();
void GameTick(GameButtons *Input, double Delta);

#endif
