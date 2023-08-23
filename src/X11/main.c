#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <stdio.h>

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
    WindowAttributes.event_mask = ResizeRedirectMask;
    Window MainWindow = XCreateWindow(DefaultDisplay, RootWindow, 0, 0, 1920, 1080, 0, VisualInfo->depth, InputOutput, VisualInfo->visual, CWColormap | CWEventMask | CWBackPixel, &WindowAttributes);

    XStoreName(DefaultDisplay, MainWindow, "gam");
    XMapWindow(DefaultDisplay, MainWindow);

    XEvent Event;
    while(1) {
        while(XEventsQueued(DefaultDisplay, QueuedAfterFlush)) {
            XNextEvent(DefaultDisplay, &Event);
            switch(Event.type) {
            }
        }
    }
}
