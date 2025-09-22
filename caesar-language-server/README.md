# Caesar Language Server

A Language Server Protocol (LSP) implementation for the Caesar programming language, providing intelligent IDE features across all LSP-compatible editors.

## Features

- **Real-time Error Checking**: Syntax validation and common error detection
- **Intelligent Autocomplete**: Context-aware suggestions for Caesar language constructs
- **Built-in Function Support**: Autocomplete for `print`, `len`, `range`, `str`, `int`, `float`, `type`, `abs`
- **Keyword Completion**: Smart snippets for `if`, `while`, `for`, `def`, `class`, etc.
- **Universal Editor Support**: Works with VS Code, IntelliJ, Vim, Emacs, and any LSP-compatible editor

## Installation

```bash
npm install
npm run compile
```

## Usage

The language server can be started directly:

```bash
npm start
```

Or integrated with editors via LSP client configuration.

## Development

- `npm run compile` - Compile TypeScript to JavaScript
- `npm run watch` - Watch mode for development
- `npm start` - Start the language server

## Integration

This language server integrates with:
- VS Code Caesar extension
- Caesar interpreter (caesar.exe) for syntax validation
- Any LSP-compatible editor

## Architecture

The server provides:
- Diagnostic reporting (error checking)
- Completion suggestions
- Document synchronization
- Configuration management

Built with Microsoft's LSP libraries for maximum compatibility.