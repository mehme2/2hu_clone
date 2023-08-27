#ifndef _ENTITY_H
#define _ENTITY_H

#include "meth.h"

typedef struct {
    u8 Active;
    Vec2 Position;
    Vec2 Velocity;
    float Size;
    u32 TextureID;
    u32 IndexBufID;
    u32 VertexBufID;
    u32 ProgramID;
} Entity;

Entity *AddEntityToList(Entity *List, u32 Size);
void UpdateEntityList(Entity *List, u32 Size, float Delta);
void RenderEntityList(Entity *List, u32 Size, Matrix4 View);

#endif
