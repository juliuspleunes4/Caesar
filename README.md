# Caesar Programming Language

Caesar is a Python-like programming language with C-speed performance, built using modern C++ and LLVM backend for efficient compilation.

## Features

- **Python-like syntax**: Familiar and readable syntax for easy adoption
- **High performance**: Compiled to native code using LLVM backend
- **Modern C++ implementation**: Clean, modular, and well-documented codebase
- **Indentation-based blocks**: Natural Python-style code structure
- **Strong typing**: Type safety with performance optimization

## Building Caesar

### Prerequisites

- CMake 3.15 or higher
- C++17 compatible compiler (GCC, Clang)
- LLVM 14 development packages
- libffi-dev and libncurses-dev

### Installation

```bash
# Install dependencies (Ubuntu/Debian)
sudo apt-get install cmake build-essential llvm-14-dev libllvm14 \
                     llvm-14-tools libffi-dev libncurses-dev

# Clone and build
git clone https://github.com/juliuspleunes4/Caesar.git
cd Caesar
mkdir build && cd build
cmake ..
make -j4
```

### Running Tests

```bash
# In the build directory
make test
# Or run individual tests
./tests/test_lexer
./tests/test_parser
```

## Usage

### Compiler

```bash
# Compile a Caesar program
./src/caesar program.csr

# Show tokenization
./src/caesar -t program.csr

# Show help
./src/caesar --help
```

### REPL

```bash
# Start interactive REPL
./src/caesar_repl
```

## Language Syntax

Caesar uses Python-like syntax with some enhancements for performance:

```python
# Function definition
def fibonacci(n):
    if n <= 1:
        return n
    else:
        return fibonacci(n-1) + fibonacci(n-2)

# Main function
def main():
    result = fibonacci(10)
    print("Fibonacci of 10 is:", result)

# Entry point
if __name__ == "__main__":
    main()
```

## Architecture

Caesar is designed with a modular architecture:

### Lexer (`src/lexer/`)
- **Token definitions**: All language tokens and operators
- **Lexical analysis**: Converts source code to tokens
- **Indentation handling**: Python-style block structure
- **Error reporting**: Detailed position information

### Parser (`src/parser/`)
- **AST nodes**: Abstract Syntax Tree representation
- **Recursive descent parser**: Clean grammar implementation
- **Expression precedence**: Correct operator precedence
- **Statement parsing**: Control flow and definitions

### IR Generation (Planned)
- **LLVM IR**: Intermediate representation
- **Optimization**: Performance optimizations
- **Type checking**: Static type analysis

### Code Generation (Planned)
- **Native compilation**: Machine code generation
- **Runtime support**: Language runtime features
- **Linking**: Executable creation

## Development Status

- [x] **Lexer**: Complete with full Python-like tokenization
- [x] **Parser**: Complete with AST generation
- [ ] **IR Generation**: Planned
- [ ] **Code Generation**: Planned
- [ ] **Runtime**: Planned
- [ ] **Standard Library**: Planned

## Contributing

Caesar is an open-source project. Contributions are welcome!

### Code Style
- Follow modern C++17 practices
- Use Doxygen-style documentation
- Maintain modular architecture
- Write comprehensive tests

### Building Documentation

```bash
# Install Doxygen
sudo apt-get install doxygen

# Generate documentation
make doc
```

## License

MIT License - see LICENSE file for details.

## Author

Julius Pleunes - Creator and maintainer of Caesar Programming Language
