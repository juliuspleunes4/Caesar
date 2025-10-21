# Caesar Installation & Upgrade Behavior

## Overview

This document describes how Caesar handles installation and upgrades to prevent conflicts when users install a newer version over an existing installation.

## Installation Methods

Caesar provides two installation methods:
1. **PowerShell Installer** (`install.ps1`) - Recommended, with VS Code extension support
2. **Batch Installer** (`install.bat`) - Simple fallback for older systems

---

## ✅ Upgrade Safety Features

### 1. Complete Version Removal
Both installers **completely remove** the old version before installing the new one:

**PowerShell Installer:**
```powershell
if (Test-Path $InstallDir) {
    Remove-Item $InstallDir -Recurse -Force
}
```

**Batch Installer:**
```batch
if exist "%INSTALL_DIR%" (
    rmdir /s /q "%INSTALL_DIR%"
)
```

**Result:** No conflicts between versions - old version is completely gone before new one installs.

---

### 2. PATH Deduplication
Both installers check if Caesar is already in PATH before adding it:

**PowerShell Installer:**
```powershell
if ($currentPath -notlike "*$binPath*") {
    $newPath = "$currentPath;$binPath"
    [Environment]::SetEnvironmentVariable("PATH", $newPath, "User")
} else {
    Write-ColorOutput "✅ Caesar already in PATH"
}
```

**Batch Installer:**
```batch
echo %PATH% | findstr /C:"%BIN_PATH%" > nul
if %errorlevel% equ 0 (
    echo + Caesar already in PATH
) else (
    setx PATH "%PATH%;%BIN_PATH%"
)
```

**Result:** No PATH pollution - Caesar bin directory is only added once, even after multiple upgrades.

---

### 3. VS Code Extension Cleanup
The PowerShell installer automatically removes old extension versions before installing new ones:

```powershell
function Uninstall-OldVSCodeExtension {
    # Checks for existing extension
    $installed = & code --list-extensions | Select-String "juliuspleunes4.caesar-language-support"
    
    if ($installed) {
        # Removes old version
        & code --uninstall-extension juliuspleunes4.caesar-language-support
    }
}
```

**Result:** No duplicate extensions - only the latest version is installed.

---

### 4. Version Detection
The PowerShell installer detects the existing version and displays upgrade information:

```powershell
function Get-InstalledVersion {
    $caesarExe = Join-Path $InstallPath "bin\caesar.exe"
    if (Test-Path $caesarExe) {
        $versionOutput = & $caesarExe --version 2>&1
        if ($versionOutput -match "v?(\d+\.\d+\.\d+)") {
            return $Matches[1]
        }
    }
    return $null
}
```

**Output Example:**
```
📦 Detected existing Caesar v1.4.0
🆕 Installing Caesar v1.5.0
```

**Result:** Users know exactly what's being upgraded.

---

### 5. User Confirmation
The PowerShell installer asks for confirmation before upgrading (unless `-Force` flag is used):

```
📦 Detected existing Caesar v1.4.0
🆕 Installing Caesar v1.5.0

Do you want to continue? (y/N)
```

**Options:**
- Type `y` or `Y` to proceed with upgrade
- Type `N` or press Enter to cancel

**Force Install:**
```powershell
.\install.ps1 -Force
```
Skips confirmation and automatically upgrades.

---

## Upgrade Process Flow

### PowerShell Installer (`install.ps1`)

```
1. Display installer header with version
2. Check if Caesar already installed
   ├─ Yes: Detect installed version
   │   ├─ Show version comparison (old vs new)
   │   └─ Ask for confirmation
   └─ No: Proceed directly
3. Remove old installation completely
4. Create fresh installation directory
5. Copy new binaries
6. Copy examples
7. Add to PATH (only if not already present)
8. Detect VS Code
   ├─ Found: Uninstall old extension → Install new extension
   └─ Not found: Skip extension
9. Display completion message
   ├─ Upgrade: "Upgraded from vX.X.X to vY.Y.Y"
   └─ New install: "Welcome to Caesar"
```

### Batch Installer (`install.bat`)

```
1. Display installer header
2. Validate executables exist in package
3. Check if Caesar already installed
   └─ Yes: Remove silently (no confirmation)
4. Create fresh installation directory
5. Copy binaries
6. Copy examples
7. Add to PATH (only if not already present)
8. Display completion message
```

---

## Installation Directory Structure

**Default Installation Path:** `C:\Caesar`

**Structure:**
```
C:\Caesar\
├── bin\
│   ├── caesar.exe
│   ├── caesar_repl.exe
│   ├── libgcc_s_seh-1.dll
│   ├── libstdc++-6.dll
│   └── libwinpthread-1.dll
└── examples\
    ├── hello_world.csr
    ├── fibonacci.csr
    ├── functions.csr
    └── control_flow.csr
```

**Custom Installation:**
```powershell
.\install.ps1 -InstallDir "D:\MyTools\Caesar"
```

