#Requires -RunAsAdministrator
$ErrorActionPreference = "Stop"

$AppName    = "STRS Teams Check-In"
$BinSource  = "STRSTeamsCheckIn.exe"
$BinName    = "checkin.exe"
$InstallDir = "$Env:ProgramFiles\STRSTeamsCheckIn"
$BinTarget  = Join-Path $InstallDir $BinName

function Fail($msg) {
    [Console]::Error.WriteLine("Error: $msg")
    exit 1
}

function Info($msg) {
    Write-Host "▶ $msg"
}

# ---------- OS check ----------
if ($env:OS -ne "Windows_NT") {
    Fail "This installer is for Windows only."
}

# ---------- source binary checks ----------
if (!(Test-Path $BinSource)) {
    Fail "Binary '$BinSource' not found in current directory."
}

# ---------- existing command conflict ----------
$existing = Get-Command checkin -ErrorAction SilentlyContinue
if ($existing -and $existing.Source -ne $BinTarget) {
    Fail "'checkin' already exists at $($existing.Source)"
}

# ---------- install ----------
Info "Installing $AppName to $InstallDir"
New-Item -ItemType Directory -Force -Path $InstallDir | Out-Null
Copy-Item $BinSource $BinTarget -Force

# ---------- PATH update ----------
$oldPath = [Environment]::GetEnvironmentVariable("Path", "Machine")
$pathEntries = $oldPath -split ';' | Where-Object { $_ -ne '' }
if ($InstallDir -notin $pathEntries) {
    Info "Adding to system PATH"
    [Environment]::SetEnvironmentVariable(
            "Path",
            (($pathEntries + $InstallDir) -join ';'),
            "Machine"
    )
}

# ---------- verification ----------
if (!(Test-Path $BinTarget)) {
    Fail "Install verification failed."
}

Info "Install successful!"
Write-Host "Restart your terminal and run:"
Write-Host "  checkin --help"
