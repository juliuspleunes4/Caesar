# Caesar Development Scripts

This directory contains automation scripts for Caesar Language development workflow.

## Development Scripts

### `dev-run.ps1` - Development Testing
**Purpose:** Build and run Caesar with fresh binaries every time.

**Usage:**
```powershell
scripts\dev-run.ps1 test.csr          # Build and run
scripts\dev-run.ps1 -Parse test.csr   # Build and show AST
scripts\dev-run.ps1 -Tokens test.csr  # Build and show tokens
scripts\dev-run.ps1 -Help             # Show help
```

**Benefits:**
- Always builds first, ensuring fresh binary
- Prevents testing with stale binaries
- Clean status messages and error handling

### `build-and-test.ps1` - Comprehensive Validation
**Purpose:** Complete build, test, and validation pipeline.

**Usage:**
```powershell
scripts\build-and-test.ps1           # Full validation
scripts\build-and-test.ps1 -Verbose  # With detailed output
scripts\build-and-test.ps1 -Clean    # Clean build first
```

**Features:**
- Builds the project
- Verifies binary deployment and freshness
- Runs full C++ test suite (9 tests)
- Tests enhanced data structures specifically
- Comprehensive status reporting

### `setup-build-env.ps1` - Environment Setup
**Purpose:** Configure Windows build environment with MinGW/MSYS2.

**Usage:**
```powershell
scripts\setup-build-env.ps1
```

**Features:**
- Detects MinGW/MSYS2 installation
- Configures PATH variables
- Validates build tools (GCC, CMake)
- Creates build directory structure

## Release Scripts

### `create_release.ps1` - Release Packaging
**Purpose:** Create distribution packages for Caesar releases.

**Usage:**
```powershell
scripts\create_release.ps1
```

**Features:**
- Builds optimized release binaries
- Packages Windows distribution
- Creates version archives

## System Integration Scripts

### `setup-file-association.ps1` - File Association
**Purpose:** Register .csr file association with Caesar on Windows.

**Usage:**
```powershell
scripts\setup-file-association.ps1
```

### `install.js` - NPM Post-Install
**Purpose:** Node.js script for NPM package installation.

**Usage:** (Automatic during `npm install caesar-lang`)

### `test.js` - NPM Test Runner
**Purpose:** Node.js script for testing NPM package functionality.

**Usage:**
```bash
npm test
```

## Recommended Development Workflow

### Daily Development
```powershell
# Quick testing with latest build
scripts\dev-run.ps1 myfile.csr
```

### Development Sessions
```powershell
# Start with full validation
scripts\build-and-test.ps1

# Then iterate with dev script
scripts\dev-run.ps1 test1.csr
scripts\dev-run.ps1 test2.csr
```

### Production Builds
```powershell
# Clean comprehensive build
scripts\build-and-test.ps1 -Clean -Verbose
```

### Environment Setup
```powershell
# One-time setup
scripts\setup-build-env.ps1
```

## VS Code Integration

All scripts are integrated with VS Code tasks:
- `Ctrl+Shift+P` → "Tasks: Run Task" → Select script
- Build tasks automatically call these scripts
- Development tasks use auto-build variants

## Script Dependencies

- **PowerShell 5.1+** (Windows PowerShell or PowerShell Core)
- **Node.js** (for NPM-related scripts)
- **CMake** and **MinGW/MSYS2** (for build scripts)

All scripts include error handling and clear status messages.