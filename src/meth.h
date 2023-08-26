#ifndef _METH_H
#define _METH_H

typedef struct {
    float Cells[4][4];
} Matrix4;

void IdentityMatrix(Matrix4 *out);
void TranslationMatrix(float x, float y, float z, Matrix4 *out);
void ScalingMatrix(float x, float y, float z, Matrix4 *out);
void MultiplyMatrix(Matrix4 *lhs, Matrix4 *rhs, Matrix4 *out);

#endif
