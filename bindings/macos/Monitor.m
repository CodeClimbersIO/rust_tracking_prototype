// Monitor.m

#import "Monitor.h"
#import <objc/runtime.h>

@interface MonitorHolder : NSObject
@property (nonatomic, strong) NSArray<id> *monitors;
@property (nonatomic, assign) MouseEventCallback mouseCallback;
@end

@implementation MonitorHolder
@end

static MonitorHolder *monitorHolder = nil;

void start_mouse_monitoring(MouseEventCallback callback) {
    if (!monitorHolder) {
        monitorHolder = [[MonitorHolder alloc] init];
    }
    monitorHolder.mouseCallback = callback;
    
    NSMutableArray *monitors = [NSMutableArray array];
    
    // Mouse move monitor
    id moveMonitor = [NSEvent addGlobalMonitorForEventsMatchingMask:NSEventMaskMouseMoved 
                                                          handler:^(NSEvent *event) {
        callback(event.locationInWindow.x,
                event.locationInWindow.y,
                MouseEventTypeMove,
                0);
    }];
    if (moveMonitor) {
        [monitors addObject:moveMonitor];
    }
    
    // Click monitor
    NSEventMask clickMask = NSEventMaskLeftMouseDown | NSEventMaskLeftMouseUp |
                           NSEventMaskRightMouseDown | NSEventMaskRightMouseUp |
                           NSEventMaskOtherMouseDown | NSEventMaskOtherMouseUp;
    
    id clickMonitor = [NSEvent addGlobalMonitorForEventsMatchingMask:clickMask
                                                           handler:^(NSEvent *event) {
        MouseEventType eventType;
        switch (event.type) {
            case NSEventTypeLeftMouseDown:
                eventType = MouseEventTypeLeftDown;
                break;
            case NSEventTypeLeftMouseUp:
                eventType = MouseEventTypeLeftUp;
                break;
            case NSEventTypeRightMouseDown:
                eventType = MouseEventTypeRightDown;
                break;
            case NSEventTypeRightMouseUp:
                eventType = MouseEventTypeRightUp;
                break;
            case NSEventTypeOtherMouseDown:
                eventType = MouseEventTypeMiddleDown;
                break;
            case NSEventTypeOtherMouseUp:
                eventType = MouseEventTypeMiddleUp;
                break;
            default:
                return;
        }
        
        callback(event.locationInWindow.x,
                event.locationInWindow.y,
                eventType,
                0);
    }];
    if (clickMonitor) {
        [monitors addObject:clickMonitor];
    }
    
    // Scroll monitor
    id scrollMonitor = [NSEvent addGlobalMonitorForEventsMatchingMask:NSEventMaskScrollWheel
                                                            handler:^(NSEvent *event) {
        callback(event.locationInWindow.x,
                event.locationInWindow.y,
                MouseEventTypeScroll,
                (int32_t)event.scrollingDeltaY);
    }];
    if (scrollMonitor) {
        [monitors addObject:scrollMonitor];
    }
    
    monitorHolder.monitors = monitors;
}

void start_keyboard_monitoring(KeyboardEventCallback callback) {
    // TODO: Implement keyboard monitoring
}

void start_window_monitoring(WindowEventCallback callback) {
    // TODO: Implement window monitoring
}

void initialize(void) {
    [[NSApplication sharedApplication] setActivationPolicy:NSApplicationActivationPolicyAccessory];
}

void process_events(void) {
    NSDate *until = [NSDate dateWithTimeIntervalSinceNow:0.1];
    NSEvent *event = [[NSApplication sharedApplication] nextEventMatchingMask:NSEventMaskAny
                                                                  untilDate:until
                                                                     inMode:NSDefaultRunLoopMode
                                                                    dequeue:YES];
    if (event) {
        [[NSApplication sharedApplication] sendEvent:event];
    }
}

void cleanup(void) {
    if (monitorHolder) {
        for (id monitor in monitorHolder.monitors) {
            [NSEvent removeMonitor:monitor];
        }
        monitorHolder.monitors = nil;
        monitorHolder = nil;
    }
}