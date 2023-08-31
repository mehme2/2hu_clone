#include "loadgl.h"
#include "entity.h"
#include "meth.h"
#include <stdio.h>

entity *addEntityToList(entity *list, u32 size) {
    entity *slot = 0;
    for(u32 i = 0; i < size; i++) {
        if(!list[i].active) {
            list[i].active = 1;
            slot = list + i;
            break;
        }
    }
    return slot;
}

void updateEntityList(entity *list, u32 size, float delta) {
    for(u32 i = 0; i < size; i++) {
        if(list[i].active) {
            list[i].pos = addVec2(list[i].pos, scaleVec2(list[i].vel, delta));
            if(lenSqrVec2(list[i].pos) > 100) {
                list[i].active = 0;
            }
        }
    }
}

void renderEntityList(entity *list, u32 size, mat4 viewMat) {
    glActiveTexture(GL_TEXTURE0);
    mat4 translate;
    mat4 scale;
    mat4 modelMat;
    mat4 transformMat;
    for(u32 i = 0; i < size; i++) {
        if(list[i].active) {
            glBindBuffer(GL_ARRAY_BUFFER, list[i].vertexID);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, list[i].indexID);
            glBindTexture(GL_TEXTURE_2D, list[i].texID);
            glUniform1i(glGetUniformLocation(list[i].progID, "tex"), 0);
            translationMat(list[i].pos.x, list[i].pos.y, 0, &translate);
            scalingMat(list[i].scale.x, list[i].scale.y, 1.0f, &scale);
            mulMat(&translate, &scale, &modelMat);
            mulMat(&viewMat, &modelMat, &transformMat);
            glUseProgram(list[i].progID);
            glUniformMatrix4fv(glGetUniformLocation(list[i].progID, "transformMat"), 1, GL_FALSE, (float *)&transformMat);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
    }
}
