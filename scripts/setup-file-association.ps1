# Caesar File Association Setup Script
# Sets up Windows file association for .csr files

param(
    [Parameter(HelpMessage="Caesar installation path")]
    [string]$CaesarPath = "",
    
    [Parameter(HelpMessage="Remove file association instead of adding it")]
    [switch]$Remove = $false
)

# Check if running as administrator
function Test-Administrator {
    $currentUser = [Security.Principal.WindowsIdentity]::GetCurrent()
    $principal = New-Object Security.Principal.WindowsPrincipal($currentUser)
    return $principal.IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)
}

# Find Caesar installation
function Find-CaesarInstallation {
    # Try to find caesar in PATH
    $caesarInPath = Get-Command caesar -ErrorAction SilentlyContinue
    if ($caesarInPath) {
        $caesarExe = $caesarInPath.Source
        if ($caesarExe -match "caesar\.cmd$") {
            # This is the NPM global installation
            $npmPath = Split-Path $caesarExe -Parent
            $nodePath = Join-Path $npmPath "node_modules\caesar-lang\bin\caesar.exe"
            if (Test-Path $nodePath) {
                return Split-Path $nodePath -Parent
            }
        } elseif ($caesarExe -match "caesar\.exe$") {
            return Split-Path $caesarExe -Parent
        }
    }
    
    # Try common installation paths
    $commonPaths = @(
        "C:\Program Files\Caesar",
        "C:\Caesar",
        "$env:LOCALAPPDATA\Caesar",
        "$env:APPDATA\npm\node_modules\caesar-lang\bin"
    )
    
    foreach ($path in $commonPaths) {
        $caesarExe = Join-Path $path "caesar.exe"
        if (Test-Path $caesarExe) {
            return $path
        }
    }
    
    return $null
}

# Find VS Code installation
function Find-VSCodeInstallation {
    # Try to find code in PATH
    $vscodeInPath = Get-Command code -ErrorAction SilentlyContinue
    if ($vscodeInPath) {
        return $vscodeInPath.Source
    }
    
    # Try common installation paths
    $commonPaths = @(
        "$env:LOCALAPPDATA\Programs\Microsoft VS Code\Code.exe",
        "$env:PROGRAMFILES\Microsoft VS Code\Code.exe",
        "${env:PROGRAMFILES(X86)}\Microsoft VS Code\Code.exe"
    )
    
    foreach ($path in $commonPaths) {
        if (Test-Path $path) {
            return $path
        }
    }
    
    return "notepad.exe"  # Fallback to notepad
}

