#!/usr/bin/env bash
set -euo pipefail

# Starts a lightweight X session and exposes it via noVNC on port 6080.
# After it starts, forward port 6080 in Codespaces and open the URL.
# Then, run your app inside this shell with DISPLAY=:0 LIBGL_ALWAYS_SOFTWARE=1 ./build/Niels3DGameEngine

# Install deps if missing
need() { command -v "$1" >/dev/null 2>&1 || MISSING=1; }
MISSING=0
for c in Xvfb x11vnc websockify fluxbox; do need "$c"; done
if [ "$MISSING" = "1" ]; then
  echo "[i] Installing VNC stack..."
  WITH_VNC=1 "$(dirname "$0")/install-deps.sh"
fi

export DISPLAY=:0
export LIBGL_ALWAYS_SOFTWARE=1

# Start Xvfb
if ! pgrep -x Xvfb >/dev/null; then
  Xvfb :0 -screen 0 1280x800x24 &
  sleep 0.5
fi

# Minimal window manager
if ! pgrep -x fluxbox >/dev/null; then
  fluxbox &
  sleep 0.5
fi

# Expose via noVNC
NOVNC_DIR=/usr/share/novnc
if [ ! -d "$NOVNC_DIR" ]; then
  echo "[!] noVNC not found; ensure novnc package is installed."
  exit 1
fi

# Start VNC server if not running
if ! pgrep -x x11vnc >/dev/null; then
  x11vnc -display :0 -nopw -forever -shared -rfbport 5900 &
  sleep 0.5
fi

# Start websockify for noVNC if not running
if ! pgrep -f "websockify.*6080" >/dev/null; then
  websockify --web "$NOVNC_DIR" 6080 localhost:5900 &
fi

echo "[OK] noVNC is running on port 6080. Forward the port and open in browser."
