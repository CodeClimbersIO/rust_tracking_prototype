#[repr(C)]
#[derive(Debug)]
pub enum MouseEventType {
    MouseMove = 0,
    MouseLeftDown,
    MouseLeftUp,
    MouseRightDown,
    MouseRightUp,
    MouseMiddleDown,
    MouseMiddleUp,
    MouseScroll,
}

#[repr(C)]
#[derive(Debug)]
pub struct MouseEventData {
    pub x: f64,
    pub y: f64,
    pub event_type: MouseEventType,
    pub scroll_delta: i32,
}

#[cfg(target_os = "macos")]
#[link(name = "MouseMonitor")]
extern "C" {
    fn initialize_cocoa();
    fn start_mouse_monitoring(callback: extern "C" fn(MouseEventData));
    fn process_events();
}

#[cfg(target_os = "windows")]
#[link(name = "WindowsMonitor")]
extern "C" {
    fn initialize_windows();
    fn start_mouse_monitoring(callback: extern "C" fn(MouseEventData));
    fn process_events();
    fn cleanup_windows();
}

extern "C" fn mouse_callback(data: MouseEventData) {
    match data.event_type {
        MouseEventType::MouseMove => {
            println!("Mouse moved to: ({}, {})", data.x, data.y);
        }
        MouseEventType::MouseLeftDown => {
            println!("Left button pressed at: ({}, {})", data.x, data.y);
        }
        MouseEventType::MouseLeftUp => {
            println!("Left button released at: ({}, {})", data.x, data.y);
        }
        MouseEventType::MouseRightDown => {
            println!("Right button pressed at: ({}, {})", data.x, data.y);
        }
        MouseEventType::MouseRightUp => {
            println!("Right button released at: ({}, {})", data.x, data.y);
        }
        MouseEventType::MouseMiddleDown => {
            println!("Middle button pressed at: ({}, {})", data.x, data.y);
        }
        MouseEventType::MouseMiddleUp => {
            println!("Middle button released at: ({}, {})", data.x, data.y);
        }
        MouseEventType::MouseScroll => {
            println!(
                "Scroll event: delta={} at ({}, {})", 
                data.scroll_delta, data.x, data.y
            );
        }
    }
}

fn main() {
    println!("Starting mouse monitoring from Rust...");
    
    unsafe {
        #[cfg(target_os = "macos")]
        {
            initialize_cocoa();
        }
        
        #[cfg(target_os = "windows")]
        {
            initialize_windows();
        }
        
        // Start monitoring mouse movements
        start_mouse_monitoring(mouse_callback);
        
        // Main event loop
        println!("Monitoring mouse movements. Press Ctrl+C to exit.");
        
        ctrlc::set_handler(|| {
            #[cfg(target_os = "windows")]
                cleanup_windows();
            std::process::exit(0);
        }).expect("Error setting Ctrl-C handler");

        loop {
            process_events();
            // Small sleep to prevent excessive CPU usage
            std::thread::sleep(std::time::Duration::from_millis(16)); // ~60 FPS
        }
    }
}