---

## Potential Issues & Solutions

### Issue 1: Multiple Versions Due to Different Install Paths
**Scenario:** User installs v1.4.0 to `C:\Caesar`, then v1.5.0 to `D:\Caesar`

**Result:** Both versions exist (different directories)

**Solution:** Always use the same installation directory, or manually remove old installation

---

### Issue 2: Caesar Command Not Found After Upgrade
**Cause:** Terminal hasn't refreshed PATH environment variable

**Solutions:**
1. Close and reopen terminal
2. Run `refreshenv` (if Chocolatey installed)
3. Log out and log back in to Windows
4. Use full path: `C:\Caesar\bin\caesar.exe`

---

### Issue 3: Old VS Code Extension Still Active
**Cause:** VS Code caches extensions

**Solution:**
1. Close all VS Code instances
2. Rerun installer: `.\install.ps1`
3. Or manually: `code --uninstall-extension juliuspleunes4.caesar-language-support`
4. Then: `code --install-extension caesar-language-support-X.X.X.vsix`

---

### Issue 4: Permission Denied During Upgrade
**Cause:** Caesar is currently running or files are locked

**Solution:**
1. Close all Caesar processes:
   - Exit any running `caesar.exe`
   - Exit any running `caesar_repl.exe`
   - Close terminals running Caesar programs
2. Run installer as Administrator (right-click → "Run as Administrator")

---

## Testing Upgrades

### Test Scenario 1: Clean Upgrade
```powershell
# Install v1.4.0
.\caesar-v1.4.0\install.ps1

# Verify installation
caesar --version  # Should show v1.4.0

# Install v1.5.0
.\caesar-v1.5.0\install.ps1

# Verify upgrade
caesar --version  # Should show v1.5.0

# Check PATH (should only appear once)
$env:PATH -split ';' | Select-String "Caesar"
```

### Test Scenario 2: Force Upgrade
```powershell
# Install with force flag (no confirmation)
.\install.ps1 -Force
```

### Test Scenario 3: Custom Directory Upgrade
```powershell
# Install v1.4.0 to custom location
.\install.ps1 -InstallDir "D:\MyApps\Caesar"

# Upgrade v1.5.0 to same location
.\install.ps1 -InstallDir "D:\MyApps\Caesar"
```

### Test Scenario 4: VS Code Extension Upgrade
```powershell
# Install v1.4.0 (includes extension 0.0.3)
.\install.ps1

# Check installed extensions
code --list-extensions | Select-String "caesar"

# Install v1.5.0 (includes extension 0.0.4)
.\install.ps1

# Verify only new extension installed
code --list-extensions | Select-String "caesar"
```

---

## Best Practices for Users

### ✅ Recommended Upgrade Process

1. **Download the latest release** from [GitHub Releases](https://github.com/juliuspleunes4/Caesar/releases)
2. **Extract the ZIP file** to a temporary location
3. **Close all Caesar instances** (IDE, terminals, REPL)
4. **Run the installer**:
   ```powershell
   .\install.ps1
   ```
5. **Restart your terminal** to refresh PATH
6. **Verify the upgrade**:
   ```powershell
   caesar --version
   ```

### ⚠️ Things to Avoid

- ❌ Don't install different versions to different directories (causes PATH confusion)
- ❌ Don't manually copy files over existing installation (use the installer)
- ❌ Don't delete `C:\Caesar` manually while programs are running
- ❌ Don't modify the installer scripts (they ensure proper upgrade behavior)

---

## Uninstallation

To completely remove Caesar:

1. **Remove installation directory:**
   ```powershell
   Remove-Item -Recurse -Force C:\Caesar
   ```

2. **Remove from PATH:**
   ```powershell
   $path = [Environment]::GetEnvironmentVariable("PATH", "User")
   $newPath = ($path -split ';' | Where-Object { $_ -notlike "*Caesar*" }) -join ';'
   [Environment]::SetEnvironmentVariable("PATH", $newPath, "User")
   ```

3. **Uninstall VS Code extension:**
   ```powershell
   code --uninstall-extension juliuspleunes4.caesar-language-support
   ```

---

## Version History

| Version | Changes |
|---------|---------|
| v1.5.0  | Added upgrade detection, version display, VS Code extension cleanup |
| v1.4.0  | Initial upgrade-safe implementation with PATH deduplication |
| v1.3.0  | Basic installer without upgrade handling |

---

## Summary

✅ **Safe Upgrades:** Old version completely removed before new install
✅ **No Conflicts:** Only one version exists at a time
✅ **Clean PATH:** No duplicate entries after multiple upgrades
✅ **VS Code Integration:** Old extensions automatically removed
✅ **User Friendly:** Clear version detection and confirmation prompts
✅ **Idempotent:** Running installer multiple times is safe

**Conclusion:** Caesar's upgrade process is designed to be **conflict-free, user-friendly, and completely safe** for users upgrading from any previous version.
