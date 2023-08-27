#ifndef _PLATFORM_H
#define _PLATFORM_H

#include "numdef.h"

extern void *LoadFile(const char *Filename, u64 *Size);
extern void FreeFile(void *Pointer);
extern void *AllocateMemory(u64 Size);
extern void FreeMemory(void *Pointer);
extern void ResizeWindow(int width, int height);
void SetWindowTitle(const char *Title);

#endif
