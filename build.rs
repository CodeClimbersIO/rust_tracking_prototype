use std::env;
use std::path::PathBuf;

fn main() {
    println!("cargo:warning=Build script starting...");

    let target_os = env::var("CARGO_CFG_TARGET_OS").unwrap();
    println!("cargo:info=Target OS: {}", target_os);

    let manifest_dir = PathBuf::from(env::var("CARGO_MANIFEST_DIR").unwrap());
    println!("cargo:info=Manifest dir: {}", manifest_dir.display());

    if target_os == "macos" {
        println!("cargo:info=Building for macOS...");

        let source_file = manifest_dir
            .join("bindings")
            .join("macos")
            .join("Monitor.swift");
        let output_dir = manifest_dir.join("bindings").join("macos");

        println!("cargo:info=Source file: {}", source_file.display());
        println!("cargo:info=Output directory: {}", output_dir.display());

        // Build the Swift code
        println!("cargo:info=Compiling Swift code...");
        let status = std::process::Command::new("swiftc")
            .args(&[
                source_file.to_str().unwrap(),
                "-emit-library",
                "-o",
                output_dir.join("libMacMonitor.dylib").to_str().unwrap(),
            ])
            .status()
            .expect("Failed to execute swiftc command");

        if !status.success() {
            panic!("Swift compilation failed");
        }

        println!("cargo:info=Setting up library paths...");
        println!("cargo:rustc-link-search=native={}", output_dir.display());
        println!("cargo:rustc-link-lib=MacMonitor");

        // Tell Cargo to rerun if our Swift source changes
        println!("cargo:rerun-if-changed={}", source_file.display());
    } else if target_os == "windows" {
        println!("cargo:info=Building for Windows...");

        let source_path = manifest_dir
            .join("bindings")
            .join("windows_monitor")
            .join("windows_monitor")
            .join("monitor.c");
        let output_dir = manifest_dir
            .join("bindings")
            .join("windows_monitor")
            .join("windows_monitor")
            .join("release");

        println!("cargo:info=Source path: {}", source_path.display());
        println!("cargo:info=Output directory: {}", output_dir.display());

        std::fs::create_dir_all(&output_dir).unwrap();

        // Compile the C code
        println!("cargo:info=Compiling C code...");
        cc::Build::new()
            .file(&source_path)
            .static_flag(false) // Not a static library
            .out_dir(&output_dir) // Specify where to put the output
            .compile("WindowsMonitor");

        println!("cargo:info=Setting up library paths...");
        println!("cargo:rustc-link-search=native={}", output_dir.display());
        println!("cargo:rustc-link-lib=WindowsMonitor");

        println!("cargo:rustc-link-lib=user32");

        // Tell Cargo to rerun if our source changes
        println!("cargo:rerun-if-changed={}", source_path.display());
        println!("cargo:warning=Build script completed successfully");
    }
}
