#!/usr/bin/env bash
set -euo pipefail

# Installs OpenGL + X11 build/runtime deps, and optional noVNC stack for GUI in Codespaces.
# Usage:
#   ./scripts/install-deps.sh           # core build + runtime deps
#   WITH_VNC=1 ./scripts/install-deps.sh  # include xvfb/x11vnc/novnc/websockify/fluxbox

run() {
  if [ "$(id -u)" -ne 0 ]; then
    sudo -E "$@"
  else
    "$@"
  fi
}

export DEBIAN_FRONTEND=noninteractive
run apt-get update
run apt-get install -y --no-install-recommends \
  build-essential cmake pkg-config \
  git ca-certificates curl \
  libgl1-mesa-dev libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libxxf86vm-dev \
  xorg-dev \
  libwayland-dev wayland-protocols libxkbcommon-dev \
  libdbus-1-dev libudev-dev \
  libgl1-mesa-dri mesa-utils

if [ "${WITH_VNC:-0}" = "1" ]; then
  run apt-get install -y --no-install-recommends \
    xvfb x11vnc fluxbox novnc websockify
fi

run apt-get clean
run rm -rf /var/lib/apt/lists/*

echo "[OK] Dependencies installed."
