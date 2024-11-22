use std::env;
use std::path::PathBuf;

fn main() {
    println!("cargo:warning=Build script starting...");
    // Print environment info for debugging
    println!("cargo:info=Build script starting...");

    let target_os = env::var("CARGO_CFG_TARGET_OS").unwrap();
    println!("cargo:info=Target OS: {}", target_os);

    let manifest_dir = PathBuf::from(env::var("CARGO_MANIFEST_DIR").unwrap());
    println!("cargo:info=Manifest dir: {}", manifest_dir.display());

    if target_os == "macos" {
        println!("cargo:rustc-link-search=native=./bindings/macos");
        println!("cargo:rustc-link-lib=MouseMonitor");
    } else if target_os == "windows" {
        println!("cargo:info=Building for Windows...");

        // Define paths
        let source_path = manifest_dir
            .join("bindings")
            .join("windows")
            .join("monitor.c");
        let output_dir = manifest_dir
            .join("bindings")
            .join("windows")
            .join("release");

        // Print paths for debugging
        println!("cargo:info=Source path: {}", source_path.display());
        println!("cargo:info=Output directory: {}", output_dir.display());

        // Create output directory if it doesn't exist
        std::fs::create_dir_all(&output_dir).unwrap();

        // Compile the C code
        println!("cargo:info=Compiling C code...");
        cc::Build::new()
            .file(&source_path)
            .static_flag(false) // Not a static library
            .compile("WindowsMonitor");

        // Tell Cargo where to find the compiled library
        println!("cargo:info=Setting up library paths...");
        println!("cargo:rustc-link-search=native={}", output_dir.display());
        println!("cargo:rustc-link-lib=WindowsMonitor");

        // Also link against required Windows libraries
        println!("cargo:rustc-link-lib=user32");

        // Tell Cargo to rerun if our source changes
        println!("cargo:rerun-if-changed={}", source_path.display());
        println!("cargo:info=Build script completed successfully");
    }
}
