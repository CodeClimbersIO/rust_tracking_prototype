#define MONITOR_EXPORTS
#include "monitor.h"
#include <windows.h>
#include <stdio.h>

// Updated callback function type
typedef void (*MouseCallbackFn)(MouseEventData data);

MouseCallbackFn g_callback = NULL;

// Low-level mouse hook procedure
LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION && g_callback != NULL) {
        MSLLHOOKSTRUCT* hookStruct = (MSLLHOOKSTRUCT*)lParam;
        MouseEventData data = {0};
        
        // Convert screen coordinates to double
        data.x = (double)hookStruct->pt.x;
        data.y = (double)hookStruct->pt.y;
        
        // Determine the event type based on wParam
        switch (wParam) {
            case WM_MOUSEMOVE:
                data.event_type = MOUSE_MOVE;
                break;
                
            case WM_LBUTTONDOWN:
                data.event_type = MOUSE_LEFT_DOWN;
                break;
                
            case WM_LBUTTONUP:
                data.event_type = MOUSE_LEFT_UP;
                break;
                
            case WM_RBUTTONDOWN:
                data.event_type = MOUSE_RIGHT_DOWN;
                break;
                
            case WM_RBUTTONUP:
                data.event_type = MOUSE_RIGHT_UP;
                break;
                
            case WM_MBUTTONDOWN:
                data.event_type = MOUSE_MIDDLE_DOWN;
                break;
                
            case WM_MBUTTONUP:
                data.event_type = MOUSE_MIDDLE_UP;
                break;
                
            case WM_MOUSEWHEEL:
                data.event_type = MOUSE_SCROLL;
                // Get scroll delta (positive for scroll up, negative for scroll down)
                data.scroll_delta = GET_WHEEL_DELTA_WPARAM(hookStruct->mouseData) / WHEEL_DELTA;
                break;
                
            default:
                return CallNextHookEx(NULL, nCode, wParam, lParam);
        }
        
        // Call the callback with the mouse event data
        g_callback(data);
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
    if (g_mouseHook != NULL) {
        UnhookWindowsHookEx(g_mouseHook);
        g_mouseHook = NULL;
    }
}