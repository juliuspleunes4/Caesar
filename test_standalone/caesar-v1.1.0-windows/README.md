# Caesar Programming Language v1.1.0

**A Python-like programming language with C++ performance**

## Quick Start

1. **Extract** this ZIP file to any directory
2. **Run the installer**:
   - **Windows PowerShell**: `powershell -ExecutionPolicy Bypass -File install.ps1`
   - **Command Prompt**: Double-click `install.bat`
3. **Start coding** with Caesar!

## What's Included

- `bin/caesar.exe` - Caesar compiler and interpreter
- `bin/caesar_repl.exe` - Interactive REPL
- `examples/` - Sample Caesar programs
- `install.ps1` - PowerShell installer
- `install.bat` - Batch installer
- `USER_GUIDE.md` - Complete documentation (if included)

## Usage

After installation, you can use Caesar from anywhere:

`ash
# Run a Caesar program
caesar --interpret program.csr

# Show help
caesar --help

# Show version
caesar --version

# Start interactive REPL
caesar_repl
`

## Sample Caesar Program

Create a file called `hello.csr`:

`python
def greet(name):
    print("Hello,", name + "!")

if __name__ == "__main__":
    greet("World")
`

Run it:
`ash
caesar --interpret hello.csr
`

## Features

- âœ… Python-like syntax
- âœ… Functions with default parameters
- âœ… Control flow (if/elif/else, while, for loops)
- âœ… Built-in functions (print, range, len, str, int, float, type, abs)
- âœ… Recursive functions
- âœ… Variable scoping
- âœ… Mathematical operations
- âœ… String operations
- âœ… Interactive REPL

## System Requirements

- Windows 10/11 (64-bit)
- No additional dependencies required

## Getting Help

- Check the `examples/` directory for sample programs
- Use `caesar --help` for command-line options
- Start `caesar_repl` for interactive experimentation

## Installation Troubleshooting

**"caesar is not recognized as a command"**
- Restart your terminal/command prompt
- Or run: `refreshenv` (if you have Chocolatey)
- Or manually add the installation directory to your PATH

**Permission errors during installation**
- Run the installer as Administrator
- Or choose a different installation directory

## Happy Coding! ðŸš€

Caesar makes programming simple and fun. Enjoy building with Python-like syntax and C++ performance!
