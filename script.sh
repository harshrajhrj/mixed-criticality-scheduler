# 1. Go into your build directory (create it if it doesn't exist)
mkdir -p build
cd build

# 2. Run CMake to generate the build files (like a Makefile)
cmake ..

# 3. Run the actual build command
cmake --build .