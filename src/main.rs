#[link(name = "MouseMonitor")]
extern "C" {
    fn initialize_cocoa();
    fn start_mouse_monitoring(callback: extern "C" fn(f64, f64));
    fn process_events();
}

extern "C" fn mouse_callback(x: f64, y: f64) {
    println!("Mouse position: ({}, {})", x, y);
}

fn main() {
    println!("Starting mouse monitoring from Rust...");

    unsafe {
        // Initialize Cocoa
        initialize_cocoa();

        // Start monitoring mouse movements
        start_mouse_monitoring(mouse_callback);

        // Main event loop
        println!("Monitoring mouse movements. Press Ctrl+C to exit.");
        loop {
            process_events();
            // Small sleep to prevent excessive CPU usage
            std::thread::sleep(std::time::Duration::from_millis(16)); // ~60 FPS
        }
    }
}
