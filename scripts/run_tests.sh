#!/bin/bash

# Create a build directory if it doesn't exist
mkdir -p /workspace/build
cd /workspace/build

# Run CMake to configure the project
cmake .. -DCMAKE_BUILD_TYPE=Debug

# Build the project
cmake --build . --clean-first

# Run the tests
ctest -R FIXMessageTests -V  # Runs only FIXMessages tests
ctest -R MemoryPoolTests -V  # Runs only MemoryPool tests
ctest -R LFQueueTests -V # Runs only LFQueue tests
