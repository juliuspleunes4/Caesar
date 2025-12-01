# Caesar Development Workflow Guide

## Preventing the "Wrong Binary" Issue

This document explains how to prevent testing with outdated binaries during Caesar development.

## The Problem

During development, you might encounter a situation where:
- You build changes to the Caesar interpreter
- Tests with the old binary in `bin/` still show old behavior
- The new binary is actually in `build/src/` but you're testing the wrong one

## Solutions Implemented

### 1. Automatic Binary Copying

**File:** `src/CMakeLists.txt`

The build system now automatically copies binaries to `bin/` after each build:

```cmake
# Auto-copy binaries to bin directory after build
add_custom_command(TARGET caesar POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:caesar> ${CMAKE_SOURCE_DIR}/bin/caesar.exe
    COMMENT "Copying caesar.exe to bin directory"
)
```

**Result:** Every `cmake --build build` automatically updates `bin/caesar.exe`

### 2. Development Scripts

#### `scripts/dev-run.ps1` - Always Fresh Development Testing

**Usage:**
```powershell
scripts\dev-run.ps1 test.csr          # Build and run
scripts\dev-run.ps1 -Parse test.csr   # Build and show AST  
scripts\dev-run.ps1 -Tokens test.csr  # Build and show tokens
scripts\dev-run.ps1 -Help             # Show help
```

**Benefits:**
- Always builds first, ensuring fresh binary
- Uses the correct `bin/caesar.exe` automatically
- Clear status messages and error handling
- No chance of testing stale binaries

#### `scripts/build-and-test.ps1` - Comprehensive Validation

**Usage:**
```powershell
scripts\build-and-test.ps1           # Full build, test, and validation
scripts\build-and-test.ps1 -Verbose  # With detailed output
scripts\build-and-test.ps1 -Clean    # Clean build first
```

**Features:**
- Builds the project
- Verifies binary deployment and freshness
- Runs full test suite (9 tests)
- Tests enhanced data structures specifically
- Comprehensive status reporting

### 3. VS Code Tasks

**File:** `.vscode/tasks.json`

New tasks added:
- **"Dev Run (Auto-Build)"** - Build and run with file prompt
- **"Dev Parse (Auto-Build)"** - Build and show AST with file prompt

**Usage:** `Ctrl+Shift+P` → "Tasks: Run Task" → Select task

### 4. Binary Version Information

**Enhancement:** `src/main.cpp`

The `--version` flag now shows build timestamp:

```
Caesar Programming Language v1.5.1
...
✓ Enhanced data structures (lists and dictionaries)
Build date: Dec 01 2025 09:43:31
```

**Usage:** `bin\caesar.exe --version` to verify you have the latest build

## Recommended Development Workflow

### For Quick Testing
```powershell
# Best practice - always use dev script
scripts\dev-run.ps1 myfile.csr
```

### For Development Sessions
```powershell
# Start with comprehensive validation
scripts\build-and-test.ps1

# Then use dev script for iterations
scripts\dev-run.ps1 test1.csr
scripts\dev-run.ps1 test2.csr
# etc.
```

### For Production Builds
```powershell
# Clean build with full validation
scripts\build-and-test.ps1 -Clean -Verbose
```

## VS Code Integration

1. **Quick Run:** Use `Ctrl+Shift+P` → "Tasks: Run Task" → "Dev Run (Auto-Build)"
2. **Quick Parse:** Use `Ctrl+Shift+P` → "Tasks: Run Task" → "Dev Parse (Auto-Build)"
3. **Build:** Use `Ctrl+Shift+P` → "Tasks: Run Task" → "Build Caesar"

## Verification Commands

To verify everything is working:

```powershell
# Check binary version and build date
bin\caesar.exe --version

# Quick functionality test
echo 'print([1, 2, 3])' | bin\caesar.exe --interpret /dev/stdin

# Or use a test file
bin\caesar.exe --interpret test_dict.csr
```

## Troubleshooting

If you still encounter issues:

1. **Force rebuild and copy:**
   ```powershell
   scripts\build-and-test.ps1 -Clean
   ```

2. **Check binary timestamps:**
   ```powershell
   Get-Item build\src\caesar.exe, bin\caesar.exe | Select Name, LastWriteTime
   ```

3. **Manual copy if needed:**
   ```powershell
   Copy-Item build\src\caesar.exe bin\caesar.exe -Force
   ```

## Key Files Modified

- `src/CMakeLists.txt` - Automatic binary copying
- `scripts/dev-run.ps1` - Development testing script  
- `scripts/build-and-test.ps1` - Comprehensive build script
- `.vscode/tasks.json` - VS Code task integration
- `src/main.cpp` - Version information enhancement

## Summary

With these changes, the "wrong binary" issue is **eliminated**:

✅ **Automatic:** Binaries copy automatically after build  
✅ **Convenient:** Dev scripts handle build + test in one command  
✅ **Verified:** Build script validates binary freshness  
✅ **Integrated:** VS Code tasks provide GUI access  
✅ **Traceable:** Version timestamps show build freshness  

**Result:** Always test with the latest code, never with stale binaries!