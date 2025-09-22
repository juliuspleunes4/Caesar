# Caesar Release Packaging Script
# Creates a distributable ZIP package for end users

param(
    [Parameter(HelpMessage="Version number (e.g., 1.3.2)")]
    [string]$Version = "1.3.2",
    
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

# Copy VS Code extension
Write-Host "Including VS Code extension..." -ForegroundColor Yellow

# Find the most recent .vsix file in vscode-extension directory
$VSCodeExtensionFiles = Get-ChildItem -Path "vscode-extension" -Filter "caesar-language-support-*.vsix" | 
    Sort-Object { 
        # Extract version from filename (e.g., "caesar-language-support-1.2.3.vsix" -> "1.2.3")
        $version = ($_.Name -replace '^caesar-language-support-(.+)\.vsix$', '$1')
        try {
            [System.Version]$version
        } catch {
            # If version parsing fails, use timestamp
            $_.LastWriteTime
        }
    } -Descending

if ($VSCodeExtensionFiles.Count -gt 0) {
    $LatestExtension = $VSCodeExtensionFiles[0]
    $VSCodeExtensionPath = $LatestExtension.FullName
    $ExtensionFileName = $LatestExtension.Name
    $ExtensionVersion = ($ExtensionFileName -replace '^caesar-language-support-(.+)\.vsix$', '$1')
    
    Copy-Item -Path $VSCodeExtensionPath -Destination $ReleaseDir
    Write-Host "Added VS Code extension: $ExtensionFileName" -ForegroundColor Green
    Write-Host "  Extension version: $ExtensionVersion" -ForegroundColor Cyan
} else {
    Write-Host "WARNING: No VS Code extension (.vsix) found in vscode-extension/" -ForegroundColor Yellow
    Write-Host "         Build the extension first with 'npm run package' in vscode-extension/" -ForegroundColor Yellow
    $ExtensionFileName = "caesar-language-support-0.0.1.vsix"  # Fallback for installer generation
}

# Copy Language Server
Write-Host "Including Caesar Language Server..." -ForegroundColor Yellow
$LSPDir = Join-Path $ReleaseDir "caesar-language-server"
if (Test-Path "caesar-language-server\lib") {
    # Create language server directory in release
    New-Item -ItemType Directory -Path $LSPDir -Force | Out-Null
    
    # Copy compiled language server
    Copy-Item -Path "caesar-language-server\lib" -Destination $LSPDir -Recurse -Force
    Copy-Item -Path "caesar-language-server\package.json" -Destination $LSPDir -Force
    Copy-Item -Path "caesar-language-server\node_modules" -Destination $LSPDir -Recurse -Force -ErrorAction SilentlyContinue
    
    Write-Host "Added Caesar Language Server with LSP capabilities" -ForegroundColor Green
    Write-Host "  LSP features: autocomplete, error checking, go-to-definition" -ForegroundColor Cyan
} else {
    Write-Host "WARNING: Language server not compiled. Run 'npm run compile' in caesar-language-server/" -ForegroundColor Yellow
}

# Copy File Association Scripts
Write-Host "Including file association scripts..." -ForegroundColor Yellow
$ScriptsDir = Join-Path $ReleaseDir "scripts"
if (Test-Path "scripts") {
    New-Item -ItemType Directory -Path $ScriptsDir -Force | Out-Null
    Copy-Item -Path "scripts\setup-file-association.ps1" -Destination $ScriptsDir -Force -ErrorAction SilentlyContinue
    Copy-Item -Path "scripts\install.js" -Destination $ScriptsDir -Force -ErrorAction SilentlyContinue
    Copy-Item -Path "scripts\test.js" -Destination $ScriptsDir -Force -ErrorAction SilentlyContinue
    Write-Host "Added file association and utility scripts" -ForegroundColor Green
    Write-Host "  Features: Windows file association, registry integration" -ForegroundColor Cyan
} else {
    Write-Host "WARNING: Scripts directory not found" -ForegroundColor Yellow
}

# Copy Assets (Icons)
Write-Host "Including Caesar assets..." -ForegroundColor Yellow
$AssetsDir = Join-Path $ReleaseDir "assets"
if (Test-Path "assets") {
    New-Item -ItemType Directory -Path $AssetsDir -Force | Out-Null
    Copy-Item -Path "assets\caesar-icon.ico" -Destination $AssetsDir -Force -ErrorAction SilentlyContinue
    Copy-Item -Path "assets\*.png" -Destination $AssetsDir -Force -ErrorAction SilentlyContinue
    Write-Host "Added Caesar icons and assets" -ForegroundColor Green
    Write-Host "  Features: Custom .csr file icons in Windows Explorer" -ForegroundColor Cyan
} else {
    Write-Host "WARNING: Assets directory not found" -ForegroundColor Yellow
}

# Copy documentation
Write-Host "Copying documentation..." -ForegroundColor Yellow
Copy-Item -Path "USER_GUIDE.md" -Destination $ReleaseDir -ErrorAction SilentlyContinue

# Create standalone installers
Write-Host "Creating standalone installers..." -ForegroundColor Yellow

# Create enhanced PowerShell installer with VS Code extension support
$StandalonePS1 = @"
# Caesar Programming Language Installer with VS Code Extension
# Installer that includes automatic VS Code extension installation

param(
    [Parameter(HelpMessage="Installation directory (default: C:\Caesar)")]
    [string]`$InstallDir = "C:\Caesar",
    
    [Parameter(HelpMessage="Skip PATH modification")]
    [switch]`$SkipPath,
    
    [Parameter(HelpMessage="Skip VS Code extension installation")]
    [switch]`$SkipVSCode,
    
    [Parameter(HelpMessage="Force overwrite existing installation")]
    [switch]`$Force
)

# Colors for output
`$ErrorColor = "Red"
`$SuccessColor = "Green"
`$WarningColor = "Yellow"
`$InfoColor = "Cyan"

function Write-ColorOutput {
    param([string]`$Message, [string]`$Color = "White")
    Write-Host `$Message -ForegroundColor `$Color
}

function Test-VSCodeInstalled {
    `"""Check if VS Code is installed`"""
    `$vscodePaths = @(
        "`${env:LOCALAPPDATA}\Programs\Microsoft VS Code\Code.exe",
        "`${env:PROGRAMFILES}\Microsoft VS Code\Code.exe",
        "`${env:PROGRAMFILES(X86)}\Microsoft VS Code\Code.exe"
    )
    
    foreach (`$path in `$vscodePaths) {
        if (Test-Path `$path) {
            return `$path
        }
    }
    
    # Check if 'code' command is available
    try {
        `$null = Get-Command "code" -ErrorAction Stop
        return "code"
    } catch {
        return `$null
    }
}

function Install-VSCodeExtension {
    `"""Install Caesar VS Code extension`"""
    param([string]`$VSCodePath)
    
    Write-ColorOutput "🔧 Installing Caesar VS Code Extension..." `$InfoColor
    
    `$extensionPath = Join-Path `$PSScriptRoot "$ExtensionFileName"
    
    if (-not (Test-Path `$extensionPath)) {
        Write-ColorOutput "❌ VS Code extension file not found: `$extensionPath" `$ErrorColor
        return `$false
    }
    
    try {
        if (`$VSCodePath -eq "code") {
            `$result = & code --install-extension `$extensionPath --force 2>&1
        } else {
            `$result = & "`$VSCodePath" --install-extension `$extensionPath --force 2>&1
        }
        
        if (`$LASTEXITCODE -eq 0) {
            Write-ColorOutput "✅ Caesar VS Code extension installed successfully!" `$SuccessColor
            Write-ColorOutput "   • Syntax highlighting for .csr files" `$InfoColor
            Write-ColorOutput "   • Code snippets and auto-completion" `$InfoColor
            Write-ColorOutput "   • Caesar Dark theme" `$InfoColor
            return `$true
        } else {
            Write-ColorOutput "❌ Failed to install VS Code extension" `$ErrorColor
            Write-ColorOutput "Error: `$result" `$ErrorColor
            return `$false
        }
    } catch {
        Write-ColorOutput "❌ Error installing VS Code extension: `$(`$_.Exception.Message)" `$ErrorColor
        return `$false
    }
}

