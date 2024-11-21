#[cfg(target_os = "macos")]
#[link(name = "MouseMonitor")]
extern "C" {
    fn initialize_cocoa();
    fn start_mouse_monitoring(callback: extern "C" fn(f64, f64));
    fn process_events();
}

#[cfg(target_os = "windows")]
#[link(name = "WindowsMonitor")]
extern "C" {
    fn initialize_windows();
    fn start_mouse_monitoring(callback: extern "C" fn(f64, f64));
    fn process_events();
    fn cleanup_windows();
}

extern "C" fn mouse_callback(x: f64, y: f64) {
    println!("Mouse position: ({}, {})", x, y);
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
