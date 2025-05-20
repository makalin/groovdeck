#!/bin/bash

# Create build directory
mkdir -p build
cd build

# Configure with CMake
cmake ..

# Build the project
cmake --build . --config Release

echo "Build complete! You can find the executable in the build directory." 