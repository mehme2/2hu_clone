#include "meth.h"

#include <time.h>
#include <stdlib.h>

void InitRandom() {
    srand(time(NULL));
}

float RandomReal(float min, float max) {
    float r = (float)rand() / RAND_MAX;
    r *= max - min;
    r += min;
    return r;
}

int RandomInt(int min, int max) {
    int r = rand() % (max - min + 1);
    r += min;
    return r;
}

inline Vec2 AddVec2(Vec2 lhs, Vec2 rhs) {
    return Vec2(lhs.x + rhs.x, lhs.y + rhs.y);
}

inline Vec2 SubVec2(Vec2 lhs, Vec2 rhs) {
    return Vec2(lhs.x - rhs.x, lhs.y - rhs.y);
}

inline Vec2 MulVec2(Vec2 lhs, Vec2 rhs) {
    return Vec2(lhs.x * rhs.x, lhs.y * rhs.y);
}

inline Vec2 DivVec2(Vec2 lhs, Vec2 rhs) {
    return Vec2(lhs.x / rhs.x, lhs.y / rhs.y);
}

inline Vec2 ScaleVec2(Vec2 v, float scalar) {
    return Vec2(v.x * scalar, v.y * scalar);
}

inline float DotVec2(Vec2 lhs, Vec2 rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

void IdentityMatrix(Matrix4 *out) {
    const static Matrix4 Identity = {
        {
            {1.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f},
        }
    };
    *out = Identity;
}

void TranslationMatrix(float x, float y, float z, Matrix4 *out) {
    IdentityMatrix(out);
    out->Cells[3][0] = x;
    out->Cells[3][1] = y;
    out->Cells[3][2] = z;
}

void ScalingMatrix(float x, float y, float z, Matrix4 *out) {
    IdentityMatrix(out);
    out->Cells[0][0] = x;
    out->Cells[1][1] = y;
    out->Cells[2][2] = z;
}

void MultiplyMatrix(Matrix4 *lhs, Matrix4 *rhs, Matrix4 *out) {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            out->Cells[i][j] = 0;
            for(int k = 0; k < 4; k++) {
                out->Cells[i][j] += lhs->Cells[k][j] * rhs->Cells[i][k];
            }
        }
    }
}
