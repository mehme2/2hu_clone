#include <math.h>
#include <stdio.h>

#include "game.h"
#include "loadgl.h"
#include "platform.h"
#include "texture.h"
#include "meth.h"
#include "entity.h"

#define EM_PLAYER 0x01
#define EM_ENEMY 0x02

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

u32 loadGLTexture(const char *path) {
    u32 texName;
    texture tex = loadTextureFile(path);
    glGenTextures(1, &texName);
    glBindTexture(GL_TEXTURE_2D, texName);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.width, tex.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.buffer);
    freeMem(tex.buffer);
    return texName;
}

typedef enum {
    TEXTURE_TEST,
    TEXTURE_PLAYER,
    TEXTURE_PLAYER_BULLET,
    TEXTURE_BOSS,
    TEXTURE_ENEMY_BULLET,
    TEXTURE_COUNT,
} textureID;

typedef struct {
    entity entitites[512];
    u32 texNames[TEXTURE_COUNT];
    u32 vertexID;
    u32 indexID;
    u32 vertShaderID;
    u32 fragShaderID;
    u32 progID;
    float timer;
    float shootTimer;
    float bossShootTimer;
    vec2 angle;
    vec2 angularSpeed;
    vec2 angularAcc;
} gameMemory;

gameMemory memory;

void gameInit() {
    glClearColor(0, 0.1, 0.3, 0);
    const char *texturePaths[TEXTURE_COUNT] = {
        "assets/test.png",
        "assets/player.png",
        "assets/player_bullet.png",
        "assets/boss.png",
        "assets/enemy_bullet.png",
    };
    for(int i = 0; i < TEXTURE_COUNT; i++) {
        memory.texNames[i] = loadGLTexture(texturePaths[i]);
    }
    const float vertexData[] = {
        -0.5f, -0.5f, 0.0f, 1.0f,
         0.5f,  0.5f, 1.0f, 0.0f,
        -0.5f,  0.5f, 0.0f, 0.0f,
         0.5f, -0.5f, 1.0f, 1.0f,
    };
    const u32 indexData[] = {
        0, 1, 2,
        0, 3, 1,
    };
    glGenBuffers(1, &memory.vertexID);
    glGenBuffers(1, &memory.indexID);
    glBindBuffer(GL_ARRAY_BUFFER, memory.vertexID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, memory.indexID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW);
    memory.vertShaderID = loadShader("../assets/shaders/vertex.glsl", GL_VERTEX_SHADER);
    memory.fragShaderID = loadShader("../assets/shaders/fragment.glsl", GL_FRAGMENT_SHADER);
    memory.progID = glCreateProgram();
    glAttachShader(memory.progID, memory.vertShaderID);
    glAttachShader(memory.progID, memory.fragShaderID);
    glLinkProgram(memory.progID);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glUseProgram(memory.progID);
    entity *player = addEntityToList(memory.entitites, sizeof(memory.entitites) / sizeof(entity));
    player->progID = memory.progID;
    player->vertexID = memory.vertexID;
    player->indexID = memory.indexID;
    player->texID = memory.texNames[TEXTURE_PLAYER];
    player->pos = vec2(0, 0);
    player->vel = vec2(0, 0);
    player->scale = vec2(1, 1);
    player->mask = EM_PLAYER;
    player->collideRad = 0.1;
    player->hp = 15;
    entity *boss = addEntityToList(memory.entitites, sizeof(memory.entitites) / sizeof(entity));
    boss->progID = memory.progID;
    boss->vertexID = memory.vertexID;
    boss->indexID = memory.indexID;
    boss->texID = memory.texNames[TEXTURE_BOSS];
    boss->pos = vec2(0, 7);
    boss->vel = vec2(0, 0);
    boss->scale = vec2(1, 1);
    boss->mask = EM_ENEMY;
    boss->collideRad = 0.3;
    boss->hp = 1000;
    initRandom();
    memory.angle = vec2(0.0f, 1.0f);
    memory.angularSpeed = vec2(cos(45), sin(-45));
    memory.angularAcc = getDirVec2(0.008f * 0.012f * PI_F);
}

void gameTick(gameButtons *input, double delta) {
    mat4 viewMat;
    scalingMat(9.0 / 160.0, 0.1, 1.0, &viewMat);

    memory.entitites[0].pos.x += 5 * delta * (input->right - input->left);
    memory.entitites[0].pos.y -= 5 * delta * (input->down - input->up);

    memory.timer += delta;
    memory.shootTimer += delta;
    memory.bossShootTimer += delta;

    if(0 && memory.timer > 1.0f) {
        memory.timer = 0;
        entity *new = addEntityToList(memory.entitites + 2, sizeof(memory.entitites) / sizeof(entity) - 2);
        if(new) {
            new->progID = memory.progID;
            new->vertexID = memory.vertexID;
            new->indexID = memory.indexID;
            new->texID = memory.texNames[TEXTURE_TEST];
            new->pos = vec2(randomReal(-10, 10), randomReal(-10, 10));
            new->vel = vec2(randomReal(-1, 1), randomReal(-1, 1));
            new->scale = vec2(1.0, 1.0);
            new->mask = EM_ENEMY;
            new->collideRad = 0.3;
            new->hp = 5;
        }
    }
    if(input->a && memory.shootTimer > 0.1f) {
        memory.shootTimer = 0;
        entity *new = addEntityToList(memory.entitites, sizeof(memory.entitites) / sizeof(entity));
        if(new) {
            new->progID = memory.progID;
            new->vertexID = memory.vertexID;
            new->indexID = memory.indexID;
            new->texID = memory.texNames[TEXTURE_PLAYER_BULLET];
            new->pos = addVec2(memory.entitites[0].pos, vec2(0, 1));
            new->vel = vec2(0, 10);
            new->scale = vec2(0.5, 0.5);
            new->collideMask = EM_ENEMY;
            new->collideRad = 0.3;
            new->damage = 1;
            new->hp = 1;
            new->collideEvent = CE_DEACTIVATE | CE_DEAL_DAMAGE;
        }
    }
    if(memory.entitites[1].active && memory.bossShootTimer > 0.008f) {
        memory.bossShootTimer -= 0.008f;
        memory.angularSpeed = mulCompVec2(memory.angularSpeed, memory.angularAcc);
        memory.angle = mulCompVec2(memory.angularSpeed, memory.angle);
        memory.angle = scaleVec2(memory.angle, 1.0f / lenVec2(memory.angle));
        entity *new = addEntityToList(memory.entitites + 2, sizeof(memory.entitites) / sizeof(entity) - 2);
        if(new) {
            new->progID = memory.progID;
            new->vertexID = memory.vertexID;
            new->indexID = memory.indexID;
            new->texID = memory.texNames[TEXTURE_ENEMY_BULLET];
            //new->pos = addVec2(memory.entitites[1].pos, scaleVec2(memory.angle, 1));
            new->pos = memory.entitites[1].pos;
            new->vel = scaleVec2(memory.angle, 7);
            new->scale = vec2(0.3, 0.3);
            new->collideMask = EM_PLAYER;
            new->collideRad = 0.3;
            new->damage = 5;
            new->hp = 1;
            new->collideEvent = CE_DEACTIVATE | CE_DEAL_DAMAGE;
        }
    }

    updateEntityList(memory.entitites, sizeof(memory.entitites) / sizeof(entity), delta);

    glClear(GL_COLOR_BUFFER_BIT);
    renderEntityList(memory.entitites, sizeof(memory.entitites) / sizeof(entity), viewMat);
}
