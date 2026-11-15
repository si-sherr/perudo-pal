#!/bin/bash
set -euo pipefail

SCRIPT_DIR=$(dirname "$0")

SRC_DIRS=(
    src/
    tests/
)

# Ensure we are in root directory
cd "$SCRIPT_DIR/.."

echo Running clang-format...
# Find all source files and format
find ${SRC_DIRS[@]} -iname '*.h' -o -iname '*.c' -o -iname '*.cpp' | xargs clang-format -i --verbose

# Optionally format cmake files if cmakefmt is installed
if command -v cmakefmt >/dev/null 2>&1 
then 
    echo Running cmakefmt...
    cmakefmt --in-place .
else
    echo "Skipping cmakefmt, executable not found"
fi

echo Formatting Done.
