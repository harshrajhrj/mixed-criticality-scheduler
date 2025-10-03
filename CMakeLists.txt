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