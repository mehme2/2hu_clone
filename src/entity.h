#ifndef _ENTITY_H
#define _ENTITY_H

#include "meth.h"

#define CE_DEAL_DAMAGE 0x01
#define CE_DEACTIVATE 0x02

typedef struct {
    u8 active;
    u8 mask;
    u8 collideMask;
    u8 collideEvent;
    vec2 pos;
    vec2 vel;
    vec2 scale;
    float collideRad;
    int hp;
    int damage;
    u32 texID;
    u32 indexID;
    u32 vertexID;
    u32 progID;
} entity;

entity *addEntityToList(entity *list, u32 size);
void updateEntityList(entity *list, u32 size, float delta);
void renderEntityList(entity *list, u32 size, mat4 viewMat);

#endif
