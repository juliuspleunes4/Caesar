# Caesar Programming Language - User Guide

## Quick Start

Caesar is a Python-like programming language with C-speed performance. Get started in 3 easy steps:

### 1. Installation

**Option A: PowerShell (Recommended)**
```powershell
# Run from the Caesar project directory
.\install.ps1
```

**Option B: Batch File**
```cmd
# Run from the Caesar project directory
install.bat
```

**Option C: Manual Installation**
1. Build the project: `cmake --build build -j`
2. Copy `build/src/caesar.exe` and `build/src/caesar_repl.exe` to your desired location
3. Add the location to your PATH environment variable

### 2. Verify Installation

```cmd
caesar --version
```

### 3. Run Your First Program

Create a file called `hello.csr`:
```python
print("Hello, Caesar!")
```

Run it:
```cmd
caesar --interpret hello.csr
```

## Usage

### Compiler Commands

```cmd
# Interpret and run a Caesar program
caesar --interpret program.csr

# Show the parsed AST (Abstract Syntax Tree)
caesar --parse program.csr

# Show tokenization output
caesar --tokens program.csr

# Start interactive REPL
caesar_repl
```

### Language Features

Caesar supports all essential programming constructs:

#### Variables and Arithmetic
```python
x = 10
y = 20
result = x + y * 2
print("Result:", result)
```

#### Functions with Default Parameters
```python
def greet(name, greeting="Hello"):
    print(greeting + ", " + name + "!")
    return len(name)

greet("World")           # Uses default greeting
greet("Caesar", "Hi")    # Custom greeting
```

#### Control Flow
```python
def classify(number):
    if number > 0:
        print("Positive")
    elif number < 0:
        print("Negative")
    else:
        print("Zero")

classify(5)
classify(-3)
classify(0)
```

#### Loops
```python
# While loops
count = 5
while count > 0:
    print("Count:", count)
    count = count - 1

# For loops with range
for i in range(1, 6):
    print("Number:", i)

# Loop control
for i in range(10):
    if i == 3:
        continue
    if i == 7:
        break
    print("Value:", i)
```

#### Recursive Functions
```python
def factorial(n):
    if n <= 1:
        return 1
    else:
        return n * factorial(n - 1)

print("5! =", factorial(5))
```

#### Built-in Functions
```python
# String operations
text = "Caesar"
print("Length:", len(text))
print("Uppercase string:", text)

# Type conversions
print("String to int:", int("42"))
print("Int to float:", float(42))
print("Number to string:", str(3.14))

# Math functions
print("Absolute value:", abs(-5))
print("Type information:", type(42))

# Range for loops
for i in range(0, 10, 2):  # start, stop, step
    print("Even number:", i)
```

## Examples

The installation includes several example programs in the `examples/` directory:

- `hello_world.csr` - Basic hello world program
- `fibonacci.csr` - Recursive fibonacci calculation
- `control_flow.csr` - Conditional statements and loops
- `functions.csr` - Function definitions and calls
- `caesar_demo.csr` - Comprehensive feature demonstration

Run any example:
```cmd
caesar --interpret examples/caesar_demo.csr
```

## Interactive REPL

Start the interactive Read-Eval-Print Loop:
```cmd
caesar_repl
```

Example REPL session:
```
Caesar Programming Language REPL v1.0.0
Type 'exit' or 'quit' to exit, 'help' for help

caesar> def add(x, y):
caesar>     return x + y
caesar> 
caesar> result = add(5, 3)
caesar> print("Result:", result)
Tokens:
  DEF('def') at 1:3
  IDENTIFIER('add') at 1:7
  ...
caesar> 
```

## Language Reference

### Supported Data Types
- **Integers**: `42`, `-17`, `0`
- **Floats**: `3.14`, `-2.7`, `0.0`
- **Strings**: `"hello"`, `'world'`
- **Booleans**: `True`, `False`
- **None**: `None`

### Operators
- **Arithmetic**: `+`, `-`, `*`, `/`, `%`, `**` (power)
- **Assignment**: `=`, `+=`, `-=`, `*=`, `/=`
- **Comparison**: `==`, `!=`, `<`, `<=`, `>`, `>=`
- **Logical**: `and`, `or`, `not`

### Control Structures
- **Conditionals**: `if`, `elif`, `else`
- **Loops**: `while`, `for`
- **Control flow**: `break`, `continue`, `pass`, `return`

### Built-in Functions
- `print(*args)` - Print values to console
- `len(obj)` - Get length of string
- `str(obj)` - Convert to string
- `int(obj)` - Convert to integer
- `float(obj)` - Convert to float
- `abs(num)` - Absolute value
- `type(obj)` - Get type information
- `range(start, stop, step)` - Generate number sequences

## Error Handling

Caesar provides clear error messages for common issues:

```cmd
# Syntax errors
caesar --parse program.csr

# Runtime errors  
caesar --interpret program.csr
```

## Performance

Caesar is designed for both ease of use and performance:
- Fast parsing: 1000+ functions in ~8ms
- Efficient execution: Optimized interpreter
- Memory efficient: Minimal allocation overhead

## Uninstallation

To remove Caesar:
```powershell
# If installed with PowerShell
PowerShell "C:\Users\YourName\Caesar\uninstall.ps1"
```

Or manually:
1. Remove the installation directory
2. Remove Caesar paths from your PATH environment variable
3. Delete any desktop shortcuts

## Troubleshooting

### Common Issues

**"caesar is not recognized as a command"**
- Ensure Caesar is in your PATH
- Restart your terminal after installation
- Try using the full path to the executable

**"Runtime Error: Undefined variable"**
- Check variable names for typos
- Ensure variables are defined before use
- Check function parameter names

**"Parsing Error"**
- Check indentation (use spaces, not tabs)
- Ensure proper syntax (colons after if/def/while)
- Verify parentheses and quotes are balanced

### Getting Help

For issues and feature requests:
- Check the examples directory
- Run `caesar_demo.csr` to test functionality
- Review error messages carefully

---

**Caesar Programming Language** - Building the future of performant, readable code. 🏛️