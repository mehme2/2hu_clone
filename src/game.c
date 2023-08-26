#include "game.h"
#include "numdef.h"

#include "platform.h"
#include "texture.h"
#include <stdio.h>
#include <math.h>

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
    u32 VertexShaderID;
    u32 FragShaderID;
    u32 ProgramID;
    u32 VertexArrayID;
    float Timer;
} GameMemory;

GameMemory Memory;

void GameInit() {
    glViewport(0, 0, 1920, 1080);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-160.0 / 9, 160.0 / 9, 10, -10, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glClearColor(0, 0, 0, 0);
    Memory.px = 0;
    Memory.py = 0;
    /*
    Memory.Test = LoadBMP("assets/test.bmp");
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &Memory.TextureID);
    glBindTexture(GL_TEXTURE_2D, Memory.TextureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Memory.Test.Width, Memory.Test.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Memory.Test.Buffer);
    */
    const static float VertexData[] = {
        -0.5f, -0.5f,
        0.5f, 0.5f,
        -0.5f, 0.5f,
        -0.5f, -0.5f,
        0.5f, -0.5f,
        0.5f, 0.5f,
    };
    glGenBuffers(1, &Memory.VertexBufID);
    glBindBuffer(GL_ARRAY_BUFFER, Memory.VertexBufID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData), VertexData, GL_STATIC_DRAW);
    Memory.VertexShaderID = LoadShader("../src/shaders/Vertex.glsl", GL_VERTEX_SHADER);
    Memory.FragShaderID = LoadShader("../src/shaders/Fragment.glsl", GL_FRAGMENT_SHADER);
    Memory.ProgramID = glCreateProgram();
    glAttachShader(Memory.ProgramID, Memory.VertexShaderID);
    glAttachShader(Memory.ProgramID, Memory.FragShaderID);
    glLinkProgram(Memory.ProgramID);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glUseProgram(Memory.ProgramID);
}

void GameTick(GameButtons *Input, double Delta) {
    Memory.px += 5 * Delta * (Input->Right - Input->Left);
    Memory.py += 5 * Delta * (Input->Down - Input->Up);

    Memory.Timer += Delta;

    int VertexColorLocation = glGetUniformLocation(Memory.ProgramID, "Color");
    glUniform4f(VertexColorLocation, 0.0f, (sin(Memory.Timer) + 1) / 2, 0.5f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
