# Caesar Architecture Documentation

This document provides a technical overview of Caesar's architecture, implementation details, and design decisions. It's intended for contributors, language implementers, and anyone interested in understanding how Caesar works under the hood.

## Table of Contents

- [Overview](#overview)
- [System Architecture](#system-architecture)
- [Component Details](#component-details)
- [Data Flow](#data-flow)
- [Design Patterns](#design-patterns)
- [Performance Considerations](#performance-considerations)
- [Extension Points](#extension-points)

## Overview

Caesar is implemented as a **tree-walking interpreter** using modern C++17. The architecture follows the traditional compiler pipeline but emphasizes simplicity and extensibility over maximum performance.

### Key Design Principles

1. **Simplicity**: Clear, maintainable code over premature optimization
2. **Extensibility**: Easy to add new language features
3. **Correctness**: Comprehensive testing and error handling
4. **Python Compatibility**: Familiar syntax and semantics

### Technology Stack

- **Language**: C++17 (modern features, smart pointers, variant types)
- **Build System**: CMake 3.10+
- **Testing**: Custom test framework with comprehensive coverage
- **Memory Management**: RAII and smart pointers (no manual memory management)

## System Architecture

```
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   Source Code   │───▶│     Lexer       │───▶│     Tokens      │
│    (.csr)       │    │  (tokenization) │    │    (stream)     │
└─────────────────┘    └─────────────────┘    └─────────────────┘
                                                       │
                                                       ▼
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│    Execution    │◀───│   Interpreter   │◀───│     Parser      │
│   (side effects)│    │  (tree walker)  │    │ (AST builder)   │
└─────────────────┘    └─────────────────┘    └─────────────────┘
                                ▲                       │
                                │                       ▼
                       ┌─────────────────┐    ┌─────────────────┐
                       │   Environment   │    │       AST       │
                       │   (variables)   │    │  (syntax tree)  │
                       └─────────────────┘    └─────────────────┘
```

### Components Overview

| Component | Purpose | Files |
|-----------|---------|-------|
| **Lexer** | Tokenization | `lexer.h`, `lexer.cpp`, `token.h`, `token.cpp` |
| **Parser** | AST Construction | `parser.h`, `parser.cpp`, `ast.h`, `ast.cpp` |
| **Interpreter** | Execution | `interpreter.h`, `interpreter.cpp` |
| **Environment** | Variable Storage | Part of `interpreter.cpp` |
| **Main/REPL** | User Interface | `main.cpp`, `repl.cpp` |

## Component Details

### 1. Lexer (Tokenization)

**Location**: `src/lexer/`

The lexer converts source code into a stream of tokens using a state machine approach.

#### Key Classes

```cpp
class Token {
    TokenType type;
    std::string value;
    size_t line;
    size_t column;
};

class Lexer {
    std::string source;
    size_t current;
    size_t line;
    size_t column;
    
public:
    std::vector<Token> tokenize();
private:
    Token scanToken();
    bool isAtEnd();
    char advance();
    char peek();
    // ... helper methods
};
```

#### Token Types

Defined in `token.h`, includes:
- **Literals**: `INTEGER`, `FLOAT`, `STRING`, `BOOLEAN`
- **Keywords**: `IF`, `ELIF`, `ELSE`, `WHILE`, `FOR`, `DEF`, etc.
- **Operators**: `PLUS`, `MINUS`, `MULTIPLY`, `EQUAL`, `LESS`, etc.
- **Delimiters**: `LPAREN`, `RPAREN`, `LBRACE`, `RBRACE`, etc.

#### Design Decisions

- **Single-pass tokenization**: No lookahead beyond one character
- **Position tracking**: Line and column information for error reporting
- **Indentation handling**: Special `INDENT`/`DEDENT` tokens for Python-like blocks

### 2. Parser (AST Construction)

**Location**: `src/parser/`

The parser implements a **recursive descent parser** that constructs an Abstract Syntax Tree (AST).

#### AST Node Hierarchy

```cpp
// Base classes
class ASTNode { /* ... */ };
class Expression : public ASTNode { /* ... */ };
class Statement : public ASTNode { /* ... */ };

// Expression nodes
class LiteralExpression : public Expression;
class IdentifierExpression : public Expression;
class BinaryExpression : public Expression;
class UnaryExpression : public Expression;
class CallExpression : public Expression;
// ... more expression types

// Statement nodes
class ExpressionStatement : public Statement;
class IfStatement : public Statement;
class WhileStatement : public Statement;
class FunctionDefinition : public Statement;
// ... more statement types
```

#### Visitor Pattern

Caesar uses the **Visitor Pattern** for AST traversal:

```cpp
class ASTVisitor {
public:
    virtual void visit(LiteralExpression& node) = 0;
    virtual void visit(BinaryExpression& node) = 0;
    virtual void visit(IfStatement& node) = 0;
    // ... visit methods for all node types
};
```

#### Grammar and Precedence

The parser implements this grammar (simplified):

```
program        → statement* EOF
statement      → exprStmt | ifStmt | whileStmt | forStmt | funcDef | returnStmt
exprStmt       → expression NEWLINE
ifStmt         → "if" expression ":" block ("elif" expression ":" block)* ("else" ":" block)?
expression     → assignment
assignment     → logic_or ( "=" assignment )?
logic_or       → logic_and ( "or" logic_and )*
logic_and      → equality ( "and" equality )*
equality       → comparison ( ( "==" | "!=" ) comparison )*
comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )*
term           → factor ( ( "-" | "+" ) factor )*
factor         → unary ( ( "/" | "*" | "%" ) unary )*
unary          → ( "not" | "-" ) unary | call
call           → primary ( "(" arguments? ")" )*
primary        → NUMBER | STRING | "True" | "False" | "None" | IDENTIFIER | "(" expression ")"
```

### 3. Interpreter (Execution)

**Location**: `src/interpreter/`

The interpreter implements a **tree-walking interpreter** using the Visitor pattern.

#### Core Classes

```cpp
class Interpreter : public ASTVisitor {
    Environment* environment;
    Value last_value;
    std::map<std::string, BuiltinFunction> builtins;
    
public:
    Value interpret(Program* program);
    Value evaluate(Expression* expr);
    
    // Visitor methods for each AST node type
    void visit(LiteralExpression& node) override;
    void visit(BinaryExpression& node) override;
    // ... all visitor methods
};
```

#### Value System

Caesar uses `std::variant` for dynamic typing:

```cpp
using Value = std::variant<
    std::nullptr_t,              // None
    bool,                        // Boolean
    int64_t,                     // Integer
    double,                      // Float
    std::string,                 // String
    std::shared_ptr<CallableFunction>  // Function
>;
```

#### Environment (Variable Storage)

```cpp
class Environment {
    std::map<std::string, Value> variables;
    Environment* enclosing;  // For nested scopes
    
public:
    void define(const std::string& name, const Value& value);
    Value get(const std::string& name);
    void assign(const std::string& name, const Value& value);
};
```

#### Built-in Functions

Built-ins are implemented as C++ lambdas:

```cpp
builtins["print"] = [](const std::vector<Value>& args) -> Value {
    // Implementation
    return nullptr;
};

builtins["len"] = [](const std::vector<Value>& args) -> Value {
    // Implementation
    return static_cast<int64_t>(length);
};
```

### 4. Error Handling

Caesar implements comprehensive error handling:

#### Error Types

```cpp
class RuntimeError : public std::runtime_error {
    std::string message;
    // Could add position information
};

class ParseError : public std::runtime_error {
    Token token;
    std::string message;
};
```

#### Error Recovery

- **Lexer**: Reports invalid characters but continues
- **Parser**: Synchronizes after errors to find next statement
- **Interpreter**: Throws exceptions with meaningful messages

## Data Flow

### 1. Source to Tokens

```cpp
// Input: "x = 42 + 3"
// Output: [IDENTIFIER("x"), EQUAL, INTEGER("42"), PLUS, INTEGER("3"), EOF]

Lexer lexer(source_code);
std::vector<Token> tokens = lexer.tokenize();
```

### 2. Tokens to AST

```cpp
// Input: Token stream
// Output: AST nodes representing the syntax structure

Parser parser(tokens);
std::unique_ptr<Program> ast = parser.parse();
```

### 3. AST to Execution

```cpp
// Input: AST
// Output: Side effects (printing, variable changes, etc.)

Interpreter interpreter;
interpreter.interpret(ast.get());
```

## Design Patterns

### 1. Visitor Pattern

**Why**: Separates operations from data structure, making it easy to add new operations without modifying AST nodes.

**Usage**: Interpreter, future type checker, code generator, etc.

### 2. RAII (Resource Acquisition Is Initialization)

**Why**: Automatic memory management, exception safety.

**Usage**: Smart pointers for AST nodes, automatic cleanup.

### 3. Variant Types

**Why**: Type-safe union for Caesar's dynamic values.

**Usage**: `Value` type represents any Caesar value at runtime.

### 4. Builder Pattern

**Why**: Complex object construction with validation.

**Usage**: AST node construction in parser.

## Performance Considerations

### Current Performance Characteristics

- **Tree-walking**: Direct AST interpretation (slower than bytecode)
- **Dynamic typing**: Runtime type checking overhead
- **Memory allocation**: Frequent allocations for values and nodes

### Optimization Opportunities

1. **Bytecode Generation**: Compile AST to bytecode for faster execution
2. **Value Optimization**: Inline small values, intern strings
3. **Call Optimization**: Function call caching and optimization
4. **JIT Compilation**: Compile hot code paths to native code

### Memory Usage

- **AST Storage**: Tree structure with smart pointers
- **Environment Stack**: Nested environments for scoping
- **Value Storage**: Variant types with reference counting for complex types

## Extension Points

### Adding New Language Features

#### 1. New Token Types

```cpp
// In token.h
enum class TokenType {
    // ... existing tokens
    NEW_KEYWORD,
    NEW_OPERATOR
};
```

#### 2. New AST Nodes

```cpp
// In ast.h
class NewStatementType : public Statement {
    // Node-specific data
    void accept(ASTVisitor& visitor) override {
        visitor.visit(*this);
    }
};
```

#### 3. Parser Extensions

```cpp
// In parser.cpp
std::unique_ptr<Statement> Parser::parseNewStatement() {
    // Parsing logic
    return std::make_unique<NewStatementType>(/* ... */);
}
```

#### 4. Interpreter Support

```cpp
// In interpreter.cpp
void Interpreter::visit(NewStatementType& node) {
    // Execution logic
}
```

### Adding Built-in Functions

```cpp
// In interpreter.cpp, initializeBuiltins()
builtins["new_function"] = [](const std::vector<Value>& args) -> Value {
    // Validation
    if (args.size() != expected_count) {
        throw RuntimeError("new_function() takes exactly " + 
                          std::to_string(expected_count) + " arguments");
    }
    
    // Implementation
    // Return result
};
```

### Future Architecture Evolution

#### Phase 1: Bytecode VM
- Compile AST to bytecode
- Stack-based virtual machine
- Significant performance improvement

#### Phase 2: JIT Compilation
- Profile-guided optimization
- Compile hot paths to native code
- Maximum performance for compute-intensive code

#### Phase 3: Advanced Features
- Garbage collector for memory management
- Type inference and optional static typing
- Module system and package management

## Build System and Tools

### CMake Configuration

```cmake
# CMakeLists.txt structure
- Root CMakeLists.txt: Project configuration, subdirectories
- src/CMakeLists.txt: Library and executable targets
- tests/CMakeLists.txt: Test executable configuration
```

### Testing Framework

```cpp
// Custom test macros
#define ASSERT_EQ(expected, actual) /* ... */
#define ASSERT_TRUE(condition) /* ... */
#define ASSERT_THROWS(expression, exception_type) /* ... */

// Test structure
void test_function_name() {
    // Setup
    // Execute
    // Assert
}
```

### Development Workflow

1. **Feature Development**: Branch from main, implement feature
2. **Testing**: Add comprehensive tests for new functionality
3. **Documentation**: Update relevant documentation files
4. **Review**: Submit pull request for code review
5. **Integration**: Merge after approval and CI success

---

*This architecture documentation is a living document. As Caesar evolves, this document will be updated to reflect new design decisions and implementation details.*

*For questions about Caesar's architecture, please open an issue or discussion on GitHub.*