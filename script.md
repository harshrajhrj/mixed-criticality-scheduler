This is a classic C++ linker error. Here's what's happening and how to fix it.

The error "undefined reference" means the compiler knows a function or class *exists* (from the `.h` header files), but the linker can't find the actual compiled code for it. Your command `g++ -o main main.cpp` only tells the compiler to process `main.cpp`, completely ignoring your other source files (`simulator.cpp`, `scheduler.cpp`, and `cpu.cpp`).

You need to tell `g++` to compile and link **all** the necessary `.cpp` files together.

-----

## Quick Fix

Compile all your source files at once. Make sure you are in the `src` directory when you run this command, or adjust the paths accordingly. Assuming you are in the project's root directory (`mixed-criticality-scheduler/`):

```bash
g++ -o main src/main.cpp src/scheduler.cpp src/simulator.cpp src/cpu.cpp -Iinclude
```

**Explanation of the command:**

  * `g++ -o main`: Compile and link, and name the final executable `main`.
  * `src/main.cpp src/scheduler.cpp ...`: The list of all source files that contain code.
  * `-Iinclude`: This is a crucial flag. It tells the compiler to look in the `include` directory for header files (like `#include "simulator.h"`).

This single command will compile everything and should resolve your linker errors. ✅

-----

## Better Solution (Recommended for C++ Projects)

Manually typing all the source files is tedious and inefficient. For any project with more than one file, you should use a build system. Your folder structure is already perfectly set up for **CMake**.

1.  **Create a `CMakeLists.txt` file** in your project's root directory (`mixed-criticality-scheduler/`) and paste the following content into it:

    ```cmake
    # Set the minimum required version of CMake
    cmake_minimum_required(VERSION 3.10)

    # Set the project name
    project(MixedCriticalityScheduler)

    # Tell CMake to use C++17 standard (good practice)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    # Find all your source files automatically
    file(GLOB SOURCES "src/*.cpp")

    # Add an executable target named "scheduler_sim"
    # It will be built from your source files
    add_executable(scheduler_sim ${SOURCES})

    # Tell the compiler where to find your header files (#include "...")
    target_include_directories(scheduler_sim PRIVATE include)
    ```

2.  **Now, build your project using these commands from the root directory:**

    ```bash
    # 1. Go into your build directory (create it if it doesn't exist)
    mkdir -p build
    cd build

    # 2. Run CMake to generate the build files (like a Makefile)
    cmake ..

    # 3. Run the actual build command
    cmake --build .
    ```

After this, you will find your executable (`scheduler_sim.exe` on Windows or `scheduler_sim` on Linux/macOS) inside the `build` directory.

Using CMake is the standard, professional way to handle C++ projects. It automatically tracks dependencies and only recompiles files that have changed, saving you a lot of time. 🚀