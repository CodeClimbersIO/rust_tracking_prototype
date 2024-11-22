import Cocoa

// Make the enum accessible from C by adding a @objc attribute
@objc public enum MouseEventType: Int32 {
    case mouseMove = 0
    case mouseLeftDown
    case mouseLeftUp
    case mouseRightDown
    case mouseRightUp
    case mouseMiddleDown
    case mouseMiddleUp
    case mouseScroll
}

@_cdecl("start_mouse_monitoring")
public func startMouseMonitoring(callback: @escaping @convention(c) (Double, Double, Int32, Int32) -> Void) {
    let app = NSApplication.shared
    
    let moveMonitor = NSEvent.addGlobalMonitorForEvents(matching: .mouseMoved) { event in
        callback(event.locationInWindow.x,
                event.locationInWindow.y,
                MouseEventType.mouseMove.rawValue,
                0)
    }
    
    let clickMonitor = NSEvent.addGlobalMonitorForEvents(matching: [.leftMouseDown, .leftMouseUp,
                                                                   .rightMouseDown, .rightMouseUp,
                                                                   .otherMouseDown, .otherMouseUp]) { event in
        let eventType: MouseEventType
        switch event.type {
        case .leftMouseDown:   eventType = .mouseLeftDown
        case .leftMouseUp:     eventType = .mouseLeftUp
        case .rightMouseDown:  eventType = .mouseRightDown
        case .rightMouseUp:    eventType = .mouseRightUp
        case .otherMouseDown:  eventType = .mouseMiddleDown
        case .otherMouseUp:    eventType = .mouseMiddleUp
        default:               return
        }
        
        callback(event.locationInWindow.x,
                event.locationInWindow.y,
                eventType.rawValue,
                0)
    }
    
    let scrollMonitor = NSEvent.addGlobalMonitorForEvents(matching: .scrollWheel) { event in
        callback(event.locationInWindow.x,
                event.locationInWindow.y,
                MouseEventType.mouseScroll.rawValue,
                Int32(event.scrollingDeltaY))
    }
    
    // Store monitor references
    if let moveMonitor = moveMonitor,
       let clickMonitor = clickMonitor,
       let scrollMonitor = scrollMonitor {
        let monitors = [moveMonitor, clickMonitor, scrollMonitor]
        objc_setAssociatedObject(app, "mouseMonitors", monitors, .OBJC_ASSOCIATION_RETAIN)
    }
}

// Keep the enum for internal use
public enum MouseEventType: Int32 {
    case mouseMove = 0
    case mouseLeftDown
    case mouseLeftUp
    case mouseRightDown
    case mouseRightUp
    case mouseMiddleDown
    case mouseMiddleUp
    case mouseScroll
}

@_cdecl("initialize_cocoa")
public func initializeCocoa() {
    // Initialize Cocoa application without activating
    NSApplication.shared.setActivationPolicy(.accessory)
}

@_cdecl("process_events")
public func processEvents() {
    // Process one event
    if let event = NSApplication.shared.nextEvent(matching: .any, 
                                                until: Date(timeIntervalSinceNow: 0.1), 
                                                inMode: .default, 
                                                dequeue: true) {
        NSApplication.shared.sendEvent(event)
    }
}