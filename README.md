# STRS Teams Check-In CLI

A cross-platform C++ command-line tool to check in to Microsoft Teams–based attendance systems using a saved
authentication token.

---

## Features

- Securely store a Teams authentication token locally
- Cross-platform (Windows, macOS, Linux)
- Hidden token input (no echo)
- Environment-variable token support
- Simple `setToken` and `checkin` commands

---
## Supported Platforms

| OS | Supported | Install Method |
|----|----------|----------------|
| Windows | ✅ | PowerShell installer |
| macOS | ✅ | Bash install script |
| Linux | ✅ | Bash install script |

---

## Installation

### macOS / Linux

#### 1. Build the binary
Make sure `STRSTeamsCheckIn` exists in the current directory.

#### 2. Run installer
```bash
chmod +x install.sh
./install.sh
```

- Installs the command as checkin
- Location: /usr/local/bin/checkin
- Requires sudo
- Performs safety checks (OS, conflicts, permissions)

#### Uninstall
```bash
chmod +x uninstall.sh
./uninstall.sh
```
### Windows

#### 1. Build the binary

Ensure STRSTeamsCheckIn.exe is present in the directory.

#### 2. Run installer (Administrator)
```powershell
powershell -ExecutionPolicy Bypass -File install.ps1
```
- Installs to: `C:\Program Files\STRSTeamsCheckIn\checkin.exe`
- Automatically adds to system PATH
- Requires Administrator privileges
- Safe against PATH conflicts

#### Uninstall
```powershell
powershell -ExecutionPolicy Bypass -File uninstall.ps1
```

## Verifying Installation
```bash
checkin --help
```
If this works, installation is complete.

