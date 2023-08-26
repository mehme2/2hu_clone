#ifndef _TEXTURE_H
#define _TEXTURE_H

#include "numdef.h"

typedef union {
    struct {
    u8 r, g, b, a;
    } Channels;
    u32 Value;
} Color;

typedef struct {
    u32 Width, Height;
    Color *Buffer;
} Texture;

Texture LoadBMP(const char *Filename);

#endif
