#ifndef _PLATFORM_H
#define _PLATFORM_H

#include "numdef.h"

extern void *loadFile(const char *filename, u64 *size);
extern void freeFile(void *pointer);
extern void *allocMem(u64 size);
extern void *reAllocMem(void *ptr, u64 size);
extern void freeMem(void *ptr);
extern void resizeWin(int width, int height);
void setWinTitle(const char *title);

#endif
