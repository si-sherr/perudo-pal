#!/bin/bash
set -euo pipefail

SCRIPT_DIR=$(dirname "$0")

# Ensure we are in root directory
cd "$SCRIPT_DIR/.."

echo Running clang-tidy on source files...

# Find all source files and format
# Explicity pass -std arg as clang-tidy assumes incorrect C standrd when not provided in compile_commands.json
find src -iname '*.h' -o -iname '*.c' | xargs clang-tidy -p build --extra-arg="-std=gnu23"

echo Checks Done.
