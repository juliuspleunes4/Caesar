#!/usr/bin/env pwsh
<#
.SYNOPSIS
    Caesar Build and Test Script
.DESCRIPTION
    Comprehensive build, test, and validation script for Caesar Language development.
    Ensures binaries are always up-to-date and properly deployed.
.EXAMPLE
    .\build-and-test.ps1
    .\build-and-test.ps1 -SkipTests
    .\build-and-test.ps1 -Verbose
#>

param(
    [switch]$SkipTests,
    [switch]$Clean,
    [switch]$Verbose
)

$ErrorActionPreference = "Stop"

function Write-Step {
    param([string]$Message, [string]$Color = "Cyan")
    Write-Host "`n🔄 $Message" -ForegroundColor $Color
}

function Write-Success {
    param([string]$Message)
    Write-Host "✅ $Message" -ForegroundColor Green
}

function Write-Warning {
    param([string]$Message)
    Write-Host "⚠️  $Message" -ForegroundColor Yellow
}

function Write-Error {
    param([string]$Message)
    Write-Host "❌ $Message" -ForegroundColor Red
}

# Check if we're in the correct directory
if (-not (Test-Path "CMakeLists.txt") -or -not (Test-Path "src")) {
    Write-Error "This script must be run from the Caesar project root directory"
    exit 1
}

Write-Host "🚀 Caesar Language Build and Test Pipeline" -ForegroundColor Magenta
Write-Host "===========================================" -ForegroundColor Magenta

# Clean build if requested
if ($Clean) {
    Write-Step "Cleaning build directory..."
    if (Test-Path "build") {
        Remove-Item -Recurse -Force "build"
        Write-Success "Build directory cleaned"
    }
}

# Build the project
Write-Step "Building Caesar project..."
try {
    $buildOutput = cmake --build build 2>&1
    if ($LASTEXITCODE -ne 0) {
        Write-Error "Build failed!"
        if ($Verbose) { Write-Host $buildOutput }
        exit 1
    }
    Write-Success "Build completed successfully"
    if ($Verbose) { Write-Host $buildOutput }
} catch {
    Write-Error "Build failed: $($_.Exception.Message)"
    exit 1
}

# Verify binaries exist and are current
Write-Step "Verifying binary deployment..."
$buildBinary = "build\src\caesar.exe"
$deployBinary = "bin\caesar.exe"

if (-not (Test-Path $buildBinary)) {
    Write-Error "Build binary not found: $buildBinary"
    exit 1
}

if (-not (Test-Path $deployBinary)) {
    Write-Warning "Deploy binary not found, copying from build..."
    Copy-Item $buildBinary $deployBinary
} else {
    $buildTime = (Get-Item $buildBinary).LastWriteTime
    $deployTime = (Get-Item $deployBinary).LastWriteTime
    
    if ($buildTime -gt $deployTime) {
        Write-Warning "Deploy binary is outdated, updating..."
        Copy-Item $buildBinary $deployBinary
    }
}

Write-Success "Binary deployment verified"

# Show version of deployed binary
Write-Step "Checking deployed binary version..."
try {
    $versionOutput = & $deployBinary --version 2>&1
    if ($LASTEXITCODE -eq 0) {
        Write-Host $versionOutput
        Write-Success "Binary version check passed"
    } else {
        Write-Error "Binary version check failed"
        exit 1
    }
} catch {
    Write-Error "Failed to run binary: $($_.Exception.Message)"
    exit 1
}

# Run tests if not skipped
if (-not $SkipTests) {
    Write-Step "Running test suite..."
    try {
        Set-Location "build"
        $testOutput = ctest --output-on-failure 2>&1
        Set-Location ".."
        
        if ($LASTEXITCODE -eq 0) {
            Write-Success "All tests passed!"
            if ($Verbose) { Write-Host $testOutput }
        } else {
            Write-Error "Some tests failed!"
            Write-Host $testOutput
            exit 1
        }
    } catch {
        Set-Location ".."
        Write-Error "Test execution failed: $($_.Exception.Message)"
        exit 1
    }
    
    # Test enhanced data structures specifically
    Write-Step "Testing enhanced data structures..."
    try {
        $testResult = & $deployBinary --interpret "test_dict.csr" 2>&1
        if ($LASTEXITCODE -eq 0 -and $testResult -like "*Caesar*" -and $testResult -like "*<class 'list'>*" -and $testResult -like "*<class 'dict'>*") {
            Write-Success "Enhanced data structures working correctly"
            if ($Verbose) { 
                Write-Host "Test output:"
                Write-Host $testResult
            }
        } else {
            Write-Error "Enhanced data structures test failed"
            Write-Host $testResult
            exit 1
        }
    } catch {
        Write-Error "Enhanced data structures test failed: $($_.Exception.Message)"
        exit 1
    }
}

Write-Host "`n🎉 All checks passed! Caesar is ready for development." -ForegroundColor Green
Write-Host "Use .\dev-run.ps1 <file.csr> for development testing" -ForegroundColor Yellow