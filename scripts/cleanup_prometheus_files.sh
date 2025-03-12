#!/bin/bash
# Directory where active .prom files are written
ACTIVE_DIR="/workspace/logs/benchmarks/prometheus/active"
# Directory where scraped files will be archived
ARCHIVE_DIR="/workspace/logs/benchmarks/prometheus/archive"

mkdir -p "$ARCHIVE_DIR"

# Move files older than 1 minute (adjust as needed) to the archive
find "$ACTIVE_DIR" -type f -mmin +1 -exec mv {} "$ARCHIVE_DIR" \;
