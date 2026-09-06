#!/bin/bash
set -euo pipefail

SCRIPT_DIR=$(dirname "$0")

SRC_DIRS=(
    src/
    tests/
)

# Ensure we are in root directory
cd "$SCRIPT_DIR/.."

# Find all source files and format
find $SRC_DIRS -iname '*.h' -o -iname '*.c' -o -iname '*.cpp' | xargs clang-format -i --verbose

# Optionally format cmake files if cmake-format is installed
if command -v cmake-format >/dev/null 2>&1 
then 
    find . -not -path "./build/*" -iname 'CMakeLists.txt' | xargs cmake-format -i
else
    echo "Skipping cmake-format, executable not found"
fi
