# Troubleshooting Guide

This guide helps you resolve common issues when building, installing, or using Caesar Programming Language.

## Table of Contents
- [Installation Issues](#installation-issues)
- [Build Problems](#build-problems)
- [Runtime Errors](#runtime-errors)
- [Platform-Specific Issues](#platform-specific-issues)
- [Getting Help](#getting-help)

## Installation Issues

### "caesar is not recognized as a command"

**Symptoms:** After installation, running `caesar` in terminal shows "command not found" or "not recognized"

**Solutions:**
1. **Restart your terminal** - PATH changes require a fresh terminal session
2. **Check PATH manually:**
   ```bash
   echo $env:PATH  # Windows PowerShell
   echo $PATH      # Linux/macOS
   ```
3. **Reinstall with admin privileges:**
   ```bash
   # Run as Administrator
   powershell -ExecutionPolicy Bypass -File install.ps1
   ```
4. **Manual PATH setup:**
   - Windows: Add `C:\Caesar\bin` to System Environment Variables
   - Linux/macOS: Add `export PATH=$PATH:/path/to/caesar/bin` to your shell profile

### Installation Script Fails

**"ERROR: Build directory not found!"**
- Solution: Run `cmake --build build` before installing
- Alternative: Use the standalone release package instead

**"Permission denied" errors**
- Solution: Run installer as Administrator (Windows) or with `sudo` (Linux/macOS)
- Alternative: Install to user directory with `-InstallDir $HOME/Caesar`

**"PowerShell execution policy" errors**
- Solution: Use `powershell -ExecutionPolicy Bypass -File install.ps1`
- Alternative: Change execution policy: `Set-ExecutionPolicy RemoteSigned`

## Build Problems

### CMake Configuration Issues

**"CMake not found"**
```bash
# Install CMake
# Ubuntu/Debian:
sudo apt-get install cmake

# Windows:
# Download from https://cmake.org/download/

# macOS:
brew install cmake
```

**"C++ compiler not found"**
```bash
# Ubuntu/Debian:
sudo apt-get install build-essential

# Windows:
# Install Visual Studio 2019+ or MinGW-w64

# macOS:
xcode-select --install
```

**CMake version too old**
- Requirement: CMake 3.15+
- Solution: Update CMake from official website

### Compilation Errors

**"C++17 features not supported"**
- Check compiler version:
  ```bash
  gcc --version    # Should be 8.0+
  clang --version  # Should be 9.0+
  ```
- Update compiler or use newer version

**Linking errors with tests**
- Clean and rebuild:
  ```bash
  rm -rf build
  mkdir build && cd build
  cmake .. && cmake --build .
  ```

**Missing header files**
- Ensure all submodules are initialized:
  ```bash
  git submodule update --init --recursive
  ```

### Test Failures

**Tests fail to run**
```bash
# Run tests with verbose output
ctest --output-on-failure --verbose

# Run specific test
./build/tests/test_lexer
```

**Specific test failures**
- Check test output for specific error messages
- Verify input files exist in `tests/` directory
- Ensure build is up-to-date: `cmake --build build`

## Runtime Errors

### Parser Errors

**"Unexpected token" errors**
- Check file encoding (should be UTF-8)
- Verify indentation consistency (use spaces, not tabs)
- Check for missing colons after `if`, `def`, `class` statements

**"Expected indented block"**
- Ensure proper indentation after `:` statements
- Check for empty lines within indented blocks
- Verify consistent indentation level (4 spaces recommended)

### Runtime Errors

**"Undefined variable" errors**
- Check variable spelling and scope
- Ensure variables are defined before use
- Verify function parameters match function calls

**"Unsupported binary operation"**
- Check data types in operations (e.g., can't add string + number)
- Use type conversion functions: `str()`, `int()`, `float()`

**Function call errors**
- Verify function exists and is spelled correctly
- Check parameter count matches function definition
- Ensure function is defined before calling

### File Handling Issues

**"File not found" errors**
- Use absolute paths or ensure working directory is correct
- Check file permissions
- Verify file extension is `.csr`

## Platform-Specific Issues

### Windows Issues

**Antivirus blocking executables**
- Add Caesar installation directory to antivirus exceptions
- Some antivirus software blocks newly compiled executables

**Long path issues**
- Enable long path support in Windows 10+
- Or install to shorter path like `C:\Caesar`

**PowerShell vs Command Prompt**
- Use PowerShell for best experience
- Some features may not work properly in old Command Prompt

### Linux Issues

**Permission denied on executables**
```bash
chmod +x /path/to/caesar/bin/*
```

**Library not found errors**
```bash
# Update library cache
sudo ldconfig

# Check dependencies
ldd /path/to/caesar/bin/caesar
```

### macOS Issues

**"Developer cannot be verified" warnings**
- Allow in System Preferences > Security & Privacy
- Or build from source instead of using pre-built binaries

**Homebrew conflicts**
- Use virtual environment or update Homebrew packages

## Performance Issues

### Slow compilation/interpretation**
- Use release build: `cmake -DCMAKE_BUILD_TYPE=Release ..`
- Check available memory and disk space
- Optimize code structure (avoid deep recursion)

### Memory issues**
- Monitor memory usage with system tools
- Reduce program complexity or data size
- Check for infinite loops or recursion

## Getting Help

### Before Asking for Help

1. **Check this troubleshooting guide**
2. **Search existing GitHub issues**
3. **Try with a minimal example**
4. **Collect system information:**
   ```bash
   # System info
   uname -a           # Linux/macOS
   systeminfo         # Windows
   
   # Caesar version
   caesar --version
   
   # CMake version
   cmake --version
   ```

### How to Report Issues

When reporting bugs, please include:

1. **System Information:**
   - Operating system and version
   - Compiler and version
   - CMake version

2. **Caesar Information:**
   - Caesar version (`caesar --version`)
   - Installation method (built from source vs pre-built)

3. **Problem Description:**
   - What you were trying to do
   - What happened instead
   - Full error messages (copy-paste, don't screenshot)

4. **Reproduction Steps:**
   - Minimal code example that reproduces the issue
   - Command-line arguments used
   - Any relevant files

5. **Additional Context:**
   - Have you tried the troubleshooting steps above?
   - Does it work with different input?
   - Recent changes to your system?

### Where to Get Help

- **GitHub Issues:** [https://github.com/juliuspleunes4/Caesar/issues](https://github.com/juliuspleunes4/Caesar/issues)
- **Documentation:** Check `docs/` directory and `USER_GUIDE.md`
- **Examples:** Look at `examples/` directory for working code

### Common Solutions Summary

1. **Restart terminal** after installation
2. **Run as Administrator/sudo** if permission issues
3. **Clean and rebuild** if compilation issues
4. **Check file encoding and indentation** for parser errors
5. **Verify paths and working directory** for file issues
6. **Update compilers and tools** for compatibility issues

---

*This troubleshooting guide is updated regularly. If you solve an issue not covered here, please consider contributing the solution!*