#!/bin/bash
set -euo pipefail

SCRIPT_DIR=$(dirname "$0")

# Ensure we are in root directory
cd "$SCRIPT_DIR/.."

# Find all source files and format
find src/ -iname '*.h' -o -iname '*.c' | xargs clang-format -i --verbose
