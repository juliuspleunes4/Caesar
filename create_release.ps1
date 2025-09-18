# Caesar Release Packaging Script
# Creates a distributable ZIP package for end users

param(
    [Parameter(HelpMessage="Version number (e.g., 1.1.0)")]
    [string]$Version = "1.1.0",
    
    [Parameter(HelpMessage="Output directory for the release package")]
    [string]$OutputDir = "release"
)

$ErrorActionPreference = "Stop"

Write-Host "======================================" -ForegroundColor Cyan
Write-Host "   Caesar Release Packager v$Version" -ForegroundColor Cyan
Write-Host "======================================" -ForegroundColor Cyan
Write-Host ""

# Ensure build exists
if (-not (Test-Path "build\src\caesar.exe")) {
    Write-Host "ERROR: Build not found! Please run 'cmake --build build' first." -ForegroundColor Red
    exit 1
}

# Create release directory structure
$ReleaseName = "caesar-v$Version-windows"
$ReleaseDir = Join-Path $OutputDir $ReleaseName
$BinDir = Join-Path $ReleaseDir "bin"
$ExamplesDir = Join-Path $ReleaseDir "examples"

Write-Host "Creating release directory: $ReleaseDir" -ForegroundColor Yellow

# Clean and create directories
if (Test-Path $ReleaseDir) {
    Remove-Item -Path $ReleaseDir -Recurse -Force
}
New-Item -ItemType Directory -Path $ReleaseDir -Force | Out-Null
New-Item -ItemType Directory -Path $BinDir -Force | Out-Null
New-Item -ItemType Directory -Path $ExamplesDir -Force | Out-Null

# Copy executables
Write-Host "Copying executables..." -ForegroundColor Yellow
Copy-Item -Path "build\src\caesar.exe" -Destination $BinDir
Copy-Item -Path "build\src\caesar_repl.exe" -Destination $BinDir

# Copy examples
Write-Host "Copying examples..." -ForegroundColor Yellow
if (Test-Path "examples") {
    Copy-Item -Path "examples\*" -Destination $ExamplesDir -Recurse
}

# Copy documentation
Write-Host "Copying documentation..." -ForegroundColor Yellow
Copy-Item -Path "USER_GUIDE.md" -Destination $ReleaseDir -ErrorAction SilentlyContinue

# Create standalone installers
Write-Host "Creating standalone installers..." -ForegroundColor Yellow

# Create standalone PowerShell installer
$StandalonePS1 = @"
# Caesar Programming Language Installer (Standalone)
# This installer works with pre-built binaries

param(
    [Parameter(HelpMessage="Installation directory (default: C:\Caesar)")]
    [string]`$InstallDir = "C:\Caesar",
    
    [Parameter(HelpMessage="Skip PATH modification")]
    [switch]`$SkipPath,
    
    [Parameter(HelpMessage="Force overwrite existing installation")]
    [switch]`$Force
)

# Colors for output
`$ErrorColor = "Red"
`$SuccessColor = "Green"
`$WarningColor = "Yellow"
`$InfoColor = "Cyan"

Write-Host ""
Write-Host "======================================" -ForegroundColor `$InfoColor
Write-Host "   Caesar Programming Language v$Version" -ForegroundColor `$InfoColor
Write-Host "             Installer" -ForegroundColor `$InfoColor
Write-Host "======================================" -ForegroundColor `$InfoColor
Write-Host ""

# Check for required executables in current directory
`$CaesarExe = Join-Path `$PSScriptRoot "bin\caesar.exe"
`$ReplExe = Join-Path `$PSScriptRoot "bin\caesar_repl.exe"

if (-not (Test-Path `$CaesarExe)) {
    Write-Host "ERROR: caesar.exe not found in bin directory!" -ForegroundColor `$ErrorColor
    Write-Host "       Please make sure you extracted the full ZIP package." -ForegroundColor `$ErrorColor
    exit 1
}

