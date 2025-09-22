# Caesar Programming Language Installer with VS Code Extension
# Enhanced installer that includes automatic VS Code extension installation

param(
    [Parameter(HelpMessage="Installation directory (default: C:\Caesar)")]
    [string]$InstallDir = "C:\Caesar",
    
    [Parameter(HelpMessage="Skip PATH modification")]
    [switch]$SkipPath,
    
    [Parameter(HelpMessage="Skip VS Code extension installation")]
    [switch]$SkipVSCode,
    
    [Parameter(HelpMessage="Force overwrite existing installation")]
    [switch]$Force
)

# Colors for output
$ErrorColor = "Red"
$SuccessColor = "Green"
$WarningColor = "Yellow"
$InfoColor = "Cyan"

function Write-ColorOutput {
    param([string]$Message, [string]$Color = "White")
    Write-Host $Message -ForegroundColor $Color
}

function Test-VSCodeInstalled {
    """Check if VS Code is installed"""
    $vscodePaths = @(
        "${env:LOCALAPPDATA}\Programs\Microsoft VS Code\Code.exe",
        "${env:PROGRAMFILES}\Microsoft VS Code\Code.exe",
        "${env:PROGRAMFILES(X86)}\Microsoft VS Code\Code.exe"
    )
    
    foreach ($path in $vscodePaths) {
        if (Test-Path $path) {
            return $path
        }
    }
    
    # Check if 'code' command is available
    try {
        $null = Get-Command "code" -ErrorAction Stop
        return "code"
    } catch {
        return $null
    }
}

function Install-VSCodeExtension {
    """Install Caesar VS Code extension"""
    param([string]$VSCodePath)
    
    Write-ColorOutput "🔧 Installing Caesar VS Code Extension..." $InfoColor
    
    $extensionPath = Join-Path $PSScriptRoot "caesar-language-support-0.0.1.vsix"
    
    if (-not (Test-Path $extensionPath)) {
        Write-ColorOutput "❌ VS Code extension file not found: $extensionPath" $ErrorColor
        return $false
    }
    
    try {
        if ($VSCodePath -eq "code") {
            $result = & code --install-extension $extensionPath --force 2>&1
        } else {
            $result = & "$VSCodePath" --install-extension $extensionPath --force 2>&1
        }
        
        if ($LASTEXITCODE -eq 0) {
            Write-ColorOutput "✅ Caesar VS Code extension installed successfully!" $SuccessColor
            Write-ColorOutput "   • Syntax highlighting for .csr files" $InfoColor
            Write-ColorOutput "   • Code snippets and auto-completion" $InfoColor
            Write-ColorOutput "   • Caesar Dark theme" $InfoColor
            return $true
        } else {
            Write-ColorOutput "❌ Failed to install VS Code extension" $ErrorColor
            Write-ColorOutput "Error: $result" $ErrorColor
            return $false
        }
    } catch {
        Write-ColorOutput "❌ Error installing VS Code extension: $($_.Exception.Message)" $ErrorColor
        return $false
    }
}