function Install-Caesar {
    Write-ColorOutput "🏛️ Caesar Programming Language Installer" `$InfoColor
    Write-ColorOutput "=======================================" `$InfoColor
    
    # Check for existing installation
    if ((Test-Path `$InstallDir) -and -not `$Force) {
        Write-ColorOutput "⚠️  Caesar is already installed at `$InstallDir" `$WarningColor
        `$response = Read-Host "Do you want to overwrite it? (y/N)"
        if (`$response -ne 'y' -and `$response -ne 'Y') {
            Write-ColorOutput "Installation cancelled." `$WarningColor
            return
        }
    }
    
    # Create installation directory
    Write-ColorOutput "📁 Creating installation directory: `$InstallDir" `$InfoColor
    try {
        if (Test-Path `$InstallDir) {
            Remove-Item `$InstallDir -Recurse -Force
        }
        New-Item -ItemType Directory -Path `$InstallDir -Force | Out-Null
    } catch {
        Write-ColorOutput "❌ Failed to create installation directory: `$(`$_.Exception.Message)" `$ErrorColor
        return
    }
    
    # Copy Caesar binaries
    Write-ColorOutput "📋 Installing Caesar binaries..." `$InfoColor
    try {
        `$binSource = Join-Path `$PSScriptRoot "bin"
        `$binDest = Join-Path `$InstallDir "bin"
        
        if (Test-Path `$binSource) {
            Copy-Item `$binSource -Destination `$binDest -Recurse -Force
            Write-ColorOutput "✅ Caesar binaries installed" `$SuccessColor
        } else {
            Write-ColorOutput "❌ Caesar binaries not found in `$binSource" `$ErrorColor
            return
        }
    } catch {
        Write-ColorOutput "❌ Failed to copy binaries: `$(`$_.Exception.Message)" `$ErrorColor
        return
    }
    
    # Copy examples
    Write-ColorOutput "📚 Installing examples..." `$InfoColor
    try {
        `$examplesSource = Join-Path `$PSScriptRoot "examples"
        `$examplesDest = Join-Path `$InstallDir "examples"
        
        if (Test-Path `$examplesSource) {
            Copy-Item `$examplesSource -Destination `$examplesDest -Recurse -Force
            Write-ColorOutput "✅ Examples installed" `$SuccessColor
        }
    } catch {
        Write-ColorOutput "⚠️  Could not copy examples: `$(`$_.Exception.Message)" `$WarningColor
    }
    
    # Add to PATH
    if (-not `$SkipPath) {
        Write-ColorOutput "🛤️  Adding Caesar to system PATH..." `$InfoColor
        try {
            `$binPath = Join-Path `$InstallDir "bin"
            `$currentPath = [Environment]::GetEnvironmentVariable("PATH", "User")
            
            if (`$currentPath -notlike "*`$binPath*") {
                `$newPath = "`$currentPath;`$binPath"
                [Environment]::SetEnvironmentVariable("PATH", `$newPath, "User")
                Write-ColorOutput "✅ Caesar added to PATH" `$SuccessColor
                Write-ColorOutput "   Restart your terminal to use 'caesar' command" `$InfoColor
            } else {
                Write-ColorOutput "✅ Caesar already in PATH" `$SuccessColor
            }
        } catch {
            Write-ColorOutput "⚠️  Could not add to PATH: `$(`$_.Exception.Message)" `$WarningColor
        }
    }
    
    # Install VS Code extension
    if (-not `$SkipVSCode) {
        `$vscodePath = Test-VSCodeInstalled
        if (`$vscodePath) {
            Write-ColorOutput "🎨 VS Code detected, installing Caesar extension..." `$InfoColor
            `$extensionInstalled = Install-VSCodeExtension `$vscodePath
            
            if (`$extensionInstalled) {
                Write-ColorOutput "" 
                Write-ColorOutput "🎉 VS Code Integration Complete!" `$SuccessColor
                Write-ColorOutput "   • Open any .csr file to see syntax highlighting" `$InfoColor
                Write-ColorOutput "   • Type 'def', 'class', 'if' for code snippets" `$InfoColor
                Write-ColorOutput "   • Switch to 'Caesar Dark' theme for best experience" `$InfoColor
            }
        } else {
            Write-ColorOutput "ℹ️  VS Code not detected, skipping extension installation" `$InfoColor
            Write-ColorOutput "   Install VS Code and run this installer again for editor support" `$InfoColor
        }
    }
    
    # Installation complete
    Write-ColorOutput "" 
    Write-ColorOutput "🎉 Caesar Installation Complete!" `$SuccessColor
    Write-ColorOutput "=================================" `$SuccessColor
    Write-ColorOutput "📍 Installation directory: `$InstallDir" `$InfoColor
    Write-ColorOutput "⚡ Caesar interpreter: `$InstallDir\bin\caesar.exe" `$InfoColor
    Write-ColorOutput "🖥️  Caesar REPL: `$InstallDir\bin\caesar_repl.exe" `$InfoColor
    Write-ColorOutput "📚 Examples: `$InstallDir\examples\" `$InfoColor
    Write-ColorOutput "" 
    Write-ColorOutput "🚀 Quick Start:" `$InfoColor
    Write-ColorOutput "   caesar `$InstallDir\examples\hello_world.csr" `$InfoColor
    Write-ColorOutput "   caesar_repl" `$InfoColor
    Write-ColorOutput "" 
    Write-ColorOutput "🏛️ Welcome to Caesar - The High-Performance Programming Language!" `$SuccessColor
}

# Run the installer
Install-Caesar
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
- ``install.ps1`` - Enhanced PowerShell installer with VS Code extension
- ``install.bat`` - Batch installer
- ``$ExtensionFileName`` - VS Code extension for syntax highlighting
- ``USER_GUIDE.md`` - Complete documentation (if included)

## VS Code Integration

The installer automatically detects and installs the Caesar VS Code extension if VS Code is found on your system. This provides:

- **Syntax highlighting** for ``.csr`` files
- **Code snippets** for common Caesar patterns
- **Caesar Dark theme** optimized for Caesar development
- **Language recognition** in the VS Code ecosystem

To manually install the extension: ``code --install-extension $ExtensionFileName``

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