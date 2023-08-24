#include <GL/gl.h>
#include <unistd.h>
#include <stdio.h>

#include "game.h"
#include "numdef.h"

typedef union {
    struct {
    u8 r, g, b, a;
    } Channels;
    u32 Value;
} Color;

typedef struct {
    u32 Width, Height;
    Color *Buffer;
} Texture;

Texture LoadBMP(const char *Filename) {
    Texture Return = {0};
    u64 FileSize;
    void *File = LoadFile(Filename, &FileSize);
    if(File) {
        u8 *PixelArray = (u8 *)File + *(u32 *)((u8 *)File + 10);
        int Width = *(int *)((u8 *)File + 18);
        int Height = *(int *)((u8 *)File + 22);
        Return.Width = Width < 0 ? -Width : Width;
        Return.Height = Height < 0 ? -Height : Height;
        Return.Buffer = AllocateMemory(sizeof(Color) * Return.Width * Return.Height);
        Color *Current = Return.Buffer + Return.Width * (Return.Height - 1);
        int Padding = 4 - (3 * Return.Width) % 4;
        for(int y = 0; y < Return.Height; y++) {
            for(int x = 0; x < Return.Width; x++) {
                Current->Channels.b = *PixelArray++;
                Current->Channels.g = *PixelArray++;
                Current->Channels.r = *PixelArray++;
                Current++;
            }
            PixelArray += Padding;
            Current -= Return.Width * 2;
        }
        FreeFile(File);
    }
    return Return;
}

typedef struct {
    float px, py;
    Texture test;
    u32 texture;
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
    Memory.test = LoadBMP("assets/test.bmp");
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &Memory.texture);
    glBindTexture(GL_TEXTURE_2D, Memory.texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Memory.test.Width, Memory.test.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Memory.test.Buffer);
    glActiveTexture(Memory.texture);
}

void GameTick(GameButtons *Input, double Delta) {
    Memory.px += 5 * Delta * (Input->Right - Input->Left);
    Memory.py += 5 * Delta * (Input->Down - Input->Up);

    glLoadIdentity();
    glTranslatef(Memory.px, Memory.py, 0);

    glClear(GL_COLOR_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D, Memory.texture);

    glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
        glVertex2f(-0.5f, -0.5f);
        glTexCoord2f(1, 0);
        glVertex2f(0.5f, -0.5f);
        glTexCoord2f(1, 1);
        glVertex2f(0.5f, 0.5f);
        glTexCoord2f(0, 1);
        glVertex2f(-0.5f, 0.5f);
    glEnd();
}
