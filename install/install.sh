#!/usr/bin/env bash
set -euo pipefail

APP_NAME="STRS Teams Check-In"
BIN_SOURCE="STRSTeamsCheckIn"
BIN_NAME="checkin"
INSTALL_DIR="/usr/local/bin"

# ---------- helpers ----------
error() {
  echo "❌ Error: $1" >&2
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
    error "Unsupported OS: $OS. This installer is for Linux/macOS only."
    ;;
esac

# ---------- dependency checks ----------
command -v install >/dev/null 2>&1 || error "'install' command not found"
command -v sudo >/dev/null 2>&1 || error "'sudo' is required"

# ---------- binary checks ----------
[[ -f "$BIN_SOURCE" ]] || error "Binary '$BIN_SOURCE' not found in current directory"
[[ -x "$BIN_SOURCE" ]] || error "Binary '$BIN_SOURCE' is not executable"

# ---------- permission checks ----------
if [[ ! -w "$INSTALL_DIR" ]]; then
  info "Admin privileges required to write to $INSTALL_DIR"
fi

# ---------- conflict checks ----------
if command -v "$BIN_NAME" >/dev/null 2>&1; then
  EXISTING_PATH="$(command -v "$BIN_NAME")"
  if [[ "$EXISTING_PATH" != "$INSTALL_DIR/$BIN_NAME" ]]; then
    error "'$BIN_NAME' already exists at $EXISTING_PATH"
  fi
  info "Updating existing installation"
else
  info "Installing new command '$BIN_NAME'"
fi

# ---------- install ----------
info "Installing $APP_NAME → $INSTALL_DIR/$BIN_NAME"
sudo install -m 755 "$BIN_SOURCE" "$INSTALL_DIR/$BIN_NAME"

# ---------- verification ----------
command -v "$BIN_NAME" >/dev/null 2>&1 || error "Install verification failed"

info "Install successful!"
echo
echo "Run:"
echo "  $BIN_NAME --help"
