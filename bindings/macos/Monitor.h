#import <Cocoa/Cocoa.h>

typedef NS_ENUM(int32_t, MouseEventType) {
    MouseEventTypeMove = 0,
    MouseEventTypeLeftDown,
    MouseEventTypeLeftUp,
    MouseEventTypeRightDown,
    MouseEventTypeRightUp,
    MouseEventTypeMiddleDown,
    MouseEventTypeMiddleUp,
    MouseEventTypeScroll
};

typedef void (*MouseEventCallback)(double x, double y, int32_t eventType, int32_t scrollDelta);
typedef void (*KeyboardEventCallback)(int32_t keyCode);
typedef void (*WindowEventCallback)(const char* title, const char* application, const char* windowClass);

// Function declarations
void start_mouse_monitoring(MouseEventCallback callback);
void start_keyboard_monitoring(KeyboardEventCallback callback);
void start_window_monitoring(WindowEventCallback callback);
void initialize(void);
void process_events(void);
void cleanup(void);