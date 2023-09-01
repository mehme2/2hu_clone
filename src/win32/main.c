#include <windows.h>
#include "../game.h"
#include "../loadgl.h"
#include <stdio.h>

gameButtons input;

HWND window;

void *loadFile(const char *path, u64 *size) {
    void *buf = 0;
    LARGE_INTEGER fileSize;
    HANDLE file = CreateFileA(path, GENERIC_READ, 0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if(file != INVALID_HANDLE_VALUE) {
        GetFileSizeEx(file, &fileSize);
        buf = VirtualAlloc(0, fileSize.QuadPart, MEM_COMMIT, PAGE_READWRITE);
        ReadFile(file, buf, fileSize.QuadPart, 0, 0);
        CloseHandle(file);
    }
    return buf;
}

inline void freeFile(void *ptr) {
    VirtualFree(ptr, 0, MEM_RELEASE);
}

inline void *allocMem(u64 size) {
    return HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size);
}

inline void freeMem(void *ptr) {
    HeapFree(GetProcessHeap(), 0, ptr);
}

void setWinTitle(const char *title) {
    SetWindowTextA(window, title);
}

LRESULT WindowProcedure(
        HWND window,
        UINT msg,
        WPARAM wParam,
        LPARAM lParam) {
    switch(msg) {
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP: {
				int prev = (lParam & (1 << 30)) != 0;
				int transition = (lParam & (1 << 31)) != 0;
				if(!(prev ^ transition)) {
					switch(wParam) {
						case VK_UP:
							input.up = !transition;
							break;
						case VK_DOWN:
							input.down = !transition;
							break;
						case VK_LEFT:
							input.left = !transition;
							break;
						case VK_RIGHT:
							input.right = !transition;
							break;
						case 'Z':
							input.a = !transition;
							break;
						case 'X':
							input.b = !transition;
							break;
					}
				}
			}
            break;
    }
    return DefWindowProcA(window, msg, wParam, lParam);
}

int WinMain(
        HINSTANCE hInstance,
        HINSTANCE NoneOf,
        LPSTR ThisMatters,
        int LOL) {
    const char *winName = "gam";
    const char *className = "GAM WINDOW";
    WNDCLASSA winClass = {0};
    winClass.style = CS_VREDRAW | CS_HREDRAW;
    winClass.lpfnWndProc = WindowProcedure;
    winClass.hInstance = hInstance;
    winClass.lpszClassName = className;
    RegisterClassA(&winClass);
    window = CreateWindowA(className, winName,
            WS_SYSMENU | WS_MINIMIZEBOX,
            0, 0, 1920, 1080,
            0, 0, hInstance, 0);
    ShowWindow(window, 1);
	PIXELFORMATDESCRIPTOR pixFormat = {0};
	pixFormat.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixFormat.nVersion = 1;
	pixFormat.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pixFormat.iPixelType = PFD_TYPE_RGBA;
	pixFormat.cColorBits = 32;
	pixFormat.cDepthBits = 32;
	pixFormat.iLayerType = PFD_MAIN_PLANE;
    HDC dc = GetDC(window);
    SetPixelFormat(dc, ChoosePixelFormat(dc, &pixFormat), &pixFormat);
    wglMakeCurrent(dc, wglCreateContext(dc));
    //char GLVersion[512];
	//sprintf(GLVersion, "\nGL Vendor: %s\nGL Renderer: %s\nGL Version: %s\nGL Shading Language: %s\n", glGetString(GL_VENDOR), glGetString(GL_RENDERER), glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION)); 
    //MessageBoxA(0, GLVersion, "GLINFO", MB_ICONEXCLAMATION);
    //char ErrorMsg[512];
    //LoadGL();
    gladLoadGL();
    //FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, 0, GetLastError(), 0, ErrorMsg, 512, 0);
    //MessageBoxA(0, ErrorMsg, "ERROR", MB_ICONEXCLAMATION);
    int exitCode = -1;
    MSG msg;
    LARGE_INTEGER timeCur;
    GetSystemTimeAsFileTime((LPFILETIME)&timeCur);
    gameInit();
    while(1) {
        if(PeekMessageA(&msg, 0, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
			if(msg.message == WM_QUIT) {
				exitCode = msg.wParam;
				break;
			}
        }
        LARGE_INTEGER timePrev = timeCur;
        GetSystemTimeAsFileTime((LPFILETIME)&timeCur);
        float delta = (double)(timeCur.QuadPart - timePrev.QuadPart) * 0.0000001;
        gameTick(&input, delta);
        SwapBuffers(dc);
    }
    return exitCode;
}
