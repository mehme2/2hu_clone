#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysymdef.h>
#include <stdio.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <malloc.h>
#include <sys/stat.h>

#include "../game.h"
#include <GL/glx.h>

void *loadFile(const char *path, u64 *size) {
    void *buffer = 0;
    int fd = open(path, O_RDONLY);
    if(fd != -1) {
        struct stat fileStats;
        fstat(fd, &fileStats);
        *size = fileStats.st_size + 1;
        buffer = malloc(fileStats.st_size + 1);
        read(fd, buffer, fileStats.st_size);
        close(fd);
        ((char *)buffer)[fileStats.st_size] = 0;
    }
    return buffer;
}

inline void freeFile(void *Pointer) {
    free(Pointer);
}

inline void *allocMem(u64 Size) {
    return malloc(Size);
}
inline void freeMem(void *Pointer) {
    free(Pointer);
}

double getSeconds() {
	static struct timeval tv;
	gettimeofday(&tv, NULL);
	return (double)tv.tv_sec + ((double)tv.tv_usec / 1000000.0);
}

int main() {
    Display *display = XOpenDisplay(0);
    int screenNum = XDefaultScreen(display);
    Window rootWin = XDefaultRootWindow(display);

    GLint glXAttribs[] = {
        GLX_RGBA,
        GLX_DOUBLEBUFFER,
        GLX_STENCIL_SIZE, 0,
        GLX_DEPTH_SIZE, 24,
        GLX_RED_SIZE, 8,
        GLX_GREEN_SIZE, 8,
        GLX_BLUE_SIZE, 8,
        GLX_SAMPLE_BUFFERS, 0,
        GLX_SAMPLES, 0,
        None,
    };

    XVisualInfo *visual = glXChooseVisual(display, screenNum, glXAttribs);

    XSetWindowAttributes winAttribs;
    winAttribs.border_pixel = BlackPixel(display, screenNum);
    winAttribs.background_pixel = BlackPixel(display, screenNum);
    winAttribs.override_redirect = True;
    winAttribs.colormap = XCreateColormap(display, rootWin, visual->visual, AllocNone);
    winAttribs.event_mask = KeyPressMask | KeyReleaseMask | KeymapStateMask | ResizeRedirectMask | ExposureMask;
    Window window = XCreateWindow(display, rootWin, 0, 0, 1920, 1080, 0, visual->depth, InputOutput, visual->visual, CWColormap | CWEventMask | CWBackPixel, &winAttribs);

    GLXContext glxc = glXCreateContext(display, visual, 0, GL_TRUE);
    glXMakeCurrent(display, window, glxc);
    //LoadGL();
    gladLoadGL();

	printf("\nGL Vendor: %s\nGL Renderer: %s\nGL Version: %s\nGL Shading Language: %s\n", glGetString(GL_VENDOR), glGetString(GL_RENDERER), glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION)); 

    XStoreName(display, window, "gam");
    XMapWindow(display, window);

	printf("\nRendering context is %s.\n", glXIsDirect(display, glxc) ? "direct" : "indirect");

    XEvent event;
    KeySym keySym;
    char keyStr[32];
    int running = 1;
    double timePrev;
    double timeCur = getSeconds();

    gameButtons input = {0};
    gameInit();
    XResizeWindow(display, window, 1920, 720);

    while(running) {
        while(XEventsQueued(display, QueuedAfterFlush)) {
            XNextEvent(display, &event);
            switch(event.type) {
                case KeymapNotify:
                    XRefreshKeyboardMapping(&event.xmapping);
                    break;
                case KeyPress:
					XLookupString(&event.xkey, keyStr, sizeof(keyStr), &keySym, 0);
					//printf("\nPressed: %s - %d\n", keyStr, (int)keySym);
					switch(keySym) {
						case XK_q:
						case XK_Escape:
							running = 0;
							break;
                        case XK_Up:
                            input.up = 1;
                            break;
                        case XK_Down:
                            input.down = 1;
                            break;
                        case XK_Left:
                            input.left = 1;
                            break;
                        case XK_Right:
                            input.right = 1;
                            break;
                        case XK_z:
                            input.a = 1;
                            break;
                        case XK_x:
                            input.b = 1;
                            break;
					}
                    break;
                case KeyRelease:
					if(XEventsQueued(display, QueuedAfterFlush)) {
						XEvent next;
						XPeekEvent(display, &next);
						if(next.type == KeyPress && next.xkey.time == event.xkey.time && next.xkey.keycode == event.xkey.keycode) {
							XNextEvent(display, &next);
							break;
						}
					}
					XLookupString(&event.xkey, keyStr, sizeof(keyStr), &keySym, 0);
					//printf("\nRelease: %s - %d\n", keyStr, (int)keySym);
					switch(keySym) {
                        case XK_Up:
                            input.up = 0;
                            break;
                        case XK_Down:
                            input.down = 0;
                            break;
                        case XK_Left:
                            input.left = 0;
                            break;
                        case XK_Right:
                            input.right = 0;
                            break;
                        case XK_z:
                            input.a = 0;
                            break;
                        case XK_x:
                            input.b = 0;
                            break;
					}
                    break;
				case Expose:
					{
						XWindowAttributes Attributes;
						XGetWindowAttributes(display, window, &Attributes);
					}
            }
        }
        timePrev = timeCur;
        timeCur = getSeconds();
        gameTick(&input, timeCur - timePrev);
        glXSwapBuffers(display, window);
    }
}
