#ifndef _GL_H
#define _GL_H

#ifndef __linux__
#include "win32/linkgl.h"
#else
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#endif

#endif
