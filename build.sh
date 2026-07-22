#!/bin/bash
set -e

BUILD_DIR="${1:-build}"

cmake -S . -B "$BUILD_DIR" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

cmake --build "$BUILD_DIR" -j

ln -sf "$BUILD_DIR/compile_commands.json" compile_commands.json
