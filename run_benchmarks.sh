#!/bin/bash

set -e  # Exit script on first error
set -o pipefail  # Capture pipeline errors

# Define variables
BUILD_DIR="build"
BENCHMARK_DIR="./benchmarks"
LOG_FILE="/workspace/logs/benchmarks/benchmark_results.log"

# Check if the benchmark directory exists
if [ ! -d "$BENCHMARK_DIR" ]; then
  echo "Error: Benchmarks directory ($BENCHMARK_DIR) not found!"
  exit 1
fi

# Create or clear the log file
echo "Running benchmarks... Logging output to $LOG_FILE"
echo "===================================" > "$LOG_FILE"
echo "Benchmark Run: $(date)" >> "$LOG_FILE"
echo "===================================" >> "$LOG_FILE"

# Build the project (if not already built)
if [ ! -d "$BUILD_DIR" ]; then
  echo "Creating build directory..."
  mkdir -p "$BUILD_DIR"
fi

# Run CMake and build
echo "Configuring CMake..."
cmake -S . -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Release
echo "Building project..."
cmake --build "$BUILD_DIR" -- -j$(nproc)

# Run all benchmark executables in /benchmarks/
echo "Running Google Benchmarks..."
for benchmark in "$BUILD_DIR/benchmarks/"*; do
  if [[ -f "$benchmark" && -x "$benchmark" ]]; then  # Fix syntax
    echo "Executing: $benchmark"
    "$benchmark" | tee -a "$LOG_FILE"
    echo "-----------------------------------" >> "$LOG_FILE"
  fi
done

echo "Benchmarks completed! Results saved to $LOG_FILE."
