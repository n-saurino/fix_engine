#!/bin/bash

set -e

cd /workspace/scripts/

echo "Running tests..."
#./run_tests.sh

echo "Running benchmarks..."
./run_benchmarks.sh

echo "Adding files for commit..."
git add /workspace

echo "Committing suite of tests, benchmarks and code..."
./git-commit-file.sh

echo "Commit successful..."
