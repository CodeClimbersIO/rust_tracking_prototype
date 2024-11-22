# Windows Monitor Project

This Visual Studio solution contains two projects:
- `windows_monitor`: A DLL that implements Windows mouse hooks
- `runner`: A test executable to verify the DLL functionality

## Setup
1. Open `windows_monitor.sln` in Visual Studio
2. Set the "runner" project as the startup project if not already set
   - Right-click "runner" in Solution Explorer
   - Select "Set as Startup Project"
3. Build Solution (F6)
4. Run (F5)

## Project Structure
- `monitor.h`: Main interface for the mouse hook functionality
- `monitor.c`: Implementation of Windows mouse hook
- `monitor_test.c`: Test program that demonstrates DLL usage

## Requirements
- Visual Studio 2019 or later
- Windows SDK
