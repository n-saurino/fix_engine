#!/bin/bash

# Create a build directory if it doesn't exist
mkdir -p build
cd build

# Run CMake to configure the project
cmake ..

# Build the project
cmake --build . --clean-first

# Run the tests
ctest -R FIXMessageTests -V  # Runs only FIXMessages tests
ctest -R MemoryPoolTests -V  # Runs only MemoryPool tests
