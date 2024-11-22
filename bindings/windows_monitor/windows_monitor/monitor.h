#ifndef MONITOR_H
#define MONITOR_H

#ifdef MONITOR_EXPORTS
#define MONITOR_API __declspec(dllexport)
#else
#define MONITOR_API __declspec(dllimport)
#endif

typedef enum {
    MOUSE_MOVE = 0,
    MOUSE_LEFT_DOWN,
    MOUSE_LEFT_UP,
    MOUSE_RIGHT_DOWN,
    MOUSE_RIGHT_UP,
    MOUSE_MIDDLE_DOWN,
    MOUSE_MIDDLE_UP,
    MOUSE_SCROLL
} MouseEventType;

typedef struct {
    double x;
    double y;
    MouseEventType event_type;
    int scroll_delta;
} MouseEventData;

typedef void (*MouseCallbackFn)(MouseEventData data);

MONITOR_API void initialize_windows(void);
MONITOR_API void start_mouse_monitoring(MouseCallbackFn callback);
MONITOR_API void process_events(void);
MONITOR_API void cleanup_windows(void);

#endif