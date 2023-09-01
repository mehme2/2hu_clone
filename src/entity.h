#ifndef _ENTITY_H
#define _ENTITY_H

#include "meth.h"

typedef struct {
    u8 active;
    vec2 pos;
    vec2 vel;
    vec2 scale;
    u32 texID;
    u32 indexID;
    u32 vertexID;
    u32 progID;
} entity;

entity *addEntityToList(entity *list, u32 size);
void updateEntityList(entity *list, u32 size, float delta);
void renderEntityList(entity *list, u32 size, mat4 viewMat);

#endif
