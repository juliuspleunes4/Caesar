# Caesar Installation with File Association
# Enhanced installer that includes Windows file association

param(
    [Parameter(HelpMessage="Skip file association setup")]
    [switch]$SkipFileAssociation = $false
)

Write-Host "======================================" -ForegroundColor Cyan
Write-Host "   Caesar Language Installer" -ForegroundColor Cyan
Write-Host "======================================" -ForegroundColor Cyan
Write-Host ""

# Get the installation directory
$InstallDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$BinDir = Join-Path $InstallDir "bin"

Write-Host "Installing Caesar Language..." -ForegroundColor Yellow
Write-Host "Installation directory: $InstallDir" -ForegroundColor Cyan

# Check if executables exist
$CaesarExe = Join-Path $BinDir "caesar.exe"
$CaesarRepl = Join-Path $BinDir "caesar_repl.exe"

if (-not (Test-Path $CaesarExe)) {
    Write-Error "Caesar executable not found: $CaesarExe"
    Write-Host "Please ensure you have the complete Caesar package." -ForegroundColor Yellow
    exit 1
}

# Add to PATH (user level)
Write-Host "Adding Caesar to PATH..." -ForegroundColor Yellow

$CurrentPath = [Environment]::GetEnvironmentVariable("Path", "User")
if ($CurrentPath -notlike "*$BinDir*") {
    $NewPath = "$CurrentPath;$BinDir"
    [Environment]::SetEnvironmentVariable("Path", $NewPath, "User")
    Write-Host "✅ Added $BinDir to user PATH" -ForegroundColor Green
    Write-Host "   Please restart your terminal to use 'caesar' command" -ForegroundColor Yellow
} else {
    Write-Host "✅ Caesar already in PATH" -ForegroundColor Green
}

# Set up file association
if (-not $SkipFileAssociation) {
    Write-Host ""
    Write-Host "Setting up file association..." -ForegroundColor Yellow
    
    $AssociationScript = Join-Path $InstallDir "scripts\setup-file-association.ps1"
    if (Test-Path $AssociationScript) {
        try {
            & $AssociationScript -CaesarPath $BinDir
            Write-Host "✅ File association configured" -ForegroundColor Green
        } catch {
            Write-Warning "File association setup failed: $($_.Exception.Message)"
            Write-Host "You can run it manually later: .\scripts\setup-file-association.ps1" -ForegroundColor Yellow
        }
    } else {
        Write-Warning "File association script not found"
    }
}

Write-Host ""
Write-Host "🎉 Caesar installation complete!" -ForegroundColor Green
Write-Host ""
Write-Host "What's been set up:" -ForegroundColor Yellow
Write-Host "✓ Caesar executables available" -ForegroundColor White
Write-Host "✓ Added to system PATH" -ForegroundColor White
if (-not $SkipFileAssociation) {
    Write-Host "✓ File association for .csr files" -ForegroundColor White
    Write-Host "✓ Context menu 'Run with Caesar'" -ForegroundColor White
    Write-Host "✓ Custom Caesar icon" -ForegroundColor White
}
Write-Host ""
Write-Host "Next steps:" -ForegroundColor Yellow
Write-Host "1. Restart your terminal" -ForegroundColor White
Write-Host "2. Test: caesar --version" -ForegroundColor White
Write-Host "3. Create a .csr file and double-click it" -ForegroundColor White
Write-Host "4. Install VS Code extension: 'Caesar Language Support'" -ForegroundColor White
Write-Host ""
Write-Host "Documentation: https://github.com/juliuspleunes4/Caesar" -ForegroundColor Cyan