
#include <windows.h>
#include <stdio.h>
#include <stdbool.h>
#include "monitor.h"

void test_callback(MouseEventData data) {
    const char* event_names[] = {
        "MOUSE_MOVE",
        "MOUSE_LEFT_DOWN",
        "MOUSE_LEFT_UP",
        "MOUSE_RIGHT_DOWN",
        "MOUSE_RIGHT_UP",
        "MOUSE_MIDDLE_DOWN",
        "MOUSE_MIDDLE_UP",
        "MOUSE_SCROLL"
    };

    printf("Mouse Event: %s\n", event_names[data.event_type]);
    printf("Position: (%.1f, %.1f)\n", data.x, data.y);

    if (data.event_type == MOUSE_SCROLL) {
        printf("Scroll delta: %d\n", data.scroll_delta);
    }
    printf("------------------------\n");
}

int main() {
    printf("Starting mouse monitor test...\n");
    printf("Press Ctrl+C to exit\n\n");

    // Initialize the monitor
    initialize_windows();

    // Start monitoring with our test callback
    start_mouse_monitoring(test_callback);

    // Main event loop
    while (1) {
        process_events();
        Sleep(10); // Small sleep to prevent high CPU usage
    }

    // Cleanup (note: this won't be reached normally, you'll need to Ctrl+C)
    cleanup_windows();

    return 0;
}