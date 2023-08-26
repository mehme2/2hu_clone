#ifndef _PLATFORM_H
#define _PLATFORM_H

extern void *LoadFile(const char *Filename, unsigned long *Size);
extern void FreeFile(void *Pointer);
extern void *AllocateMemory(long Size);
extern void FreeMemory(void *Pointer);
extern void ResizeWindow(int width, int height);
void SetWindowTitle(const char *Title);

#endif
