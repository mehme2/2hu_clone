#ifndef _TEXTURE_H
#define _TEXTURE_H

#include "numdef.h"

typedef union {
    struct {
    u8 r, g, b, a;
    };
    u32 rgba;
} color;

typedef struct {
    u32 width, height;
    color *buffer;
} texture;

texture loadBMP(const char *path);

#endif
