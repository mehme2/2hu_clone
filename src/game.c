#include "game.h"

#include "loadgl.h"
#include "texture.h"
#include <GL/gl.h>
#include <stdio.h>
#include <math.h>
#include "meth.h"

u32 LoadShader(const char *Path, GLenum Type) {
    u32 ShaderID = 0;
    u64 Size;
    char *File = LoadFile(Path, &Size);
    if(File) {
        int LoadSuccess;
        char Info[512];
        ShaderID = glCreateShader(Type);
        glShaderSource(ShaderID, 1, (const char **)&File, 0);
        glCompileShader(ShaderID);
        FreeFile(File);
        glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &LoadSuccess);
        if(!LoadSuccess) {
            glGetShaderInfoLog(ShaderID, sizeof(Info), 0, Info);
            printf("\nError while compiling \"%s\":\n\n%s\n", Path, Info);
        }
    }
    return ShaderID;
}

typedef struct {
    float px, py;
    Texture Test;
    u32 TextureID;
    u32 VertexBufID;
    u32 IndexBufID;
    u32 VertexShaderID;
    u32 FragShaderID;
    u32 ProgramID;
    u32 VertexArrayID;
    float Timer;
} GameMemory;

GameMemory Memory;

void GameInit() {
    //glViewport(0, 0, 1920, 1080);
    glClearColor(0, 0, 0.2, 0);
    Memory.px = 0;
    Memory.py = 0;
    Memory.Test = LoadBMP("assets/test.bmp");
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &Memory.TextureID);
    glBindTexture(GL_TEXTURE_2D, Memory.TextureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Memory.Test.Width, Memory.Test.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Memory.Test.Buffer);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Memory.TextureID);
    const static float VertexData[] = {
        -0.5f, -0.5f, 0.0f, 0.0f,
         0.5f,  0.5f, 1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 1.0f,
         0.5f, -0.5f, 1.0f, 0.0f,
    };
    const static u32 IndexData[] = {
        0, 1, 2,
        0, 3, 1,
    };
    glGenBuffers(1, &Memory.VertexBufID);
    glGenBuffers(1, &Memory.IndexBufID);
    glBindBuffer(GL_ARRAY_BUFFER, Memory.VertexBufID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData), VertexData, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Memory.IndexBufID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(IndexData), IndexData, GL_STATIC_DRAW);
    Memory.VertexShaderID = LoadShader("../src/shaders/Vertex.glsl", GL_VERTEX_SHADER);
    Memory.FragShaderID = LoadShader("../src/shaders/Fragment.glsl", GL_FRAGMENT_SHADER);
    Memory.ProgramID = glCreateProgram();
    glAttachShader(Memory.ProgramID, Memory.VertexShaderID);
    glAttachShader(Memory.ProgramID, Memory.FragShaderID);
    glLinkProgram(Memory.ProgramID);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glUseProgram(Memory.ProgramID);
}

void GameTick(GameButtons *Input, double Delta) {
    Matrix4 TransformMat;
    Matrix4 Ortho;
    Matrix4 Translate;
    ScalingMatrix(9.0 / 160.0, 0.1, 0.1, &Ortho);
    TranslationMatrix(Memory.px, -Memory.py, 0, &Translate);
    MultiplyMatrix(&Ortho, &Translate, &TransformMat);

    Memory.px += 5 * Delta * (Input->Right - Input->Left);
    Memory.py += 5 * Delta * (Input->Down - Input->Up);

    Memory.Timer += Delta;

    glUniform1i(glGetUniformLocation(Memory.ProgramID, "Texture"), 0);
    glUniformMatrix4fv(glGetUniformLocation(Memory.ProgramID, "TransformMat"), 1, GL_FALSE, (float *)&TransformMat);

    glClear(GL_COLOR_BUFFER_BIT);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
