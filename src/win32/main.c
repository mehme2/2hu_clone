#include <windows.h>
#include "../game.h"
#include "../loadgl.h"
#include <stdio.h>

GameButtons Input;

HWND HWnd;

void *LoadFile(const char *Filename, u64 *Size) {
    void *Buffer = 0;
    LARGE_INTEGER FileSize;
    HANDLE File = CreateFileA(Filename, GENERIC_READ, 0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if(File != INVALID_HANDLE_VALUE) {
        GetFileSizeEx(File, &FileSize);
        Buffer = VirtualAlloc(0, FileSize.QuadPart, MEM_COMMIT, PAGE_READWRITE);
        ReadFile(File, Buffer, FileSize.QuadPart, 0, 0);
        CloseHandle(File);
    }
    return Buffer;
}

inline void FreeFile(void *Pointer) {
    VirtualFree(Pointer, 0, MEM_RELEASE);
}

inline void *AllocateMemory(u64 Size) {
    return HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, Size);
}

inline void FreeMemory(void *Pointer) {
    HeapFree(GetProcessHeap(), 0, Pointer);
}

void SetWindowTitle(const char *Title) {
    SetWindowTextA(HWnd, Title);
}

LRESULT WindowProcedure(
        HWND HWnd,
        UINT Message,
        WPARAM WParam,
        LPARAM LParam) {
    switch(Message) {
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP: {
				int Prev = (LParam & (1 << 30)) != 0;
				int Transition = (LParam & (1 << 31)) != 0;
				if(!(Prev ^ Transition)) {
					switch(WParam) {
						case VK_UP:
							Input.Up = !Transition;
							break;
						case VK_DOWN:
							Input.Down = !Transition;
							break;
						case VK_LEFT:
							Input.Left = !Transition;
							break;
						case VK_RIGHT:
							Input.Right = !Transition;
							break;
						case 'Z':
							Input.A = !Transition;
							break;
						case 'X':
							Input.B = !Transition;
							break;
					}
				}
			}
            break;
    }
    return DefWindowProcA(HWnd, Message, WParam, LParam);
}

int WinMain(
        HINSTANCE HInstance,
        HINSTANCE NoneOf,
        LPSTR ThisMatters,
        int LOL) {
    const char *WindowName = "gam";
    const char *WindowClassName = "GAM WINDOW";
    WNDCLASSA WindowClass = {0};
    WindowClass.style = CS_VREDRAW | CS_HREDRAW;
    WindowClass.lpfnWndProc = WindowProcedure;
    WindowClass.hInstance = HInstance;
    WindowClass.lpszClassName = WindowClassName;
    RegisterClassA(&WindowClass);
    HWnd = CreateWindowA(WindowClassName, WindowName,
            WS_SYSMENU | WS_MINIMIZEBOX,
            0, 0, 1920, 1080,
            0, 0, HInstance, 0);
    ShowWindow(HWnd, 1);
	PIXELFORMATDESCRIPTOR PixelFormat = {0};
	PixelFormat.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	PixelFormat.nVersion = 1;
	PixelFormat.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	PixelFormat.iPixelType = PFD_TYPE_RGBA;
	PixelFormat.cColorBits = 32;
	PixelFormat.cDepthBits = 32;
	PixelFormat.iLayerType = PFD_MAIN_PLANE;
    HDC DeviceContext = GetDC(HWnd);
    SetPixelFormat(DeviceContext, ChoosePixelFormat(DeviceContext, &PixelFormat), &PixelFormat);
    wglMakeCurrent(DeviceContext, wglCreateContext(DeviceContext));
    //char GLVersion[512];
	//sprintf(GLVersion, "\nGL Vendor: %s\nGL Renderer: %s\nGL Version: %s\nGL Shading Language: %s\n", glGetString(GL_VENDOR), glGetString(GL_RENDERER), glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION)); 
    //MessageBoxA(0, GLVersion, "GLINFO", MB_ICONEXCLAMATION);
    //char ErrorMsg[512];
    //LoadGL();
    gladLoadGL();
    //FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, 0, GetLastError(), 0, ErrorMsg, 512, 0);
    //MessageBoxA(0, ErrorMsg, "ERROR", MB_ICONEXCLAMATION);
    int ExitCode = -1;
    MSG Message;
    LARGE_INTEGER TimeCurrent;
    GetSystemTimeAsFileTime((LPFILETIME)&TimeCurrent);
    GameInit();
    while(1) {
        if(PeekMessageA(&Message, 0, 0, 0, PM_REMOVE)) {
            TranslateMessage(&Message);
            DispatchMessageA(&Message);
			if(Message.message == WM_QUIT) {
				ExitCode = Message.wParam;
				break;
			}
        }
        LARGE_INTEGER TimePrevious = TimeCurrent;
        GetSystemTimeAsFileTime((LPFILETIME)&TimeCurrent);
        float delta = (double)(TimeCurrent.QuadPart - TimePrevious.QuadPart) * 0.0000001;
        GameTick(&Input, delta);
        SwapBuffers(DeviceContext);
    }
    return ExitCode;
}
