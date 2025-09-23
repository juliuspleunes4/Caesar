#!/usr/bin/env pwsh
<#
.SYNOPSIS
    Caesar Language Development Helper Script
.DESCRIPTION
    This script ensures you're always testing with the latest build.
    It builds the project and runs Caesar with the correct binary.
.PARAMETER File
    The Caesar source file to run
.PARAMETER Tokens
    Show tokenization output
.PARAMETER Parse
    Show parsing output (AST)
.PARAMETER Help
    Show Caesar help
.PARAMETER Version
    Show Caesar version
.EXAMPLE
    .\dev-run.ps1 test.csr
    .\dev-run.ps1 -Parse test.csr
    .\dev-run.ps1 -Tokens test.csr
#>

param(
    [Parameter(Position=0)]
    [string]$File,
    
    [switch]$Tokens,
    [switch]$Parse,
    [switch]$Help,
    [switch]$Version,
    [switch]$Force
)

$ErrorActionPreference = "Stop"

function Write-StatusMessage {
    param([string]$Message, [string]$Color = "Green")
    Write-Host "🔧 $Message" -ForegroundColor $Color
}

function Write-ErrorMessage {
    param([string]$Message)
    Write-Host "❌ $Message" -ForegroundColor Red
}

# Check if we're in the Caesar project directory
if (-not (Test-Path "CMakeLists.txt") -or -not (Test-Path "src")) {
    Write-ErrorMessage "This script must be run from the Caesar project root directory"
    exit 1
}

# Build the project
Write-StatusMessage "Building Caesar project..."
try {
    $buildResult = cmake --build build 2>&1
    if ($LASTEXITCODE -ne 0) {
        Write-ErrorMessage "Build failed!"
        Write-Host $buildResult
        exit 1
    }
    Write-StatusMessage "Build successful!"
} catch {
    Write-ErrorMessage "Build failed: $($_.Exception.Message)"
    exit 1
}

# Prepare arguments
$caesarArgs = @()

if ($Help) {
    $caesarArgs += "--help"
} elseif ($Version) {
    $caesarArgs += "--version"
} elseif ($Tokens) {
    $caesarArgs += "--tokens"
    if ($File) { $caesarArgs += $File }
} elseif ($Parse) {
    $caesarArgs += "--parse"
    if ($File) { $caesarArgs += $File }
} else {
    if ($File) {
        $caesarArgs += "--interpret"
        $caesarArgs += $File
    } else {
        $caesarArgs += "--help"
    }
}

# Run Caesar with the freshly built binary
Write-StatusMessage "Running Caesar..."
Write-Host "Command: bin\caesar.exe $($caesarArgs -join ' ')" -ForegroundColor Yellow
Write-Host ""

& "bin\caesar.exe" @caesarArgs
$exitCode = $LASTEXITCODE

if ($exitCode -eq 0) {
    Write-Host ""
    Write-StatusMessage "Execution completed successfully!"
} else {
    Write-Host ""
    Write-ErrorMessage "Caesar exited with code $exitCode"
}

exit $exitCode