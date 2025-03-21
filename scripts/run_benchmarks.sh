#!/bin/bash
set -e  # Exit immediately if any command fails
set -o pipefail  # Capture errors in pipelines

# Define the build directory and benchmark executable path.
BUILD_DIR="/workspace/build"
BENCHMARK_EXEC="${BUILD_DIR}/benchmarks/FIXEncoderBenchmark"

# Create the build directory if it doesn't exist.
mkdir -p "${BUILD_DIR}"

cd /workspace

# Run CMake to configure the project.
echo "Configuring project..."
cmake -S . -B "${BUILD_DIR}" -DCMAKE_BUILD_TYPE=Release

# Build the project.
echo "Building project..."
cmake --build "${BUILD_DIR}" -- -j$(nproc)

# Run the benchmark executable.
echo "Running benchmarks..."
# "${BENCHMARK_EXEC}"
"${BUILD_DIR}/benchmarks/FIXEncoderBenchmark"
"${BUILD_DIR}/benchmarks/LFQueueBenchmark"
