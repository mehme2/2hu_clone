#include "linkgl.h"

#define ATTACHPROC(name) name = (void *)wglGetProcAddress(#name)

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


void LinkGL() {
    ATTACHPROC(glCreateShader);
    ATTACHPROC(glShaderSource);
    ATTACHPROC(glCompileShader);
    ATTACHPROC(glGetShaderiv);
    ATTACHPROC(glGetShaderInfoLog);
    ATTACHPROC(glGenBuffers);
    ATTACHPROC(glBindBuffer);
    ATTACHPROC(glBufferData);
    ATTACHPROC(glCreateProgram);
    ATTACHPROC(glAttachShader);
    ATTACHPROC(glLinkProgram);
    ATTACHPROC(glVertexAttribPointer);
    ATTACHPROC(glEnableVertexAttribArray);
    ATTACHPROC(glUseProgram);
    ATTACHPROC(glGetUniformLocation);
    ATTACHPROC(glUniform4f);
    glGenBuffers(0, 0);
}
