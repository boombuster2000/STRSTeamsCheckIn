#!/usr/bin/env bash
set -e

BIN_NAME=checkin
INSTALL_DIR=/usr/local/bin

sudo install -m 755 STRSTeamsCheckIn $INSTALL_DIR/$BIN_NAME
echo "Installed to $INSTALL_DIR/$BIN_NAME"
