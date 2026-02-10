# STRSTeamsCheckIn

A command-line tool for Microsoft Teams check-in automation.

## Prerequisites

### All Platforms
- CMake 3.20 or higher
- C++20 compatible compiler
- Git

### Platform-Specific Requirements

#### Linux
```bash
# Debian/Ubuntu
sudo apt-get install build-essential cmake libcurl4-openssl-dev

# Fedora/RHEL
sudo dnf install gcc-c++ cmake libcurl-devel

# Arch Linux
sudo pacman -S base-devel cmake curl
```

#### macOS
```bash
# Install Xcode Command Line Tools
xcode-select --install

# Install CMake (using Homebrew)
brew install cmake curl
```

#### Windows
- Visual Studio 2019 or later (with C++ desktop development workload)
- CMake (download from https://cmake.org/download/)
- CURL library (can be installed via vcpkg)

**Using vcpkg for CURL on Windows:**
```powershell
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg install curl:x64-windows
```

## Building

### Linux/macOS

```bash
# Clone the repository
git clone <repository-url>
cd STRSTeamsCheckIn

# Configure the build
cmake -B build -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build

# The executable will be at: build/checkin
```

### Windows (Visual Studio)

```powershell
# Clone the repository
git clone <repository-url>
cd STRSTeamsCheckIn

# If using vcpkg, specify the toolchain file
cmake -B build -DCMAKE_TOOLCHAIN_FILE="path/to/vcpkg/scripts/buildsystems/vcpkg.cmake"

# Or without vcpkg (if CURL is installed system-wide)
cmake -B build

# Build
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

## Troubleshooting

### CURL not found
- **Linux**: Install `libcurl4-openssl-dev` or `libcurl-devel`
- **macOS**: Install via `brew install curl`
- **Windows**: Use vcpkg or download from https://curl.se/windows/

### CMake version too old
Download the latest CMake from https://cmake.org/download/

### Compiler doesn't support C++20
- **Linux**: Install GCC 10+ or Clang 10+
- **macOS**: Update Xcode Command Line Tools
- **Windows**: Use Visual Studio 2019 16.11+ or Visual Studio 2022

## Build Options

### Debug Build
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

### Specify Compiler
```bash
# Use Clang
cmake -B build -DCMAKE_CXX_COMPILER=clang++

# Use specific GCC version
cmake -B build -DCMAKE_CXX_COMPILER=g++-11
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