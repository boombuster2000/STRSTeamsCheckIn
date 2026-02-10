#Requires -RunAsAdministrator
$ErrorActionPreference = "Stop"

$AppName    = "STRS Teams Check-In"
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
    Fail "This uninstaller is for Windows only."
}

# ---------- existence checks ----------
if (!(Test-Path $BinTarget)) {
    Fail "'checkin' is not installed at expected location."
}

# ---------- PATH ownership check ----------
$cmd = Get-Command checkin -ErrorAction SilentlyContinue
if ($cmd -and $cmd.Source -ne $BinTarget) {
    Fail "'checkin' resolves to $($cmd.Source), not managed by this installer."
}

# ---------- uninstall ----------
Info "Removing $AppName"
Remove-Item $BinTarget -Force

if (Test-Path $InstallDir) {
    Remove-Item $InstallDir -Force -Recurse
}

# ---------- PATH cleanup ----------
$oldPath = [Environment]::GetEnvironmentVariable("Path", "Machine")
$newPath = ($oldPath -split ';' | Where-Object { $_ -ne '' -and $_ -ne $InstallDir }) -join ';'
[Environment]::SetEnvironmentVariable("Path", $newPath, "Machine")

# ---------- verification ----------
if (Get-Command checkin -ErrorAction SilentlyContinue) {
    Fail "Uninstall verification failed."
}

Info "Uninstall successful!"
