#ifndef _METH_H
#define _METH_H

#include "numdef.h"

void InitRandom();
float RandomReal(float min, float max);
int RandomInt(int min, int max);

typedef struct {
    float x, y;
} Vec2;

#define Vec2(x, y) (Vec2){x, y}

extern Vec2 AddVec2(Vec2 lhs, Vec2 rhs);
extern Vec2 SubVec2(Vec2 lhs, Vec2 rhs);
extern Vec2 MulVec2(Vec2 lhs, Vec2 rhs);
extern Vec2 DivVec2(Vec2 lhs, Vec2 rhs);
extern Vec2 ScaleVec2(Vec2 v, float scalar);
extern float DotVec2(Vec2 lhs, Vec2 rhs);

typedef struct {
    float Cells[4][4];
} Matrix4;

void IdentityMatrix(Matrix4 *out);
void TranslationMatrix(float x, float y, float z, Matrix4 *out);
void ScalingMatrix(float x, float y, float z, Matrix4 *out);
void MultiplyMatrix(Matrix4 *lhs, Matrix4 *rhs, Matrix4 *out);

#endif
