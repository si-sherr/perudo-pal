#!/bin/bash
set -euo pipefail

SCRIPT_DIR=$(dirname "$0")

cd  "$SCRIPT_DIR/.."

mv build/src/cli/ppal ~/.local/bin/

echo "Copied ppal to ~/.local/bin/"
