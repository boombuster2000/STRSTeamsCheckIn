#!/usr/bin/env bash
set -euo pipefail

APP_NAME="STRS Teams Check-In"
BIN_NAME="checkin"
INSTALL_DIR="/usr/local/bin"
BIN_PATH="$INSTALL_DIR/$BIN_NAME"

# ---------- helpers ----------
error() {
  echo "Error: $1" >&2
  exit 1
}

info() {
  echo "▶ $1"
}

# ---------- OS checks ----------
OS="$(uname -s)"
case "$OS" in
  Linux|Darwin) ;;
  *)
    error "Unsupported OS: $OS. This uninstaller is for Linux/macOS only."
    ;;
esac

# ---------- existence checks ----------
if [[ ! -e "$BIN_PATH" ]]; then
  error "'$BIN_NAME' is not installed at $BIN_PATH"
fi

# ---------- ownership / conflict checks ----------
RESOLVED_PATH="$(command -v "$BIN_NAME" || true)"
if [[ -n "$RESOLVED_PATH" && "$RESOLVED_PATH" != "$BIN_PATH" ]]; then
  error "'$BIN_NAME' resolves to $RESOLVED_PATH (not managed by this installer)"
fi

# ---------- permission notice ----------
if [[ ! -w "$INSTALL_DIR" ]]; then
  info "Admin privileges required to remove from $INSTALL_DIR"
fi

# ---------- uninstall ----------
info "Removing $APP_NAME from $BIN_PATH"
sudo rm -f "$BIN_PATH"

# ---------- verification ----------
if command -v "$BIN_NAME" >/dev/null 2>&1; then
  error "Uninstall failed: '$BIN_NAME' still found in PATH"
fi

info "Uninstall successful!"
