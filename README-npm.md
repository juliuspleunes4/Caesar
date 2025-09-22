# Caesar Language - NPM Package

[![npm version](https://badge.fury.io/js/caesar-lang.svg)](https://badge.fury.io/js/caesar-lang)

High-performance interpreted programming language with modern syntax and exceptional speed.

## 🚀 Quick Installation

```bash
npm install -g caesar-lang
```

## ✨ Features

- **High Performance**: 2-50x faster than Python, competitive with C++
- **Modern Syntax**: Clean, readable code with powerful features
- **Global CLI**: `caesar` command available anywhere
- **VS Code Support**: Full IDE integration with LSP
- **Cross-Platform**: Windows, macOS, Linux support

## 📖 Usage

### Command Line Interface

```bash
# Show help
caesar --help

# Show version
caesar --version

# Run a Caesar file
caesar file.csr

# Interpret mode
caesar --interpret file.csr

# NPM package info
caesar --npm-info
```

### Example Code

Create `hello.csr`:
```caesar
print "Hello, World!"

def fibonacci(n) {
    if n <= 1 {
        return n
    }
    return fibonacci(n-1) + fibonacci(n-2)
}

print "Fibonacci sequence:"
for i in range(10) {
    print fibonacci(i)
}
```

Run it:
```bash
caesar hello.csr
```

## 🛠️ Development

Caesar comes with example files to get you started:

```bash
# Check installation
caesar --version

# Try examples (if available)
caesar examples/hello_world.csr
caesar examples/fibonacci.csr
caesar examples/functions.csr
```

## 🎯 VS Code Integration

For the best development experience, install the VS Code extension:

1. Open VS Code
2. Search for "Caesar Language Support"
3. Install the extension
4. Enjoy syntax highlighting, autocomplete, and error checking!

## 📦 What's Included

- `caesar` - Main interpreter executable
- Example Caesar programs
- Global CLI wrapper for cross-platform compatibility
- Full Language Server Protocol support

## 🔧 Troubleshooting

### Command not found after installation

Try refreshing your terminal or running:
```bash
npm ls -g caesar-lang
```

### Permission errors on Unix systems

You may need to use sudo:
```bash
sudo npm install -g caesar-lang
```

### Windows PATH issues

Restart your terminal after installation to refresh the PATH.

## 📋 System Requirements

- Node.js 14.0.0 or higher
- Windows 10+, macOS 10.14+, or Linux (Ubuntu 18.04+)

## 🤝 Contributing

Visit our [GitHub repository](https://github.com/juliuspleunes4/Caesar) for:
- Source code
- Issue tracking
- Development documentation
- Contributing guidelines

## 📄 License

MIT License - see [LICENSE](https://github.com/juliuspleunes4/Caesar/blob/main/LICENSE) file for details.

## 🔗 Links

- [GitHub Repository](https://github.com/juliuspleunes4/Caesar)
- [VS Code Extension](https://marketplace.visualstudio.com/items?itemName=juliuspleunes4.caesar-language-support)
- [Documentation](https://github.com/juliuspleunes4/Caesar/tree/main/docs)
- [Issue Tracker](https://github.com/juliuspleunes4/Caesar/issues)

---

**Made with ❤️ by Julius Pleunes**