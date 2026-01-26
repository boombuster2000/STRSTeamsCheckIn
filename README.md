# STRS Teams Check-In

A cross-platform command-line tool for checking into STRS Teams locations.

## Features

- Simple command-line interface
- Secure token storage in `.env` file
- Cross-platform support (Windows, macOS, Linux)
- Automatic `.env` file creation with proper permissions

## Installation

1. Download the appropriate version for your operating system from the [Releases](../../releases) page.

2. **macOS/Linux only**: Make the file executable

```bash
   chmod +x strs-teams-checkin-*
```

## First-Time Setup

1. Run the program for the first time. It will automatically create a `.env` file in the same directory as the
   executable.

2. Open the `.env` file and replace `PASTE_YOUR_TOKEN` with your actual Teams token:
   **Example:**
    ```
       TOKEN=123abc-456def-gfgs
    ```

3. Run the program again.

## Usage

Simply run the executable:

**Windows:**

```cmd
strs-teams-checkin-windows-x64.exe
```

**macOS/Linux:**

```bash
./strs-teams-checkin-macos-arm64
```

When prompted, enter your location and press Enter. The program will check you in and display the response from Teams.

## Troubleshooting

### "Not Signed In" Error

If you receive a "Not Signed In" message, check the following:

- You are connected to the school Wi-Fi network
- Your token in the `.env` file is correct and up-to-date
- The token hasn't expired

### macOS Security Warning

If macOS prevents you from opening the app with a message about an unidentified developer:

1. Right-click (or Control-click) on the file
2. Select "Open" from the menu
3. Click "Open" in the dialog that appears

Alternatively, you can authorize it in System Settings > Privacy & Security.

## Security Note

Your token is stored locally in the `.env` file. Keep this file secure and do not share it with others. On Unix-like
systems (macOS/Linux), the file is automatically created with restricted permissions (readable/writable only by you).

## Support

For issues or questions, please [open an issue](../../issues) on GitHub.