# Create registry entries
function Set-CaesarFileAssociation {
    param(
        [string]$CaesarBinPath,
        [string]$VSCodePath
    )
    
    Write-Host "Setting up Caesar file association..." -ForegroundColor Yellow
    Write-Host "  Caesar path: $CaesarBinPath" -ForegroundColor Cyan
    Write-Host "  Editor path: $VSCodePath" -ForegroundColor Cyan
    
    try {
        # Register .csr extension
        New-Item -Path "HKCU:\Software\Classes\.csr" -Force | Out-Null
        Set-ItemProperty -Path "HKCU:\Software\Classes\.csr" -Name "(Default)" -Value "CaesarSourceFile"
        Set-ItemProperty -Path "HKCU:\Software\Classes\.csr" -Name "Content Type" -Value "text/plain"
        
        # Define Caesar source file type
        New-Item -Path "HKCU:\Software\Classes\CaesarSourceFile" -Force | Out-Null
        Set-ItemProperty -Path "HKCU:\Software\Classes\CaesarSourceFile" -Name "(Default)" -Value "Caesar Source File"
        
        # Set default icon
        New-Item -Path "HKCU:\Software\Classes\CaesarSourceFile\DefaultIcon" -Force | Out-Null
        
        # Try to find the Caesar ICO file first, fallback to executable
        $iconPath = $null
        
        # Look for ICO file in various locations
        $possibleIconPaths = @(
            (Join-Path (Split-Path $CaesarBinPath -Parent) "assets\caesar-icon.ico"),
            (Join-Path $CaesarBinPath "..\assets\caesar-icon.ico"),
            (Join-Path (Split-Path $CaesarBinPath -Parent) "caesar-icon.ico")
        )
        
        foreach ($path in $possibleIconPaths) {
            if (Test-Path $path) {
                $iconPath = $path
                Write-Host "  Using Caesar ICO: $iconPath" -ForegroundColor Green
                break
            }
        }
        
        # Fallback to executable icon
        if (-not $iconPath) {
            $iconPath = Join-Path $CaesarBinPath "caesar.exe"
            Write-Host "  Using executable icon: $iconPath" -ForegroundColor Yellow
            Set-ItemProperty -Path "HKCU:\Software\Classes\CaesarSourceFile\DefaultIcon" -Name "(Default)" -Value "`"$iconPath`",0"
        } else {
            Set-ItemProperty -Path "HKCU:\Software\Classes\CaesarSourceFile\DefaultIcon" -Name "(Default)" -Value "`"$iconPath`""
        }
        
        # Shell commands
        New-Item -Path "HKCU:\Software\Classes\CaesarSourceFile\shell" -Force | Out-Null
        Set-ItemProperty -Path "HKCU:\Software\Classes\CaesarSourceFile\shell" -Name "(Default)" -Value "open"
        
        # Open with editor
        New-Item -Path "HKCU:\Software\Classes\CaesarSourceFile\shell\open" -Force | Out-Null
        New-Item -Path "HKCU:\Software\Classes\CaesarSourceFile\shell\open\command" -Force | Out-Null
        Set-ItemProperty -Path "HKCU:\Software\Classes\CaesarSourceFile\shell\open" -Name "(Default)" -Value "&Edit"
        Set-ItemProperty -Path "HKCU:\Software\Classes\CaesarSourceFile\shell\open\command" -Name "(Default)" -Value "`"$VSCodePath`" `"%1`""
        
        # Run with Caesar
        New-Item -Path "HKCU:\Software\Classes\CaesarSourceFile\shell\run" -Force | Out-Null
        New-Item -Path "HKCU:\Software\Classes\CaesarSourceFile\shell\run\command" -Force | Out-Null
        Set-ItemProperty -Path "HKCU:\Software\Classes\CaesarSourceFile\shell\run" -Name "(Default)" -Value "&Run with Caesar"
        $caesarExe = Join-Path $CaesarBinPath "caesar.exe"
        Set-ItemProperty -Path "HKCU:\Software\Classes\CaesarSourceFile\shell\run\command" -Name "(Default)" -Value "`"$caesarExe`" --interpret `"%1`""
        
        # Open with Caesar REPL
        New-Item -Path "HKCU:\Software\Classes\CaesarSourceFile\shell\repl" -Force | Out-Null
        New-Item -Path "HKCU:\Software\Classes\CaesarSourceFile\shell\repl\command" -Force | Out-Null
        Set-ItemProperty -Path "HKCU:\Software\Classes\CaesarSourceFile\shell\repl" -Name "(Default)" -Value "Open in Caesar &REPL"
        $caesarRepl = Join-Path $CaesarBinPath "caesar_repl.exe"
        Set-ItemProperty -Path "HKCU:\Software\Classes\CaesarSourceFile\shell\repl\command" -Name "(Default)" -Value "`"$caesarRepl`""
        
        Write-Host "✅ Caesar file association created successfully!" -ForegroundColor Green
        Write-Host "" 
        Write-Host "Features enabled:" -ForegroundColor Yellow
        Write-Host "  • Double-click .csr files to edit" -ForegroundColor White
        Write-Host "  • Right-click → 'Run with Caesar'" -ForegroundColor White
        Write-Host "  • Right-click → 'Open in Caesar REPL'" -ForegroundColor White
        Write-Host "  • Custom Caesar icon in File Explorer" -ForegroundColor White
        
    } catch {
        Write-Error "Failed to create file association: $($_.Exception.Message)"
        return $false
    }
    
    return $true
}

# Remove registry entries
function Remove-CaesarFileAssociation {
    Write-Host "Removing Caesar file association..." -ForegroundColor Yellow
    
    try {
        # Remove the registry entries
        Remove-Item -Path "HKCU:\Software\Classes\.csr" -Recurse -Force -ErrorAction SilentlyContinue
        Remove-Item -Path "HKCU:\Software\Classes\CaesarSourceFile" -Recurse -Force -ErrorAction SilentlyContinue
        
        Write-Host "✅ Caesar file association removed successfully!" -ForegroundColor Green
        
    } catch {
        Write-Error "Failed to remove file association: $($_.Exception.Message)"
        return $false
    }
    
    return $true
}

# Refresh file associations
function Update-FileAssociations {
    # Notify Windows that file associations have changed
    Add-Type -TypeDefinition @"
        using System;
        using System.Runtime.InteropServices;
        public class Shell32 {
            [DllImport("shell32.dll", CharSet = CharSet.Auto, SetLastError = true)]
            public static extern void SHChangeNotify(uint wEventId, uint uFlags, IntPtr dwItem1, IntPtr dwItem2);
        }
"@
    
    [Shell32]::SHChangeNotify(0x08000000, 0x0000, [IntPtr]::Zero, [IntPtr]::Zero)
}

# Main execution
Write-Host "======================================" -ForegroundColor Cyan
Write-Host "   Caesar File Association Setup" -ForegroundColor Cyan
Write-Host "======================================" -ForegroundColor Cyan
Write-Host ""

if ($Remove) {
    $success = Remove-CaesarFileAssociation
    if ($success) {
        Update-FileAssociations
        Write-Host ""
        Write-Host "File association removed. You may need to restart File Explorer." -ForegroundColor Yellow
    }
    exit
}

# Find Caesar installation
if (-not $CaesarPath) {
    $CaesarPath = Find-CaesarInstallation
}

if (-not $CaesarPath) {
    Write-Error "Could not find Caesar installation. Please specify -CaesarPath parameter."
    Write-Host ""
    Write-Host "Usage examples:" -ForegroundColor Yellow
    Write-Host "  .\setup-file-association.ps1 -CaesarPath 'C:\Program Files\Caesar\bin'" -ForegroundColor White
    Write-Host "  .\setup-file-association.ps1 -Remove" -ForegroundColor White
    exit 1
}

if (-not (Test-Path (Join-Path $CaesarPath "caesar.exe"))) {
    Write-Error "Caesar executable not found at: $CaesarPath\caesar.exe"
    exit 1
}

# Find VS Code
$VSCodePath = Find-VSCodeInstallation

# Create file association
$success = Set-CaesarFileAssociation -CaesarBinPath $CaesarPath -VSCodePath $VSCodePath

if ($success) {
    Update-FileAssociations
    
    Write-Host ""
    Write-Host "🎉 Setup complete!" -ForegroundColor Green
    Write-Host ""
    Write-Host "Next steps:" -ForegroundColor Yellow
    Write-Host "1. Create a test file: echo 'print \"Hello, World!\"' > test.csr" -ForegroundColor White
    Write-Host "2. Double-click test.csr in File Explorer" -ForegroundColor White
    Write-Host "3. Right-click test.csr → 'Run with Caesar'" -ForegroundColor White
    Write-Host ""
    Write-Host "Note: You may need to restart File Explorer to see the new icon." -ForegroundColor Yellow
}