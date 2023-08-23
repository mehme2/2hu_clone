#ifndef _GAME_H
#define _GAME_H

typedef struct {
    int Up, Down, Left, Right, A, B;
} GameButtons;

typedef struct {
    float px, py;
} GameMemory;

void *LoadFile(const char *Filename, int *Size);
void FreeMemory(void *Pointer);
void ResizeWindow(int width, int height);

void GameInit(GameMemory *Memory);
void GameTick(GameMemory *Memory, GameButtons *Input, double Delta);

#endif