if (-not (Test-Path `$ReplExe)) {
    Write-Host "ERROR: caesar_repl.exe not found in bin directory!" -ForegroundColor `$ErrorColor
    Write-Host "       Please make sure you extracted the full ZIP package." -ForegroundColor `$ErrorColor
    exit 1
}

# Create installation directory
Write-Host "Creating installation directory: `$InstallDir" -ForegroundColor `$InfoColor

try {
    if (Test-Path `$InstallDir) {
        if (`$Force) {
            Write-Host "Removing existing installation..." -ForegroundColor `$WarningColor
            Remove-Item -Path `$InstallDir -Recurse -Force
        } else {
            Write-Host "ERROR: Installation directory already exists!" -ForegroundColor `$ErrorColor
            Write-Host "       Use -Force to overwrite or choose a different directory." -ForegroundColor `$ErrorColor
            exit 1
        }
    }
    
    New-Item -ItemType Directory -Path `$InstallDir -Force | Out-Null
    `$BinPath = Join-Path `$InstallDir "bin"
    New-Item -ItemType Directory -Path `$BinPath -Force | Out-Null
    
    Write-Host "Created directory: `$InstallDir" -ForegroundColor `$SuccessColor
} catch {
    Write-Host "ERROR: Failed to create installation directory!" -ForegroundColor `$ErrorColor
    Write-Host "       `$(`$_.Exception.Message)" -ForegroundColor `$ErrorColor
    exit 1
}

# Copy executables
Write-Host "Installing Caesar executables..." -ForegroundColor `$InfoColor

try {
    Copy-Item -Path `$CaesarExe -Destination `$BinPath -Force
    Copy-Item -Path `$ReplExe -Destination `$BinPath -Force
    
    Write-Host "Installed caesar.exe" -ForegroundColor `$SuccessColor
    Write-Host "Installed caesar_repl.exe" -ForegroundColor `$SuccessColor
} catch {
    Write-Host "ERROR: Failed to copy executables!" -ForegroundColor `$ErrorColor
    Write-Host "       `$(`$_.Exception.Message)" -ForegroundColor `$ErrorColor
    exit 1
}

# Add to PATH
if (-not `$SkipPath) {
    Write-Host "Adding Caesar to PATH..." -ForegroundColor `$WarningColor
    
    try {
        `$CurrentPath = [Environment]::GetEnvironmentVariable("Path", "User")
        if (`$CurrentPath -notlike "*`$BinPath*") {
            if (`$CurrentPath -and `$CurrentPath -ne "") {
                `$NewPath = `$CurrentPath + ";" + `$BinPath
            } else {
                `$NewPath = `$BinPath
            }
            [Environment]::SetEnvironmentVariable("Path", `$NewPath, "User")
            Write-Host "Added to user PATH" -ForegroundColor `$SuccessColor
            Write-Host "You may need to restart your terminal for PATH changes to take effect." -ForegroundColor `$WarningColor
        } else {
            Write-Host "Already in PATH" -ForegroundColor `$SuccessColor
        }
    } catch {
        Write-Host "WARNING: Failed to add to PATH!" -ForegroundColor `$WarningColor
        Write-Host "         You can manually add `$BinPath to your PATH." -ForegroundColor `$WarningColor
    }
}

# Copy examples if they exist
`$SourceExamples = Join-Path `$PSScriptRoot "examples"
if (Test-Path `$SourceExamples) {
    Write-Host "Copying example programs..." -ForegroundColor `$InfoColor
    try {
        Copy-Item -Path `$SourceExamples -Destination `$InstallDir -Recurse -Force
        `$ExamplesDir = Join-Path `$InstallDir "examples"
        Write-Host "Installed example programs to `$ExamplesDir" -ForegroundColor `$SuccessColor
    } catch {
        Write-Host "WARNING: Failed to copy examples!" -ForegroundColor `$WarningColor
    }
}

# Installation complete
Write-Host ""
Write-Host "======================================" -ForegroundColor `$SuccessColor
Write-Host "   Installation Complete!" -ForegroundColor `$SuccessColor
Write-Host "======================================" -ForegroundColor `$SuccessColor
Write-Host ""
Write-Host "Caesar has been installed to: `$InstallDir" -ForegroundColor `$InfoColor
Write-Host ""
Write-Host "Usage:" -ForegroundColor `$InfoColor
Write-Host "  caesar --help           Show help" -ForegroundColor `$InfoColor
Write-Host "  caesar --version        Show version" -ForegroundColor `$InfoColor
Write-Host "  caesar --interpret file.csr  Run a Caesar program" -ForegroundColor `$InfoColor
Write-Host "  caesar_repl             Start interactive REPL" -ForegroundColor `$InfoColor
Write-Host ""

if (-not `$SkipPath) {
    Write-Host "Restart your terminal or run 'refreshenv' to use Caesar from anywhere." -ForegroundColor `$WarningColor
} else {
    Write-Host "Add `$BinPath to your PATH to use Caesar from anywhere." -ForegroundColor `$InfoColor
}

Write-Host ""
if (Test-Path (Join-Path `$InstallDir "examples")) {
    Write-Host "Example programs are available in: " -NoNewline -ForegroundColor `$InfoColor
    Write-Host (Join-Path `$InstallDir "examples") -ForegroundColor `$InfoColor
}
Write-Host ""
Write-Host "Happy coding with Caesar!" -ForegroundColor `$SuccessColor
"@

$StandalonePS1 | Out-File -FilePath (Join-Path $ReleaseDir "install.ps1") -Encoding UTF8

# Create standalone batch installer
$StandaloneBat = @"
@echo off
echo ========================================
echo Caesar Programming Language Installer
echo ========================================
echo.

REM Check for executables
if not exist "%~dp0bin\caesar.exe" (
    echo ERROR: caesar.exe not found in bin directory!
    echo        Please make sure you extracted the full ZIP package.
    pause
    exit /b 1
)

if not exist "%~dp0bin\caesar_repl.exe" (
    echo ERROR: caesar_repl.exe not found in bin directory!
    echo        Please make sure you extracted the full ZIP package.
    pause
    exit /b 1
)

set INSTALL_DIR=%USERPROFILE%\Caesar
echo Installing Caesar to: %INSTALL_DIR%
echo.

REM Create installation directory
if exist "%INSTALL_DIR%" (
    echo Removing existing installation...
    rmdir /s /q "%INSTALL_DIR%"
)

mkdir "%INSTALL_DIR%"
mkdir "%INSTALL_DIR%\bin"

REM Copy executables
echo Copying executables...
copy "%~dp0bin\caesar.exe" "%INSTALL_DIR%\bin\" > nul
copy "%~dp0bin\caesar_repl.exe" "%INSTALL_DIR%\bin\" > nul
echo + caesar.exe copied
echo + caesar_repl.exe copied

REM Copy examples if they exist
if exist "%~dp0examples" (
    echo Copying examples...
    xcopy /e /i "%~dp0examples" "%INSTALL_DIR%\examples" > nul
    echo + Examples copied
)

REM Add to PATH
echo Adding to PATH...
setx PATH "%PATH%;%INSTALL_DIR%\bin" > nul 2>&1
if %errorlevel% equ 0 (
    echo + Added to user PATH ^(restart terminal to take effect^)
) else (
    echo WARNING: Failed to add to PATH automatically
    echo          Please manually add %INSTALL_DIR%\bin to your PATH
)

echo.
echo ========================================
echo Installation Complete!
echo ========================================
echo.
echo Caesar is now installed in: %INSTALL_DIR%
echo.
echo Usage:
echo   caesar --interpret file.csr  - Run a Caesar program
echo   caesar --help                - Show help
echo   caesar_repl                  - Start interactive REPL
echo.
echo Try: caesar --interpret "%INSTALL_DIR%\examples\hello_world.csr"
echo.
pause
"@

$StandaloneBat | Out-File -FilePath (Join-Path $ReleaseDir "install.bat") -Encoding ASCII

# Create README.md for the release
Write-Host "Creating README.md..." -ForegroundColor Yellow
$ReadmeContent = @"
# Caesar Programming Language v$Version

**A Python-like programming language with C++ performance**

## Quick Start

1. **Extract** this ZIP file to any directory
2. **Run the installer**:
   - **Windows PowerShell**: ``powershell -ExecutionPolicy Bypass -File install.ps1``
   - **Command Prompt**: Double-click ``install.bat``
3. **Start coding** with Caesar!

## What's Included

- ``bin/caesar.exe`` - Caesar compiler and interpreter
- ``bin/caesar_repl.exe`` - Interactive REPL
- ``examples/`` - Sample Caesar programs
- ``install.ps1`` - PowerShell installer
- ``install.bat`` - Batch installer
- ``USER_GUIDE.md`` - Complete documentation (if included)

## Usage

After installation, you can use Caesar from anywhere:

```bash
# Run a Caesar program
caesar --interpret program.csr

# Show help
caesar --help

# Show version
caesar --version

# Start interactive REPL
caesar_repl
```

## Sample Caesar Program

Create a file called ``hello.csr``:

```python
def greet(name):
    print("Hello,", name + "!")

if __name__ == "__main__":
    greet("World")
```

Run it:
```bash
caesar --interpret hello.csr
```

## Features

- ✅ Python-like syntax
- ✅ Functions with default parameters
- ✅ Control flow (if/elif/else, while, for loops)
- ✅ Built-in functions (print, range, len, str, int, float, type, abs)
- ✅ Recursive functions
- ✅ Variable scoping
- ✅ Mathematical operations
- ✅ String operations
- ✅ Interactive REPL

## System Requirements

- Windows 10/11 (64-bit)
- No additional dependencies required

## Getting Help

- Check the ``examples/`` directory for sample programs
- Use ``caesar --help`` for command-line options
- Start ``caesar_repl`` for interactive experimentation

## Installation Troubleshooting

**"caesar is not recognized as a command"**
- Restart your terminal/command prompt
- Or run: ``refreshenv`` (if you have Chocolatey)
- Or manually add the installation directory to your PATH

**Permission errors during installation**
- Run the installer as Administrator
- Or choose a different installation directory

## Happy Coding! 🚀

Caesar makes programming simple and fun. Enjoy building with Python-like syntax and C++ performance!
"@

$ReadmeContent | Out-File -FilePath (Join-Path $ReleaseDir "README.md") -Encoding UTF8

# Create the ZIP file
Write-Host "Creating ZIP package..." -ForegroundColor Yellow
$ZipPath = Join-Path $OutputDir "$ReleaseName.zip"

if (Test-Path $ZipPath) {
    Remove-Item $ZipPath -Force
}

# Use PowerShell's Compress-Archive
Compress-Archive -Path $ReleaseDir -DestinationPath $ZipPath -Force

# Cleanup temporary directory
Remove-Item -Path $ReleaseDir -Recurse -Force

# Success!
Write-Host ""
Write-Host "======================================" -ForegroundColor Green
Write-Host "   Release Package Created!" -ForegroundColor Green
Write-Host "======================================" -ForegroundColor Green
Write-Host ""
Write-Host "Package: $ZipPath" -ForegroundColor Cyan
Write-Host "Size: $([math]::Round((Get-Item $ZipPath).Length / 1MB, 2)) MB" -ForegroundColor Cyan
Write-Host ""
Write-Host "Ready for distribution!" -ForegroundColor Green
Write-Host ""
Write-Host "To test the package:" -ForegroundColor Yellow
Write-Host "1. Extract $ZipPath to a test directory" -ForegroundColor Yellow
Write-Host "2. Run install.bat or install.ps1" -ForegroundColor Yellow
Write-Host "3. Test with: caesar --interpret examples/hello_world.csr" -ForegroundColor Yellow