#[repr(i32)]
#[derive(Debug, Clone, Copy, PartialEq)]
pub enum MouseEventType {
    MouseMove = 0,
    MouseLeftDown = 1,
    MouseLeftUp = 2,
    MouseRightDown = 3,
    MouseRightUp = 4,
    MouseMiddleDown = 5,
    MouseMiddleUp = 6,
    MouseScroll = 7,
}

impl MouseEventType {
    pub fn from_i32(value: i32) -> Option<MouseEventType> {
        match value {
            0 => Some(MouseEventType::MouseMove),
            1 => Some(MouseEventType::MouseLeftDown),
            2 => Some(MouseEventType::MouseLeftUp),
            3 => Some(MouseEventType::MouseRightDown),
            4 => Some(MouseEventType::MouseRightUp),
            5 => Some(MouseEventType::MouseMiddleDown),
            6 => Some(MouseEventType::MouseMiddleUp),
            7 => Some(MouseEventType::MouseScroll),
            _ => None,
        }
    }
}

#[cfg(target_os = "macos")]
#[link(name = "MouseMonitor")]
extern "C" {
    fn initialize_cocoa();
    fn start_mouse_monitoring(callback: extern "C" fn(f64, f64, i32, i32));
    fn process_events();
}

#[cfg(target_os = "windows")]
#[link(name = "WindowsMonitor")]
extern "C" {
    fn initialize_windows();
    fn start_mouse_monitoring(callback: extern "C" fn(f64, f64, i32, i32));
    fn process_events();
    fn cleanup_windows();
}

extern "C" fn mouse_callback(x: f64, y: f64, event_type: i32, scroll_delta: i32) {
    if let Some(event_type) = MouseEventType::from_i32(event_type) {
        match event_type {
            MouseEventType::MouseMove => {
                println!("Mouse moved to: ({}, {})", x, y);
            }
            MouseEventType::MouseLeftDown => {
                println!("Left button pressed at: ({}, {})", x, y);
            }
            MouseEventType::MouseLeftUp => {
                println!("Left button released at: ({}, {})", x, y);
            }
            MouseEventType::MouseRightDown => {
                println!("Right button pressed at: ({}, {})", x, y);
            }
            MouseEventType::MouseRightUp => {
                println!("Right button released at: ({}, {})", x, y);
            }
            MouseEventType::MouseMiddleDown => {
                println!("Middle button pressed at: ({}, {})", x, y);
            }
            MouseEventType::MouseMiddleUp => {
                println!("Middle button released at: ({}, {})", x, y);
            }
            MouseEventType::MouseScroll => {
                println!("Scroll event: delta={} at ({}, {})", scroll_delta, x, y);
            }
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
        })
        .expect("Error setting Ctrl-C handler");

        loop {
            process_events();
            // Small sleep to prevent excessive CPU usage
            std::thread::sleep(std::time::Duration::from_millis(16)); // ~60 FPS
        }
    }
}
