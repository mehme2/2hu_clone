#include "loadgl.h"
#include "entity.h"
#include "meth.h"
#include <stdio.h>

Entity *AddEntityToList(Entity *List, u32 Size) {
    Entity *Slot = 0;
    for(u32 i = 0; i < Size; i++) {
        if(!List[i].Active) {
            List[i].Active = 1;
            Slot = List + i;
            break;
        }
    }
    return Slot;
}

void UpdateEntityList(Entity *List, u32 Size, float Delta) {
    for(u32 i = 0; i < Size; i++) {
        if(List[i].Active) {
            List[i].Position = AddVec2(List[i].Position, ScaleVec2(List[i].Velocity, Delta));
        }
    }
}

void RenderEntityList(Entity *List, u32 Size, Matrix4 ViewMat) {
    glActiveTexture(GL_TEXTURE0);
    Matrix4 Translate;
    Matrix4 Scale;
    Matrix4 ModelMat;
    Matrix4 TransformMat;
    for(u32 i = 0; i < Size; i++) {
        if(List[i].Active) {
            glBindBuffer(GL_ARRAY_BUFFER, List[i].VertexBufID);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, List[i].IndexBufID);
            glBindTexture(GL_TEXTURE_2D, List[i].TextureID);
            glUniform1i(glGetUniformLocation(List[i].ProgramID, "Texture"), 0);
            TranslationMatrix(List[i].Position.x, List[i].Position.y, 0, &Translate);
            ScalingMatrix(List[i].Size, List[i].Size, 1.0f, &Scale);
            MultiplyMatrix(&Scale, &Translate, &ModelMat);
            MultiplyMatrix(&ViewMat, &ModelMat, &TransformMat);
            glUseProgram(List[i].ProgramID);
            glUniformMatrix4fv(glGetUniformLocation(List[i].ProgramID, "TransformMat"), 1, GL_FALSE, (float *)&TransformMat);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
    }
}
