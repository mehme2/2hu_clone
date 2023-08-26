#include "loadgl.h"

#ifndef __linux__
#define LOADFUNC(name) name = (void *)wglGetProcAddress(#name)
#else
#define LOADFUNC(name) name = (void *)glXGetProcAddress((unsigned char *)#name)
#endif

PFNGLCREATESHADERPROC glCreateShader;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLGETSHADERIVPROC glGetShaderiv;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
PFNGLGENBUFFERSPROC glGenBuffers;
PFNGLBINDBUFFERPROC glBindBuffer;
PFNGLBUFFERDATAPROC glBufferData;
PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLATTACHSHADERPROC glAttachShader;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
PFNGLENABLEVERTEXATTRIBARRAYARBPROC glEnableVertexAttribArray;
PFNGLUSEPROGRAMPROC glUseProgram;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
PFNGLUNIFORM4FPROC glUniform4f;

void LoadGL() {
    LOADFUNC(glCreateShader);
    LOADFUNC(glShaderSource);
    LOADFUNC(glCompileShader);
    LOADFUNC(glGetShaderiv);
    LOADFUNC(glGetShaderInfoLog);
    LOADFUNC(glGenBuffers);
    LOADFUNC(glBindBuffer);
    LOADFUNC(glBufferData);
    LOADFUNC(glCreateProgram);
    LOADFUNC(glAttachShader);
    LOADFUNC(glLinkProgram);
    LOADFUNC(glVertexAttribPointer);
    LOADFUNC(glEnableVertexAttribArray);
    LOADFUNC(glUseProgram);
    LOADFUNC(glGetUniformLocation);
    LOADFUNC(glUniform4f);
}
