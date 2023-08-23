#include <GL/gl.h>

#include "game.h"

void GameInit(GameMemory *Memory) {
    glViewport(0, 0, 1920, 1080);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-160.0 / 9, 160.0 / 9, 10, -10, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glClearColor(0, 0, 0, 0);
    Memory->px = 0;
    Memory->py = 0;
}

void GameTick(GameMemory *Memory, GameButtons *Input, double Delta) {
    Memory->px += 5 * Delta * (Input->Right - Input->Left);
    Memory->py += 5 * Delta * (Input->Down - Input->Up);

    glLoadIdentity();
    glTranslatef(Memory->px, Memory->py, 0);

    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_QUADS);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(0.5f, -0.5f);
        glVertex2f(0.5f, 0.5f);
        glVertex2f(-0.5f, 0.5f);
    glEnd();
}
