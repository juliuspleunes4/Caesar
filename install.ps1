# Caesar Programming Language Installer
# This script installs Caesar to C:\Caesar and adds it to the user's PATH

param(
    [Parameter(HelpMessage="Installation directory (default: C:\Caesar)")]
    [string]$InstallDir = "C:\Caesar",
    
    [Parameter(HelpMessage="Skip PATH modification")]
    [switch]$SkipPath,
    
    [Parameter(HelpMessage="Force overwrite existing installation")]
    [switch]$Force
)

# Colors for output
$ErrorColor = "Red"
$SuccessColor = "Green"
$WarningColor = "Yellow"
$InfoColor = "Cyan"

Write-Host ""
Write-Host "======================================" -ForegroundColor $InfoColor
Write-Host "   Caesar Programming Language v1.1.0" -ForegroundColor $InfoColor
Write-Host "             Installer" -ForegroundColor $InfoColor
Write-Host "======================================" -ForegroundColor $InfoColor
Write-Host ""

# Check if running as administrator (optional but recommended)
$IsAdmin = ([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)
if (-not $IsAdmin) {
    Write-Host "NOTE: Running without administrator privileges." -ForegroundColor $WarningColor
    Write-Host "      Installation will proceed to user directory." -ForegroundColor $WarningColor
    Write-Host ""
}

# Build directory check
$BuildDir = Join-Path $PSScriptRoot "build\src"
if (-not (Test-Path $BuildDir)) {
    Write-Host "ERROR: Build directory not found!" -ForegroundColor $ErrorColor
    Write-Host "       Please run 'cmake --build build' first." -ForegroundColor $ErrorColor
    exit 1
}

# Check for required executables
$CaesarExe = Join-Path $BuildDir "caesar.exe"
$ReplExe = Join-Path $BuildDir "caesar_repl.exe"

if (-not (Test-Path $CaesarExe)) {
    Write-Host "ERROR: caesar.exe not found in build directory!" -ForegroundColor $ErrorColor
    Write-Host "       Please build the project first." -ForegroundColor $ErrorColor
    exit 1
}

if (-not (Test-Path $ReplExe)) {
    Write-Host "ERROR: caesar_repl.exe not found in build directory!" -ForegroundColor $ErrorColor
    Write-Host "       Please build the project first." -ForegroundColor $ErrorColor
    exit 1
}

# Create installation directory
Write-Host "Creating installation directory: $InstallDir" -ForegroundColor $InfoColor

try {
    if (Test-Path $InstallDir) {
        if ($Force) {
            Write-Host "Removing existing installation..." -ForegroundColor $WarningColor
            Remove-Item -Path $InstallDir -Recurse -Force
        } else {
            Write-Host "ERROR: Installation directory already exists!" -ForegroundColor $ErrorColor
            Write-Host "       Use -Force to overwrite or choose a different directory." -ForegroundColor $ErrorColor
            exit 1
        }
    }
    
    New-Item -ItemType Directory -Path $InstallDir -Force | Out-Null
    $BinPath = Join-Path $InstallDir "bin"
    New-Item -ItemType Directory -Path $BinPath -Force | Out-Null
    
    Write-Host "Created directory: $InstallDir" -ForegroundColor $SuccessColor
} catch {
    Write-Host "ERROR: Failed to create installation directory!" -ForegroundColor $ErrorColor
    Write-Host "       $($_.Exception.Message)" -ForegroundColor $ErrorColor
    exit 1
}

# Copy executables
Write-Host "Installing Caesar executables..." -ForegroundColor $InfoColor

try {
    Copy-Item -Path $CaesarExe -Destination $BinPath -Force
    Copy-Item -Path $ReplExe -Destination $BinPath -Force
    
    Write-Host "Installed caesar.exe" -ForegroundColor $SuccessColor
    Write-Host "Installed caesar_repl.exe" -ForegroundColor $SuccessColor
} catch {
    Write-Host "ERROR: Failed to copy executables!" -ForegroundColor $ErrorColor
    Write-Host "       $($_.Exception.Message)" -ForegroundColor $ErrorColor
    exit 1
}

# Add to PATH
if (-not $SkipPath) {
    Write-Host "Adding Caesar to PATH..." -ForegroundColor $WarningColor
    
    try {
        $CurrentPath = [Environment]::GetEnvironmentVariable("Path", "User")
        if ($CurrentPath -notlike "*$BinPath*") {
            if ($CurrentPath -and $CurrentPath -ne "") {
                $NewPath = $CurrentPath + ";" + $BinPath
            } else {
                $NewPath = $BinPath
            }
            [Environment]::SetEnvironmentVariable("Path", $NewPath, "User")
            Write-Host "Added to user PATH" -ForegroundColor $SuccessColor
            Write-Host "You may need to restart your terminal for PATH changes to take effect." -ForegroundColor $WarningColor
        } else {
            Write-Host "Already in PATH" -ForegroundColor $SuccessColor
        }
    } catch {
        Write-Host "WARNING: Failed to add to PATH!" -ForegroundColor $WarningColor
        Write-Host "         You can manually add $BinPath to your PATH." -ForegroundColor $WarningColor
    }
}

# Create examples directory
$ExamplesDir = Join-Path $InstallDir "examples"
$SourceExamples = Join-Path $PSScriptRoot "examples"

if (Test-Path $SourceExamples) {
    Write-Host "Copying example programs..." -ForegroundColor $InfoColor
    try {
        Copy-Item -Path $SourceExamples -Destination $InstallDir -Recurse -Force
        Write-Host "Installed example programs to $ExamplesDir" -ForegroundColor $SuccessColor
    } catch {
        Write-Host "WARNING: Failed to copy examples!" -ForegroundColor $WarningColor
    }
}

# Installation complete
Write-Host ""
Write-Host "======================================" -ForegroundColor $SuccessColor
Write-Host "   Installation Complete!" -ForegroundColor $SuccessColor
Write-Host "======================================" -ForegroundColor $SuccessColor
Write-Host ""
Write-Host "Caesar has been installed to: $InstallDir" -ForegroundColor $InfoColor
Write-Host ""
Write-Host "Usage:" -ForegroundColor $InfoColor
Write-Host "  caesar --help           Show help" -ForegroundColor $InfoColor
Write-Host "  caesar --version        Show version" -ForegroundColor $InfoColor
Write-Host "  caesar file.csr         Run a Caesar program" -ForegroundColor $InfoColor
Write-Host "  caesar_repl             Start interactive REPL" -ForegroundColor $InfoColor
Write-Host ""

if (-not $SkipPath) {
    Write-Host "Restart your terminal or run 'refreshenv' to use Caesar from anywhere." -ForegroundColor $WarningColor
} else {
    Write-Host "Add $BinPath to your PATH to use Caesar from anywhere." -ForegroundColor $InfoColor
}

Write-Host ""
Write-Host "Example programs are available in: $ExamplesDir" -ForegroundColor $InfoColor
Write-Host ""
Write-Host "Happy coding with Caesar!" -ForegroundColor $SuccessColor