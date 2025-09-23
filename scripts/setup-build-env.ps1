# setup-build-env.ps1
# Caesar Build Environment Setup Script
# 
# This script sets up the development environment for building Caesar
# on Windows with MinGW/MSYS2. It handles common build issues automatically.
#
# Usage: .\setup-build-env.ps1
#
# Author: J.J.G. Pleunes
# Version: 1.0.0

Write-Host "Caesar Build Environment Setup" -ForegroundColor Magenta
Write-Host "===============================" -ForegroundColor Magenta
Write-Host ""

# Function to check if a command exists
function Test-Command {
    param($Command)
    try {
        if (Get-Command $Command -ErrorAction SilentlyContinue) {
            return $true
        }
    } catch {
        return $false
    }
    return $false
}

# Add MinGW to PATH if not already present
$mingwPath = "C:\msys64\mingw64\bin"
$pathUpdated = $false

if (Test-Path $mingwPath) {
    if ($env:PATH -notlike "*$mingwPath*") {
        $env:PATH = "$mingwPath;$env:PATH"
        $pathUpdated = $true
        Write-Host "SUCCESS: Added MinGW to PATH: $mingwPath" -ForegroundColor Green
    } else {
        Write-Host "INFO: MinGW already in PATH" -ForegroundColor Yellow
    }
} else {
    Write-Host "WARNING: MinGW not found at $mingwPath" -ForegroundColor Yellow
    Write-Host "  Please install MSYS2 from: https://www.msys2.org/" -ForegroundColor Cyan
    Write-Host "  Then run: pacman -S mingw-w64-x86_64-toolchain mingw-w64-x86_64-cmake" -ForegroundColor Cyan
}

# Check for alternative MinGW locations
$alternativePaths = @(
    "C:\MinGW\bin",
    "C:\mingw64\bin",
    "C:\tools\mingw64\bin",
    "C:\msys64\mingw32\bin"
)

foreach ($altPath in $alternativePaths) {
    if ((Test-Path $altPath) -and ($env:PATH -notlike "*$altPath*") -and (-not $pathUpdated)) {
        $env:PATH = "$altPath;$env:PATH"
        Write-Host "SUCCESS: Found alternative MinGW at: $altPath" -ForegroundColor Green
        $pathUpdated = $true
        break
    }
}

Write-Host ""

# Verify build tools are available
Write-Host "Checking build tools..." -ForegroundColor Cyan

$buildTools = @{
    "gcc" = "C Compiler"
    "g++" = "C++ Compiler" 
    "mingw32-make" = "Make Build Tool"
    "cmake" = "CMake Build System"
}

$allToolsFound = $true
foreach ($tool in $buildTools.Keys) {
    if (Test-Command $tool) {
        try {
            $version = & $tool --version 2>$null | Select-Object -First 1
            Write-Host "SUCCESS: $($buildTools[$tool]): $version" -ForegroundColor Green
        } catch {
            Write-Host "SUCCESS: $($buildTools[$tool]): Found" -ForegroundColor Green
        }
    } else {
        Write-Host "ERROR: $($buildTools[$tool]): Not found" -ForegroundColor Red
        $allToolsFound = $false
    }
}

if (-not $allToolsFound) {
    Write-Host ""
    Write-Host "ERROR: Missing build tools. Please install MSYS2 and required packages:" -ForegroundColor Red
    Write-Host "   1. Download and install MSYS2: https://www.msys2.org/" -ForegroundColor Cyan
    Write-Host "   2. Open MSYS2 terminal and run:" -ForegroundColor Cyan
    Write-Host "      pacman -Syu" -ForegroundColor White
    Write-Host "      pacman -S mingw-w64-x86_64-toolchain mingw-w64-x86_64-cmake" -ForegroundColor White
    Write-Host "   3. Add C:\msys64\mingw64\bin to your system PATH" -ForegroundColor Cyan
    exit 1
}

Write-Host ""

# Check and clean build directory if needed
Write-Host "Checking build configuration..." -ForegroundColor Cyan

if (Test-Path "build/CMakeCache.txt") {
    $generator = Get-Content "build/CMakeCache.txt" | Select-String "CMAKE_GENERATOR:INTERNAL="
    if ($generator -and $generator -notlike "*MinGW*") {
        Write-Host "WARNING: Wrong CMake generator detected: $generator" -ForegroundColor Yellow
        Write-Host "  Cleaning build directory to fix generator mismatch..." -ForegroundColor Yellow
        
        try {
            Remove-Item -Recurse -Force build -ErrorAction Stop
            Write-Host "SUCCESS: Build directory cleaned" -ForegroundColor Green
        } catch {
            Write-Host "ERROR: Failed to clean build directory: $($_.Exception.Message)" -ForegroundColor Red
            Write-Host "  Please manually delete the 'build' folder and try again" -ForegroundColor Cyan
            exit 1
        }
    }
}

# Create build directory if it doesn't exist
if (-not (Test-Path "build")) {
    Write-Host "Creating build directory..." -ForegroundColor Cyan
    try {
        mkdir build | Out-Null
        Write-Host "SUCCESS: Build directory created" -ForegroundColor Green
    } catch {
        Write-Host "ERROR: Failed to create build directory: $($_.Exception.Message)" -ForegroundColor Red
        exit 1
    }
}

# Configure CMake with MinGW
Write-Host ""
Write-Host "Configuring CMake with MinGW generator..." -ForegroundColor Cyan

Set-Location build

try {
    cmake -G "MinGW Makefiles" .. 2>&1 | Tee-Object -Variable cmakeOutput
    
    if ($LASTEXITCODE -eq 0) {
        Write-Host ""
        Write-Host "SUCCESS: Build environment setup complete!" -ForegroundColor Green
        Write-Host ""
        Write-Host "Next steps:" -ForegroundColor Cyan
        Write-Host "  1. Build the project:  cmake --build ." -ForegroundColor White
        Write-Host "  2. Run tests:          ctest" -ForegroundColor White
        Write-Host "  3. Run specific test:  ctest -R test_lexer" -ForegroundColor White
        Write-Host ""
        Write-Host "For future sessions, ensure MinGW is in your PATH or run this script again." -ForegroundColor Yellow
    } else {
        Write-Host ""
        Write-Host "ERROR: CMake configuration failed!" -ForegroundColor Red
        Write-Host ""
        Write-Host "Common solutions:" -ForegroundColor Cyan
        Write-Host "  1. Ensure all build tools are properly installed" -ForegroundColor White
        Write-Host "  2. Check that MinGW is in your PATH" -ForegroundColor White
        Write-Host "  3. Try running as Administrator" -ForegroundColor White
        Write-Host "  4. Check the CMake output above for specific errors" -ForegroundColor White
        exit 1
    }
} catch {
    Write-Host "X CMake execution failed: $($_.Exception.Message)" -ForegroundColor Red
    exit 1
}