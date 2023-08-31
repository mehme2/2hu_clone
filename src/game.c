#include <stdio.h>

#include "game.h"
#include "loadgl.h"
#include "texture.h"
#include "meth.h"
#include "entity.h"

u32 loadShader(const char *path, GLenum type) {
    u32 shaderID = 0;
    u64 size;
    char *file = loadFile(path, &size);
    if(file) {
        int success;
        char Info[512];
        shaderID = glCreateShader(type);
        glShaderSource(shaderID, 1, (const char **)&file, 0);
        glCompileShader(shaderID);
        freeFile(file);
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(shaderID, sizeof(Info), 0, Info);
            printf("\nError while compiling \"%s\":\n\n%s\n", path, Info);
        }
    }
    return shaderID;
}

typedef struct {
    entity Entities[32];
    texture test;
    u32 texID;
    u32 vertexID;
    u32 indexID;
    u32 vertShaderID;
    u32 fragShaderID;
    u32 progID;
    float timer;
} gameMemory;

gameMemory memory;

void gameInit() {
    //glViewport(0, 0, 1920, 1080);
    glClearColor(0, 0, 0.2, 0);
    memory.test = loadBMP("assets/test.bmp");
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &memory.texID);
    glBindTexture(GL_TEXTURE_2D, memory.texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, memory.test.width, memory.test.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, memory.test.buffer);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, memory.texID);
    const static float vertexData[] = {
        -0.5f, -0.5f, 0.0f, 0.0f,
         0.5f,  0.5f, 1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 1.0f,
         0.5f, -0.5f, 1.0f, 0.0f,
    };
    const static u32 indexData[] = {
        0, 1, 2,
        0, 3, 1,
    };
    glGenBuffers(1, &memory.vertexID);
    glGenBuffers(1, &memory.indexID);
    glBindBuffer(GL_ARRAY_BUFFER, memory.vertexID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, memory.indexID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW);
    memory.vertShaderID = loadShader("../src/shaders/vertex.glsl", GL_VERTEX_SHADER);
    memory.fragShaderID = loadShader("../src/shaders/fragment.glsl", GL_FRAGMENT_SHADER);
    memory.progID = glCreateProgram();
    glAttachShader(memory.progID, memory.vertShaderID);
    glAttachShader(memory.progID, memory.fragShaderID);
    glLinkProgram(memory.progID);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glUseProgram(memory.progID);
    entity *player = addEntityToList(memory.Entities, sizeof(memory.Entities) / sizeof(entity));
    player->progID = memory.progID;
    player->vertexID = memory.vertexID;
    player->indexID = memory.indexID;
    player->texID = memory.texID;
    player->pos = vec2(0, 0);
    player->vel = vec2(0, 0);
    player->scale = vec2(1, 1);
    initRandom();
}

void gameTick(gameButtons *Input, double Delta) {
    mat4 viewMat;
    scalingMat(9.0 / 160.0, 0.1, 1.0, &viewMat);

    memory.Entities[0].pos.x += 5 * Delta * (Input->right - Input->left);
    memory.Entities[0].pos.y -= 5 * Delta * (Input->down - Input->up);

    memory.timer += Delta;
    if(memory.timer > 1.0f) {
        memory.timer = 0;
        entity *New = addEntityToList(memory.Entities, sizeof(memory.Entities) / sizeof(entity));
        if(New) {
            New->progID = memory.progID;
            New->vertexID = memory.vertexID;
            New->indexID = memory.indexID;
            New->texID = memory.texID;
            New->pos = vec2(randomReal(-10, 10), randomReal(-10, 10));
            New->vel = vec2(randomReal(-1, 1), randomReal(-1, 1));
            New->scale = vec2(randomReal(0.1, 5), randomReal(0.1, 5));
        }
    }

    updateEntityList(memory.Entities, sizeof(memory.Entities) / sizeof(entity), Delta);

    glClear(GL_COLOR_BUFFER_BIT);
    renderEntityList(memory.Entities, sizeof(memory.Entities) / sizeof(entity), viewMat);
}
