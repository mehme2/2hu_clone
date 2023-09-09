#include "meth.h"

#include <math.h>
#include <time.h>
#include <stdlib.h>

void initRandom() {
    srand(time(NULL));
}

float randomReal(float min, float max) {
    float r = (float)rand() / RAND_MAX;
    r *= max - min;
    r += min;
    return r;
}

int randomInt(int min, int max) {
    int r = rand() % (max - min + 1);
    r += min;
    return r;
}

inline vec2 addVec2(vec2 lhs, vec2 rhs) {
    return vec2(lhs.x + rhs.x, lhs.y + rhs.y);
}

inline vec2 subVec2(vec2 lhs, vec2 rhs) {
    return vec2(lhs.x - rhs.x, lhs.y - rhs.y);
}

inline vec2 mulVec2(vec2 lhs, vec2 rhs) {
    return vec2(lhs.x * rhs.x, lhs.y * rhs.y);
}

inline vec2 divVec2(vec2 lhs, vec2 rhs) {
    return vec2(lhs.x / rhs.x, lhs.y / rhs.y);
}

inline vec2 scaleVec2(vec2 v, float scalar) {
    return vec2(v.x * scalar, v.y * scalar);
}

inline float dotVec2(vec2 lhs, vec2 rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

inline float lenSqrVec2(vec2 v) {
    return v.x * v.x + v.y * v.y;
}

inline float lenVec2(vec2 v) {
    return sqrt(lenSqrVec2(v));
}

inline vec2 mulCompVec2(vec2 lhs, vec2 rhs) {
    return vec2(lhs.x * rhs.x - lhs.y * rhs.y, lhs.x * rhs.y + lhs.y * rhs.x);
}

inline vec2 getDirVec2(float angle) {
    return vec2(cosf(angle), sinf(angle));
}

void identityMat(mat4 *out) {
    const static mat4 identity = {
        {
            {1.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f},
        }
    };
    *out = identity;
}

void translationMat(float x, float y, float z, mat4 *out) {
    identityMat(out);
    out->cells[3][0] = x;
    out->cells[3][1] = y;
    out->cells[3][2] = z;
}

void scalingMat(float x, float y, float z, mat4 *out) {
    identityMat(out);
    out->cells[0][0] = x;
    out->cells[1][1] = y;
    out->cells[2][2] = z;
}

void mulMat(mat4 *lhs, mat4 *rhs, mat4 *out) {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            out->cells[i][j] = 0;
            for(int k = 0; k < 4; k++) {
                out->cells[i][j] += lhs->cells[k][j] * rhs->cells[i][k];
            }
        }
    }
}