function Install-Caesar {
    Write-ColorOutput "🏛️ Caesar Programming Language Installer" $InfoColor
    Write-ColorOutput "=======================================" $InfoColor
    
    # Check for existing installation
    if ((Test-Path $InstallDir) -and -not $Force) {
        Write-ColorOutput "⚠️  Caesar is already installed at $InstallDir" $WarningColor
        $response = Read-Host "Do you want to overwrite it? (y/N)"
        if ($response -ne 'y' -and $response -ne 'Y') {
            Write-ColorOutput "Installation cancelled." $WarningColor
            return
        }
    }
    
    # Create installation directory
    Write-ColorOutput "📁 Creating installation directory: $InstallDir" $InfoColor
    try {
        if (Test-Path $InstallDir) {
            Remove-Item $InstallDir -Recurse -Force
        }
        New-Item -ItemType Directory -Path $InstallDir -Force | Out-Null
    } catch {
        Write-ColorOutput "❌ Failed to create installation directory: $($_.Exception.Message)" $ErrorColor
        return
    }
    
    # Copy Caesar binaries
    Write-ColorOutput "📋 Installing Caesar binaries..." $InfoColor
    try {
        $binSource = Join-Path $PSScriptRoot "bin"
        $binDest = Join-Path $InstallDir "bin"
        
        if (Test-Path $binSource) {
            Copy-Item $binSource -Destination $binDest -Recurse -Force
            Write-ColorOutput "✅ Caesar binaries installed" $SuccessColor
        } else {
            Write-ColorOutput "❌ Caesar binaries not found in $binSource" $ErrorColor
            return
        }
    } catch {
        Write-ColorOutput "❌ Failed to copy binaries: $($_.Exception.Message)" $ErrorColor
        return
    }
    
    # Copy examples
    Write-ColorOutput "📚 Installing examples..." $InfoColor
    try {
        $examplesSource = Join-Path $PSScriptRoot "examples"
        $examplesDest = Join-Path $InstallDir "examples"
        
        if (Test-Path $examplesSource) {
            Copy-Item $examplesSource -Destination $examplesDest -Recurse -Force
            Write-ColorOutput "✅ Examples installed" $SuccessColor
        }
    } catch {
        Write-ColorOutput "⚠️  Could not copy examples: $($_.Exception.Message)" $WarningColor
    }
    
    # Add to PATH
    if (-not $SkipPath) {
        Write-ColorOutput "🛤️  Adding Caesar to system PATH..." $InfoColor
        try {
            $binPath = Join-Path $InstallDir "bin"
            $currentPath = [Environment]::GetEnvironmentVariable("PATH", "User")
            
            if ($currentPath -notlike "*$binPath*") {
                $newPath = "$currentPath;$binPath"
                [Environment]::SetEnvironmentVariable("PATH", $newPath, "User")
                Write-ColorOutput "✅ Caesar added to PATH" $SuccessColor
                Write-ColorOutput "   Restart your terminal to use 'caesar' command" $InfoColor
            } else {
                Write-ColorOutput "✅ Caesar already in PATH" $SuccessColor
            }
        } catch {
            Write-ColorOutput "⚠️  Could not add to PATH: $($_.Exception.Message)" $WarningColor
        }
    }
    
    # Install VS Code extension
    if (-not $SkipVSCode) {
        $vscodePath = Test-VSCodeInstalled
        if ($vscodePath) {
            Write-ColorOutput "🎨 VS Code detected, installing Caesar extension..." $InfoColor
            $extensionInstalled = Install-VSCodeExtension $vscodePath
            
            if ($extensionInstalled) {
                Write-ColorOutput "" 
                Write-ColorOutput "🎉 VS Code Integration Complete!" $SuccessColor
                Write-ColorOutput "   • Open any .csr file to see syntax highlighting" $InfoColor
                Write-ColorOutput "   • Type 'def', 'class', 'if' for code snippets" $InfoColor
                Write-ColorOutput "   • Switch to 'Caesar Dark' theme for best experience" $InfoColor
            }
        } else {
            Write-ColorOutput "ℹ️  VS Code not detected, skipping extension installation" $InfoColor
            Write-ColorOutput "   Install VS Code and run this installer again for editor support" $InfoColor
        }
    }
    
    # Installation complete
    Write-ColorOutput "" 
    Write-ColorOutput "🎉 Caesar Installation Complete!" $SuccessColor
    Write-ColorOutput "=================================" $SuccessColor
    Write-ColorOutput "📍 Installation directory: $InstallDir" $InfoColor
    Write-ColorOutput "⚡ Caesar interpreter: $InstallDir\bin\caesar.exe" $InfoColor
    Write-ColorOutput "🖥️  Caesar REPL: $InstallDir\bin\caesar_repl.exe" $InfoColor
    Write-ColorOutput "📚 Examples: $InstallDir\examples\" $InfoColor
    Write-ColorOutput "" 
    Write-ColorOutput "🚀 Quick Start:" $InfoColor
    Write-ColorOutput "   caesar $InstallDir\examples\hello_world.csr" $InfoColor
    Write-ColorOutput "   caesar_repl" $InfoColor
    Write-ColorOutput "" 
    Write-ColorOutput "🏛️ Welcome to Caesar - The High-Performance Programming Language!" $SuccessColor
}

# Run the installer
Install-Caesar