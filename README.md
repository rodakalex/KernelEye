# KernelEye

**KernelEye** is a C++ application for monitoring process creation on Windows systems.  
It logs detailed information about each new process, including Process ID (PID), executable name, and parent process.

---

## Features

- Real-time monitoring of process creation.
- Logs the following details:
  - Process ID (PID)
  - Executable name
  - Parent Process ID
  - Timestamp of creation
- Saves logs to a file for further analysis.

---

## Requirements

- **Operating System**: Windows
- **Compiler**: GCC (MinGW) or MSVC
- **Tools**: CMake for project build


## How to Build

1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/KernelEye.git
   cd KernelEye
   ```

2. Create a build directory and configure the project using CMake with Ninja:
   ```bash
   mkdir build
   cd build
   cmake -G "Ninja" ..
   ```

3. Build the project using Ninja:
   ```bash
   ninja
   ```

4. Run the executable:
   ```bash
   ./KernelEye.exe
   ```

---

## Notes:
- **Ninja** is a fast build system that works seamlessly with CMake.
- Make sure **CMake** and **Ninja** are installed on your system.


---

## Usage

1. By default, logs are saved to the `logs/processes.log` file.
2. Run the application to monitor new processes:
   ```bash
   KernelEye.exe
   ```

3. Stop the application manually (Ctrl+C), or it will continue monitoring indefinitely.

---

## Project Structure

```
KernelEye/
│
├── include/            # Header files
│   ├── Logger.h
│   ├── ProcessMonitor.h
│   └── SystemUtils.h
│
├── src/                # Source files
│   ├── Logger.cpp
│   ├── ProcessMonitor.cpp
│   └── SystemUtils.cpp
│
├── logs/               # Log files
├── CMakeLists.txt      # CMake configuration file
└── README.md           # Project documentation
```

---

## Example Output

When running **KernelEye**, logs look like this:

```
[2024-12-15 15:34:23] KernelEye started...
[2024-12-17 19:24:05] New process detected: PID=25480, Name=C:\Users\abstract\AppData\Local\PowerToys\PowerToys.ColorPickerUI.exe, ParentPID=3032
[2024-12-17 19:24:05] New process detected: PID=23844, Name=C:\Users\abstract\AppData\Local\PowerToys\PowerToys.CropAndLock.exe, ParentPID=3032
[2024-12-17 19:24:05] New process detected: PID=17080, Name=C:\Users\abstract\AppData\Local\PowerToys\PowerToys.FancyZones.exe, ParentPID=3032
[2024-12-17 19:24:05] New process detected: PID=5732, Name=C:\Users\abstract\AppData\Local\PowerToys\WinUI3Apps\PowerToys.Peek.UI.exe, ParentPID=3032
[2024-12-17 19:24:05] New process detected: PID=17432, Name=C:\Users\abstract\AppData\Local\PowerToys\PowerToys.PowerLauncher.exe, ParentPID=3032
[2024-12-17 19:24:05] New process detected: PID=24444, Name=C:\Users\abstract\AppData\Local\PowerToys\PowerToys.PowerOCR.exe, ParentPID=3032
[2024-12-15 15:34:30] Stopping process monitor...
```

---

## Future Improvements

- Add support for monitoring process termination.
- Implement statistics on process creation over time.
- Provide an option to run as a Windows service.

---

## Contribution

Feel free to open issues or create pull requests to improve **KernelEye**.

---

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
