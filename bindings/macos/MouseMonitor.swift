import Cocoa

@_cdecl("start_mouse_monitoring")
public func startMouseMonitoring(callback: @escaping @convention(c) (Double, Double) -> Void) {
    // Ensure we have a reference to the app
    let app = NSApplication.shared
    
    // Create and store monitor to prevent it from being deallocated
    let monitor = NSEvent.addGlobalMonitorForEvents(matching: .mouseMoved) { event in
        let point = event.locationInWindow
        callback(point.x, point.y)
    }
    
    // Store the monitor reference to keep it alive
    if let monitor = monitor {
        objc_setAssociatedObject(app, "mouseMonitor", monitor, .OBJC_ASSOCIATION_RETAIN)
    }
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