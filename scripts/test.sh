#!/bin/bash
set -euo pipefail

SCRIPT_DIR=$(dirname "$0")

# Ensure we are in root directory
cd "$SCRIPT_DIR/.."

# Assumes CMake already configured, and tests are built
ctest --test-dir build/tests