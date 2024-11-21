#include <windows.h>
#include <stdio.h>

// Function pointer type for the callback
typedef void (*MouseCallbackFn)(double x, double y);
MouseCallbackFn g_callback = NULL;

// Low-level mouse hook procedure
LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION && g_callback != NULL) {
        MSLLHOOKSTRUCT* hookStruct = (MSLLHOOKSTRUCT*)lParam;
        
        // Convert screen coordinates to double
        double x = (double)hookStruct->pt.x;
        double y = (double)hookStruct->pt.y;
        
        // Call the Rust callback with the mouse coordinates
        g_callback(x, y);
    }
    
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

HHOOK g_mouseHook = NULL;

// Initialize Windows-specific resources
__declspec(dllexport) void initialize_windows(void) {
    // Nothing specific needed for initialization
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

// Process Windows messages
__declspec(dllexport) void process_events(void) {
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

// Cleanup function
__declspec(dllexport) void cleanup_windows(void) {
    if (g_mouseHook != NULL) {
        UnhookWindowsHookEx(g_mouseHook);
        g_mouseHook = NULL;
    }
}