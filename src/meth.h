#ifndef _METH_H
#define _METH_H

#include "numdef.h"

void initRandom();
float randomReal(float min, float max);
int randomInt(int min, int max);

typedef struct {
    float x, y;
} vec2;

#define vec2(x, y) (vec2){x, y}

extern vec2 addVec2(vec2 lhs, vec2 rhs);
extern vec2 subVec2(vec2 lhs, vec2 rhs);
extern vec2 mulVec2(vec2 lhs, vec2 rhs);
extern vec2 divVec2(vec2 lhs, vec2 rhs);
extern vec2 scaleVec2(vec2 v, float scalar);
extern float dotVec2(vec2 lhs, vec2 rhs);
extern float lenSqrVec2(vec2 v);
extern float lenVec2(vec2 v);

typedef struct {
    float cells[4][4];
} mat4;

void identityMat(mat4 *out);
void translationMat(float x, float y, float z, mat4 *out);
void scalingMat(float x, float y, float z, mat4 *out);
void mulMat(mat4 *lhs, mat4 *rhs, mat4 *out);

#endif
