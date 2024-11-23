#define MONITOR_EXPORTS
#include "monitor.h"
#include <windows.h>
#include <stdio.h>

MouseCallbackFn g_callback = NULL;

// Low-level mouse hook procedure
LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION && g_callback != NULL) {
        MSLLHOOKSTRUCT* hookStruct = (MSLLHOOKSTRUCT*)lParam;
        double x = (double)hookStruct->pt.x;
        double y = (double)hookStruct->pt.y;
                int event_type = 0;  // Default to MOUSE_MOVE
        int scroll_delta = 0;
        switch (wParam) {
            case WM_MOUSEMOVE:
                event_type = MOUSE_MOVE;
                printf("Mouse Move - ");
                break;
                
            case WM_LBUTTONDOWN:
                event_type = MOUSE_LEFT_DOWN;
                printf("Left Down - ");
                break;
                
            case WM_LBUTTONUP:
                event_type = MOUSE_LEFT_UP;
                printf("Left Up - ");
                break;
                
            case WM_RBUTTONDOWN:
                event_type = MOUSE_RIGHT_DOWN;
                printf("Right Down - ");
                break;
                
            case WM_RBUTTONUP:
                event_type = MOUSE_RIGHT_UP;
                printf("Right Up - ");
                break;
                
            case WM_MBUTTONDOWN:
                event_type = MOUSE_MIDDLE_DOWN;
                printf("Middle Down - ");
                break;
                
            case WM_MBUTTONUP:
                event_type = MOUSE_MIDDLE_UP;
                printf("Middle Up - ");
                break;
                
            case WM_MOUSEWHEEL:
                event_type = MOUSE_SCROLL;
                scroll_delta = GET_WHEEL_DELTA_WPARAM(hookStruct->mouseData) / WHEEL_DELTA;
                printf("Scroll (delta: %.2f) - ", scroll_delta);
                break;
                
            default:
                return CallNextHookEx(NULL, nCode, wParam, lParam);
        }

        g_callback(x, y, event_type, scroll_delta);
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

HHOOK g_mouseHook = NULL;

__declspec(dllexport) void initialize_windows(void) {
}

// Start monitoring mouse movements
__declspec(dllexport) void start_mouse_monitoring(MouseCallbackFn callback) {
    g_callback = callback;
    g_mouseHook = SetWindowsHookEx(
        WH_MOUSE_LL,
        LowLevelMouseProc,
        GetModuleHandle(NULL),
        0
    );
    
    if (g_mouseHook == NULL) {
        fprintf(stderr, "Failed to set mouse hook. Error code: %lu\n", GetLastError());
    }
}

__declspec(dllexport) void process_events(void) {
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

__declspec(dllexport) void cleanup_windows(void) {
    printf("WINDOWS CLEANUP");
    if (g_mouseHook != NULL) {
        UnhookWindowsHookEx(g_mouseHook);
        g_mouseHook = NULL;
    }
}