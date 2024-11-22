#define MONITOR_EXPORTS
#include "monitor.h"
#include <windows.h>
#include <stdio.h>

// Callback function types
typedef void (*MouseCallbackFn)(MouseEventData data);
typedef void (*KeyboardCallbackFn)(KeyboardEventData data);

// Global variables
MouseCallbackFn g_mouseCallback = NULL;
KeyboardCallbackFn g_keyboardCallback = NULL;
HHOOK g_mouseHook = NULL;
HHOOK g_keyboardHook = NULL;

// Low-level mouse hook procedure (unchanged)
LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION && g_mouseCallback != NULL) {
        MSLLHOOKSTRUCT* hookStruct = (MSLLHOOKSTRUCT*)lParam;
        MouseEventData data = { 0 };

        data.x = (double)hookStruct->pt.x;
        data.y = (double)hookStruct->pt.y;

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
            data.scroll_delta = GET_WHEEL_DELTA_WPARAM(hookStruct->mouseData) / WHEEL_DELTA;
            break;
        default:
            return CallNextHookEx(NULL, nCode, wParam, lParam);
        }

        g_mouseCallback(data);
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

// New: Low-level keyboard hook procedure
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION && g_keyboardCallback != NULL) {
        KBDLLHOOKSTRUCT* hookStruct = (KBDLLHOOKSTRUCT*)lParam;
        KeyboardEventData data = { 0 };

        data.virtual_key_code = hookStruct->vkCode;
        data.scan_code = hookStruct->scanCode;
        data.flags = hookStruct->flags;

        switch (wParam) {
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            data.event_type = KEY_DOWN;
            break;
        case WM_KEYUP:
        case WM_SYSKEYUP:
            data.event_type = KEY_UP;
            break;
        default:
            return CallNextHookEx(NULL, nCode, wParam, lParam);
        }

        g_keyboardCallback(data);
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

__declspec(dllexport) void initialize_windows(void) {
    // Initialization if needed
}

// Start monitoring mouse movements
__declspec(dllexport) void start_mouse_monitoring(MouseCallbackFn callback) {
    g_mouseCallback = callback;
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

// New: Start monitoring keyboard events
__declspec(dllexport) void start_keyboard_monitoring(KeyboardCallbackFn callback) {
    g_keyboardCallback = callback;
    g_keyboardHook = SetWindowsHookEx(
        WH_KEYBOARD_LL,
        LowLevelKeyboardProc,
        GetModuleHandle(NULL),
        0
    );

    if (g_keyboardHook == NULL) {
        fprintf(stderr, "Failed to set keyboard hook. Error code: %lu\n", GetLastError());
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
    if (g_keyboardHook != NULL) {
        UnhookWindowsHookEx(g_keyboardHook);
        g_keyboardHook = NULL;
    }
}