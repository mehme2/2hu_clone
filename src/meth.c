#include "meth.h"
#include "numdef.h"

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
