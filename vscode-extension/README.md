# Caesar Language Support for VS Code

[![Visual Studio Marketplace Version](https://img.shields.io/visual-studio-marketplace/v/caesar-lang.caesar-language-support)](https://marketplace.visualstudio.com/items?itemName=caesar-lang.caesar-language-support)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)

Official Visual Studio Code extension for the **Caesar Programming Language** - a high-performance, Python-like programming language that delivers **C++-level performance** with **interpreted language simplicity**.

## 🚀 Features

### **Complete Language Support**
- **Syntax Highlighting**: Full syntax highlighting for `.csr`, `.caesar`, and `.cae` files
- **Intelligent Code Completion**: Smart snippets for Caesar language constructs
- **Auto-Indentation**: Python-style indentation with proper block structure
- **Bracket Matching**: Automatic bracket pairing and matching
- **Comment Support**: Line comments (`#`) and block comments (`/* */`)

### **Code Snippets**
Ready-to-use code snippets for:
- Control structures (`if`, `while`, `for` loops)
- Function and class definitions
- Object-oriented programming patterns
- Error handling (`try-except`)
- Common programming patterns

### **Custom Theme**
- **Caesar Dark Theme**: Optimized color scheme for Caesar syntax
- Enhanced readability for Caesar-specific language constructs
- Professional developer experience

## 📥 Installation

1. **Via VS Code Marketplace**:
   - Open VS Code
   - Go to Extensions (`Ctrl+Shift+X`)
   - Search for "Caesar Language Support"
   - Click Install

2. **Via Command Line**:
   ```bash
   code --install-extension caesar-lang.caesar-language-support
   ```

## 🛠️ Getting Started

1. **Install Caesar**: Download Caesar from [GitHub Releases](https://github.com/juliuspleunes4/Caesar/releases)
2. **Create a Caesar file**: Create a new file with `.csr` extension
3. **Start coding**: Enjoy full syntax highlighting and code completion!

### Example Caesar Code
```python
# Caesar Programming Language Example
def fibonacci(n):
    if n <= 1:
        return n
    return fibonacci(n-1) + fibonacci(n-2)

class Calculator:
    def __init__(self):
        self.history = []
    
    def add(self, a, b):
        result = a + b
        self.history.append(f"Added {a} + {b} = {result}")
        return result

# Main execution
def main():
    calc = Calculator()
    result = calc.add(10, 20)
    fib_result = fibonacci(10)
    
    print(f"Addition result: {result}")
    print(f"Fibonacci(10): {fib_result}")

if __name__ == "__main__":
    main()
```

## 📚 Language Features Supported

### **Syntax Elements**
- Variables and expressions
- Control flow (`if`/`elif`/`else`, `while`, `for`)
- Functions with default parameters
- Classes and inheritance
- Lists, dictionaries, and data structures
- String literals with escape sequences
- Numeric literals (integers, floats, hex, binary, octal)

### **Built-in Functions**
- `print()`, `len()`, `range()`, `type()`
- `int()`, `float()`, `str()`, `bool()`
- `abs()`, `max()`, `min()`, `sum()`
- And many more...

## ⚡ Performance

Caesar delivers exceptional performance:
- **2-50x faster** than Python across all benchmarks
- **Competitive with C++** in many scenarios
- **Interpreted execution** with compiled-level performance

## 🔧 Extension Settings

This extension contributes the following settings:
- File associations for `.csr`, `.caesar`, and `.cae` files
- Syntax highlighting configuration
- Code snippet activation
- Custom theme integration

## 🐛 Known Issues

- Language Server Protocol (LSP) support coming in future releases
- IntelliSense and error checking planned for v1.0

## 📝 Release Notes

### 0.0.1

**Initial Release**
- Complete syntax highlighting for Caesar language
- Comprehensive code snippets library
- Caesar Dark theme
- Language configuration with proper indentation
- File association for multiple Caesar file extensions

## 🤝 Contributing

We welcome contributions! Visit our [GitHub repository](https://github.com/juliuspleunes4/Caesar) to:
- Report issues
- Submit feature requests
- Contribute to development
- Join the Caesar community

## 📄 License

This extension is licensed under the MIT License. See [LICENSE](LICENSE) for details.

## 🔗 Links

- **Caesar Language**: [GitHub Repository](https://github.com/juliuspleunes4/Caesar)
- **Documentation**: [Caesar Language Docs](https://github.com/juliuspleunes4/Caesar/docs)
- **Performance Benchmarks**: [Benchmark Results](https://github.com/juliuspleunes4/Caesar#performance-benchmarks)
- **Community**: [Discussions](https://github.com/juliuspleunes4/Caesar/discussions)

---

**Experience the future of programming languages with Caesar! 🏛️**
