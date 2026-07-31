#!/bin/bash
set -e

BUILD_DIR="${1:-build}"

cmake -S . -B "$BUILD_DIR" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

cmake --build "$BUILD_DIR" -j

ln -sf "$BUILD_DIR/compile_commands.json" compile_commands.json

# Create internals variant alongside the original (same build output dir)
LIBEXT=so
if [ "$(uname)" = "Darwin" ]; then LIBEXT=dylib; fi
cp -f "$BUILD_DIR/libde265/libde265.$LIBEXT" "$BUILD_DIR/libde265/libde265-internals.$LIBEXT"

# Stage shared libraries at $BUILD_DIR/ root (unified output convention)
cp -f "$BUILD_DIR/libde265/libde265.$LIBEXT" "$BUILD_DIR/libde265.$LIBEXT"
cp -f "$BUILD_DIR/libde265/libde265-internals.$LIBEXT" "$BUILD_DIR/libde265-internals.$LIBEXT"
echo "[deploy] libde265.$LIBEXT + libde265-internals.$LIBEXT -> $BUILD_DIR/"
