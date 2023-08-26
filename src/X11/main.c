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

void *LoadFile(const char *Filename, u64 *Size) {
    void *Buffer = 0;
    int File = open(Filename, O_RDONLY);
    if(File != -1) {
        struct stat FileStats;
        fstat(File, &FileStats);
        *Size = FileStats.st_size;
        Buffer = malloc(FileStats.st_size);
        read(File, Buffer, FileStats.st_size);
        close(File);
    }
    return Buffer;
}

inline void FreeFile(void *Pointer) {
    free(Pointer);
}

inline void *AllocateMemory(long Size) {
    return malloc(Size);
}
inline void FreeMemory(void *Pointer) {
    free(Pointer);
}

double GetSeconds() {
	static struct timeval tv;
	gettimeofday(&tv, NULL);
	return (double)tv.tv_sec + ((double)tv.tv_usec / 1000000.0);
}

int main() {
    Display *DefaultDisplay = XOpenDisplay(0);
    int ScreenNumber = XDefaultScreen(DefaultDisplay);
    Window RootWindow = XDefaultRootWindow(DefaultDisplay);

    GLint GLXAttributes[] = {
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

    XVisualInfo *VisualInfo = glXChooseVisual(DefaultDisplay, ScreenNumber, GLXAttributes);

    XSetWindowAttributes WindowAttributes;
    WindowAttributes.border_pixel = BlackPixel(DefaultDisplay, ScreenNumber);
    WindowAttributes.background_pixel = BlackPixel(DefaultDisplay, ScreenNumber);
    WindowAttributes.override_redirect = True;
    WindowAttributes.colormap = XCreateColormap(DefaultDisplay, RootWindow, VisualInfo->visual, AllocNone);
    WindowAttributes.event_mask = KeyPressMask | KeyReleaseMask | KeymapStateMask | ResizeRedirectMask | ExposureMask;
    Window MainWindow = XCreateWindow(DefaultDisplay, RootWindow, 0, 0, 1920, 1080, 0, VisualInfo->depth, InputOutput, VisualInfo->visual, CWColormap | CWEventMask | CWBackPixel, &WindowAttributes);

    GLXContext GLXC = glXCreateContext(DefaultDisplay, VisualInfo, 0, GL_TRUE);
    glXMakeCurrent(DefaultDisplay, MainWindow, GLXC);
    LoadGL();

	printf("\nGL Vendor: %s\nGL Renderer: %s\nGL Version: %s\nGL Shading Language: %s\n", glGetString(GL_VENDOR), glGetString(GL_RENDERER), glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION)); 

    XStoreName(DefaultDisplay, MainWindow, "gam");
    XMapWindow(DefaultDisplay, MainWindow);

	printf("\nRendering context is %s.\n", glXIsDirect(DefaultDisplay, GLXC) ? "direct" : "indirect");

    XEvent Event;
    KeySym KeySymbol;
    char KeyString[32];
    int running = 1;
    double TimePrevious;
    double TimeCurrent = GetSeconds();

    GameButtons Input = {0};
    GameInit();

    while(running) {
        while(XEventsQueued(DefaultDisplay, QueuedAfterFlush)) {
            XNextEvent(DefaultDisplay, &Event);
            switch(Event.type) {
                case KeymapNotify:
                    XRefreshKeyboardMapping(&Event.xmapping);
                    break;
                case KeyPress:
					XLookupString(&Event.xkey, KeyString, sizeof(KeyString), &KeySymbol, 0);
					printf("\nPressed: %s - %d\n", KeyString, (int)KeySymbol);
					switch(KeySymbol) {
						case XK_q:
						case XK_Escape:
							running = 0;
							break;
                        case XK_Up:
                            Input.Up = 1;
                            break;
                        case XK_Down:
                            Input.Down = 1;
                            break;
                        case XK_Left:
                            Input.Left = 1;
                            break;
                        case XK_Right:
                            Input.Right = 1;
                            break;
                        case XK_z:
                            Input.A = 1;
                            break;
                        case XK_x:
                            Input.B = 1;
                            break;
					}
                    break;
                case KeyRelease:
					if(XEventsQueued(DefaultDisplay, QueuedAfterFlush)) {
						XEvent Next;
						XPeekEvent(DefaultDisplay, &Next);
						if(Next.type == KeyPress && Next.xkey.time == Event.xkey.time && Next.xkey.keycode == Event.xkey.keycode) {
							XNextEvent(DefaultDisplay, &Next);
							break;
						}
					}
					XLookupString(&Event.xkey, KeyString, sizeof(KeyString), &KeySymbol, 0);
					printf("\nRelease: %s - %d\n", KeyString, (int)KeySymbol);
					switch(KeySymbol) {
                        case XK_Up:
                            Input.Up = 0;
                            break;
                        case XK_Down:
                            Input.Down = 0;
                            break;
                        case XK_Left:
                            Input.Left = 0;
                            break;
                        case XK_Right:
                            Input.Right = 0;
                            break;
                        case XK_z:
                            Input.A = 0;
                            break;
                        case XK_x:
                            Input.B = 0;
                            break;
					}
                    break;
				case Expose:
					{
						XWindowAttributes Attributes;
						XGetWindowAttributes(DefaultDisplay, MainWindow, &Attributes);
					}
					break;
            }
        }
        TimePrevious = TimeCurrent;
        TimeCurrent = GetSeconds();
        GameTick(&Input, TimeCurrent - TimePrevious);
        glXSwapBuffers(DefaultDisplay, MainWindow);
    }
}
