# STRSTeamsCheckIn

A command-line tool for Microsoft Teams check-in automation.

## Prerequisites

### All Platforms
- CMake 3.20 or higher
- C++20-compatible compiler
- Git
- Internet connection (for downloading dependencies)

**Note:** All dependencies (CLI11, base64, CPR/libcurl) are automatically downloaded by CMake during the build process.
**Note:** School Wi-Fi may block the downloads for these dependencies.


### Platform-Specific Requirements

#### Linux
```bash
# Debian/Ubuntu
sudo apt-get install build-essential cmake git

# Fedora/RHEL
sudo dnf install gcc-c++ cmake git

# Arch Linux
sudo pacman -S base-devel cmake git
```

#### macOS
```bash
# Install Xcode Command Line Tools
xcode-select --install

# Install CMake (using Homebrew)
brew install cmake
```

#### Windows
- Visual Studio 2019 or later (with C++ desktop development workload)
- CMake
- Git for Windows

## Building

**Download the latest stable release from:** https://github.com/boombuster2000/STRSTeamsCheckIn/releases

### Linux/macOS
```bash
# Extract the downloaded source code
tar -xzf STRSTeamsCheckIn-*.tar.gz
cd STRSTeamsCheckIn-*

# Configure and build
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# The executable will be at: build/checkin
```

### Windows (Visual Studio)
```powershell
# Extract the downloaded zip file
# Navigate to the extracted folder in PowerShell

# Configure and build
cmake -B build
cmake --build build --config Release

# The executable will be at: build\Release\checkin.exe
```

## Installation

### System-Wide Installation (requires admin/sudo)

#### Linux/macOS
```bash
sudo cmake --install build --prefix /usr/local
```

This installs `checkin` to `/usr/local/bin/checkin`

#### Windows (as Administrator)
```powershell
cmake --install build --prefix "C:\Program Files\STRSTeamsCheckIn"
```

Add `C:\Program Files\STRSTeamsCheckIn\bin` to your PATH to use `checkin` from anywhere.

### User-Local Installation (no admin required)

#### Linux/macOS
```bash
cmake --install build --prefix ~/.local
```

Make sure `~/.local/bin` is in your PATH:
```bash
echo 'export PATH="$HOME/.local/bin:$PATH"' >> ~/.bashrc
source ~/.bashrc
```

#### Windows
```powershell
cmake --install build --prefix "$env:USERPROFILE\AppData\Local\Programs\STRSTeamsCheckIn"
```

Add to your user PATH:
1. Open "Edit environment variables for your account"
2. Add `%USERPROFILE%\AppData\Local\Programs\STRSTeamsCheckIn\bin` to PATH

### Custom Location
```bash
cmake --install build --prefix /path/to/custom/location
```
## Usage

After installation, run:
```bash
checkin --help
```

## Uninstalling

If you kept track of the install prefix:
```bash
# Linux/macOS
sudo rm /usr/local/bin/checkin

# Windows
del "C:\Program Files\STRSTeamsCheckIn\bin\checkin.exe"
```

Or use CMake's install manifest:
```bash
cat build/install_manifest.txt | sudo xargs rm
```

## Disclaimer

This project and its contents are not affiliated with, funded, authorized, endorsed by, or in any way associated with Sir Thomas Rich's School, Microsoft, or any of its affiliates and subsidiaries.