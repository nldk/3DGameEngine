#!/usr/bin/env bash
set -euo pipefail

# Configure and build the project into ./build

BUILD_DIR=${BUILD_DIR:-build}
TYPE=${TYPE:-Debug}

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

cmake -DCMAKE_BUILD_TYPE="$TYPE" ..
cmake --build . -j"$(nproc)"

echo "[OK] Build finished: $BUILD_DIR (type: $TYPE)"
