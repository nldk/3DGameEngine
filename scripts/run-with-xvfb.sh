#!/usr/bin/env bash
set -euo pipefail

# Runs the engine under Xvfb (software OpenGL). Useful in Codespaces where no GPU/display is available.
# Usage: ./scripts/run-with-xvfb.sh [args passed to the binary]

SCREEN_RES=${SCREEN_RES:-1280x800x24}
BUILD_DIR=${BUILD_DIR:-build}
TYPE=${TYPE:-Debug}

if [ ! -x "$BUILD_DIR/Niels3DGameEngine" ]; then
  echo "[i] Binary not found; building first..."
  BUILD_DIR="$BUILD_DIR" TYPE="$TYPE" "$(dirname "$0")/build.sh"
fi

# Ensure GL libraries exist
if ! command -v xvfb-run >/dev/null 2>&1; then
  echo "[!] xvfb-run not found. Install with: WITH_VNC=1 ./scripts/install-deps.sh"
  exit 1
fi

export LIBGL_ALWAYS_SOFTWARE=1
export XDG_RUNTIME_DIR=${XDG_RUNTIME_DIR:-/tmp}
mkdir -p "$XDG_RUNTIME_DIR"
xvfb-run -s "-screen 0 ${SCREEN_RES}" "${BUILD_DIR}/Niels3DGameEngine" "$@"
