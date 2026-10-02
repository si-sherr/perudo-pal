#!/bin/bash
set -euo pipefail

SCRIPT_DIR=$(dirname "$0")

# Ensure we are in root directory
cd "$SCRIPT_DIR/.."

# Configure CMake
cmake -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo

# Build
cmake --build build
