#ifndef _LINKGL_H
#define _LINKGL_H

#include <windows.h>
#include <gl/gl.h>
#include <GL/glext.h>

extern PFNGLCREATESHADERPROC glCreateShader;
extern PFNGLSHADERSOURCEPROC glShaderSource;
extern PFNGLCOMPILESHADERPROC glCompileShader;
extern PFNGLGETSHADERIVPROC glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
extern PFNGLGENBUFFERSPROC glGenBuffers;
extern PFNGLBINDBUFFERPROC glBindBuffer;
extern PFNGLBUFFERDATAPROC glBufferData;
extern PFNGLCREATEPROGRAMPROC glCreateProgram;
extern PFNGLATTACHSHADERPROC glAttachShader;
extern PFNGLLINKPROGRAMPROC glLinkProgram;
extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
extern PFNGLENABLEVERTEXATTRIBARRAYARBPROC glEnableVertexAttribArray;
extern PFNGLUSEPROGRAMPROC glUseProgram;
extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
extern PFNGLUNIFORM4FPROC glUniform4f;

void LinkGL();

#endif
