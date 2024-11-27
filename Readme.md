# Project Name
Tracks the current active window

## Prerequisites

- Rust (latest stable version)
- Cargo (comes with Rust)
- Any other system requirements

## Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/your-project.git
   ```

2. Build the project:
   ```bash
   bash ./scripts/build-mac.sh
   cargo build
   ```

## Running the Project

### Development Mode

To run the project in development mode:
   ```bash
   cargo install cargo-watch
   cargo dev
   ```

### Release Mode

To run the project in release mode:
   ```bash
   cargo run --release
   ```
## Project Structure

- `src/`: Source code for the project.
  - `main.rs`: Main entry point of the program.
- `bindings/`: Bindings for the project.
  - `macos/`: MacOS bindings written in Objective-C.
  - `windows/`: Windows bindings written in C.
- `build.rs`: Build the C and Objective-C code for linking in the Rust code.

