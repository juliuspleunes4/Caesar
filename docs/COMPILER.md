# Caesar Compiler Documentation

## Table of Contents

- [Overview](#overview)
- [Compilation Architecture](#compilation-architecture)
- [Compiler Pipeline](#compiler-pipeline)
- [Target Platforms](#target-platforms)
- [Language Features Coverage](#language-features-coverage)
- [Implementation Details](#implementation-details)
- [Usage Guide](#usage-guide)
- [Optimization Strategies](#optimization-strategies)
- [Testing Framework](#testing-framework)
- [Future Enhancements](#future-enhancements)

## Overview

Caesar is transitioning from a tree-walking interpreter to a true compiler with multiple compilation targets. This document provides a comprehensive guide to understanding, building, and extending the Caesar compiler.

### Design Philosophy

1. **Multi-Target Compilation**: Support multiple backends (bytecode, C, assembly)
2. **Optimization Pipeline**: Modern compiler optimizations for performance
3. **Complete Feature Coverage**: All Caesar language features must compile correctly
4. **Ease of Use**: Simple command-line interface matching `python` workflow
5. **Debugging Support**: Generate debug information for compiled code

### Project Goals

- **Default Compilation**: `caesar file.csr` compiles by default (no interpreter flag needed)
- **Optional Interpretation**: `caesar -i file.csr` to run with interpreter
- **Multiple Backends**: Bytecode, C transpilation, and native assembly
- **Performance**: Compiled code should match or exceed C++ performance
- **Complete Testing**: Rigorous testing of all language features across all compilation targets

## Implementation Status

### ✅ Fully Implemented (Production-Ready)

1. **Lexer** (100%)
   - All tokens recognized
   - Indentation tracking
   - String literals with escape sequences
   - Position tracking for error messages
   - Comprehensive test coverage

2. **Parser** (100%)
   - All language constructs supported
   - Proper precedence handling
   - Complete AST generation
   - Comprehensive test coverage

3. **Interpreter** (100%)
   - All features working
   - Functions, recursion, control flow
   - Built-in functions
   - Data structures (lists, dicts)
   - Comprehensive test coverage

4. **IR Generation** (98%)
   - Three-address code generation
   - All basic blocks
   - Control flow
   - Function definitions
   - ✅ **For loops with range()** (NEW - properly generates loop variables, initialization, condition checks, increments)
   - ⚠️ Classes not fully implemented

### 🚧 Partial Implementation

5. **C Code Generator** (92%)
   - ✅ Variables and assignments
   - ✅ All arithmetic operations (+, -, *, /, %, negation)
   - ✅ All comparison operators (==, !=, <, <=, >, >=)
   - ✅ All logical operators (and, or, not)
   - ✅ Boolean literals (True/False → true/false)
   - ✅ **String literals** (proper const char* types)
   - ✅ **Float/double literals** (proper double types)
   - ✅ **Complete type system** (int64_t, bool, const char*, double)
   - ✅ **Type promotion** (mixed int/float → double, C semantics)
   - ✅ **Float arithmetic** (all operations, mixed types, comparisons)
   - ✅ **Mixed type operations** (all combinations tested)
   - ✅ **Modulo operations** (positive, negative, large numbers)
   - ✅ **Proper C identifier generation** (sanitizes IR names)
   - ✅ Control flow (if/else, goto, labels)
   - ✅ **While loops** (simple, nested, complex conditions, all edge cases)
   - ✅ **For loops with range()** (simple, start/stop, step, negative ranges, descending, nested, all edge cases)
   - ✅ **print() built-in function** (single/multiple args, all types, loops, expressions)
   - ✅ **len() built-in function** (strings, empty, long, single char, expressions, loops, comparisons, arithmetic)
   - ✅ **str() built-in function** (int/float/string/bool conversion, nested calls, all types)
   - ✅ **int() built-in function** (string parsing, float truncation, bool conversion, identity, all types)
   - ✅ **float() built-in function** (int→float, string parsing, bool conversion, identity, nested calls) (NEW)
   - ✅ **abs() built-in function** (positive/negative int/float, zero, bool conversion, nested calls) (NEW)
   - ✅ **Self-contained C code generation** (inlined runtime, no external dependencies)
   - ✅ **215 comprehensive tests pass** (15 basic + 20 edge + 15 strings + 20 floats + 24 mixed + 30 loops + 16 print + 15 len + 15 str + 15 int + 15 float + 15 abs)
   - ✅ Variable declaration tracking
   - ✅ Negative numbers, zero operations
   - ✅ Large numbers, operator precedence
   - ✅ Deep nesting, complex expressions
   - ⚠️ Escape sequences in strings (lexer limitation)
   - ⚠️ **Function definitions incomplete**:
     - ✅ IR Generator: Generates function labels, parameter declarations, body, and return instructions
     - ❌ C Generator: No DEFINE_FUNCTION opcode handling - functions not emitted as C functions
     - ❌ Missing: Function prologue/epilogue generation
     - ❌ Missing: Parameter passing in generated C code
     - ❌ Missing: Return value handling in generated C code
     - ❌ Missing: Function call mechanism (CALL opcode only handles built-ins, user-defined functions not supported)
     - ❌ Missing: Scope management for local variables
   - ⚠️ Other built-in functions incomplete (type, etc.)
   - ⚠️ Data structures not implemented

6. **Bytecode Generator** (40%)
   - ✅ Format defined
   - ✅ Basic emission
   - ⚠️ VM executor not implemented
   - ⚠️ Runtime library not implemented

7. **x86-64 Generator** (30%)
   - ✅ Basic structure
   - ✅ Register allocation
   - ✅ Simple operations
   - ⚠️ Function calls incomplete
   - ⚠️ System integration incomplete

### 📋 Not Implemented

8. **Bytecode Virtual Machine** (0%)
   - Requires production-ready value type system
   - Proper memory management
   - Call stack implementation
   - Built-in function runtime
   - Extensive testing infrastructure

9. **Semantic Analysis** (0%)
   - Type checking
   - Symbol table management
   - Scope resolution
   - Error detection

10. **Optimization Passes** (0%)
   - Constant folding
   - Dead code elimination
   - Register allocation
   - Other optimizations

11. **ARM64 Backend** (0%)
   - Not yet started

### Current Limitations

**What Works:**
- Lexing and parsing all Caesar syntax
- Interpreting all language features
- Generating IR for most features
- Basic C code generation (arithmetic, control flow)
- Basic assembly generation structure

**What Doesn't Work Yet:**
- **Bytecode VM execution**: Falls back to interpreter
- **Function compilation**: Functions work in interpreter but not in compiled code
- **Built-in functions in compiled code**: Need runtime library
- **Data structures in compiled code**: Need runtime support
- **Classes**: Not fully implemented anywhere
- **Optimizations**: No optimization passes yet

**Current Behavior:**
```bash
$ caesar program.csr
# Currently falls back to interpreter
# TODO: Execute compiled bytecode with VM (see docs/IMPLEMENTATION_PLAN.md Phase 2)

$ caesar -i program.csr  
# Uses tree-walking interpreter (fully working)
```

### Testing Status

- **15/15 tests passing** (100%)
- Comprehensive lexer tests ✅
- Comprehensive parser tests ✅
- IR generation tests ✅
- Compiler tests (basic) ✅
- **C code generation tests** ✅
  - Basic features: 15 sub-tests
  - Edge cases: 20 sub-tests
  - String literals: 15 sub-tests
  - **Float/double operations: 20 sub-tests** (NEW)
  - **Total: 70 comprehensive C codegen tests**
- Integration tests ✅
- **Missing**: VM execution tests, end-to-end compilation with execution tests

## Compilation Architecture

Caesar uses a traditional multi-stage compiler architecture with modern optimizations:

```
┌─────────────────┐
│  Source Code    │
│   (.csr file)   │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│  Lexical        │
│  Analysis       │ ───→ Token Stream
│  (Lexer)        │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│  Syntax         │
│  Analysis       │ ───→ Abstract Syntax Tree (AST)
│  (Parser)       │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│  Semantic       │
│  Analysis       │ ───→ Annotated AST
│  (Type Check)   │      Symbol Tables
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│  Intermediate   │
│  Representation │ ───→ Three-Address Code (IR)
│  (IR Gen)       │      Basic Blocks
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│  Optimization   │
│  Passes         │ ───→ Optimized IR
│  (Optimizer)    │
└────────┬────────┘
         │
         ├──────────────┬──────────────┬──────────────┐
         ▼              ▼              ▼              ▼
    ┌─────────┐   ┌─────────┐   ┌─────────┐   ┌─────────┐
    │Bytecode │   │ C Code  │   │  x86-64 │   │  ARM64  │
    │Generator│   │Generator│   │Generator│   │Generator│
    └─────────┘   └─────────┘   └─────────┘   └─────────┘
         │              │              │              │
         ▼              ▼              ▼              ▼
    ┌─────────┐   ┌─────────┐   ┌─────────┐   ┌─────────┐
    │ .bc     │   │  .c     │   │  .asm   │   │  .asm   │
    │bytecode │   │ C code  │   │ x86 asm │   │ ARM asm │
    └─────────┘   └─────────┘   └─────────┘   └─────────┘
         │              │              │              │
         ▼              ▼              ▼              ▼
    ┌─────────┐   ┌─────────┐   ┌─────────┐   ┌─────────┐
    │   VM    │   │   GCC   │   │Assembler│   │Assembler│
    │Executor │   │Compiler │   │+ Linker │   │+ Linker │
    └─────────┘   └─────────┘   └─────────┘   └─────────┘
         │              │              │              │
         └──────────────┴──────────────┴──────────────┘
                        │
                        ▼
                 ┌─────────────┐
                 │  Executable │
                 │   Binary    │
                 └─────────────┘
```

## Compiler Pipeline

### Stage 1: Lexical Analysis (Lexer)

**Purpose**: Convert source code text into a stream of tokens

**Implementation**: `src/lexer/lexer.cpp`, `include/caesar/lexer.h`

**Key Features**:
- Character-by-character scanning
- Token recognition (keywords, operators, literals, identifiers)
- Indentation tracking (INDENT/DEDENT tokens for Python-like syntax)
- String literal processing with escape sequences
- Comment handling
- Position tracking for error reporting

**Token Types**:
```cpp
enum class TokenType {
    // Literals
    INTEGER,        // 42, 123
    FLOAT,          // 3.14, 2.718
    STRING,         // "hello", 'world'
    BOOLEAN,        // True, False
    NONE,           // None
    
    // Keywords
    IF, ELIF, ELSE,       // Conditionals
    WHILE, FOR, IN,       // Loops
    DEF, CLASS, RETURN,   // Definitions
    BREAK, CONTINUE, PASS,// Control flow
    AND, OR, NOT,         // Logical operators
    
    // Operators
    PLUS, MINUS, MULT, DIV, MOD, POWER,          // Arithmetic
    ASSIGN,                                       // Assignment
    PLUS_ASSIGN, MINUS_ASSIGN, MULT_ASSIGN, DIV_ASSIGN,  // Compound
    EQUAL, NOT_EQUAL,                             // Comparison
    LESS, GREATER, LESS_EQUAL, GREATER_EQUAL,
    
    // Delimiters
    LPAREN, RPAREN,       // ()
    LBRACKET, RBRACKET,   // []
    LBRACE, RBRACE,       // {}
    COMMA, COLON, DOT,    // , : .
    
    // Structure
    INDENT, DEDENT,       // Python-like indentation
    NEWLINE, EOF,         // Line/file endings
    IDENTIFIER            // Variable/function names
};
```

**Example Lexer Output**:
```python
# Source code:
def add(a, b):
    return a + b

# Token stream:
DEF "def" (1:0)
IDENTIFIER "add" (1:4)
LPAREN "(" (1:7)
IDENTIFIER "a" (1:8)
COMMA "," (1:9)
IDENTIFIER "b" (1:11)
RPAREN ")" (1:12)
COLON ":" (1:13)
NEWLINE "\n" (1:14)
INDENT "" (2:0)
RETURN "return" (2:4)
IDENTIFIER "a" (2:11)
PLUS "+" (2:13)
IDENTIFIER "b" (2:15)
NEWLINE "\n" (2:16)
DEDENT "" (3:0)
EOF "" (3:0)
```

### Stage 2: Syntax Analysis (Parser)

**Purpose**: Build Abstract Syntax Tree (AST) from token stream

**Implementation**: `src/parser/parser.cpp`, `include/caesar/ast.h`

**Parsing Strategy**: Recursive descent with operator precedence

**Key Components**:

1. **Expression Parsing** (precedence-based):
   - Primary: Literals, identifiers, parenthesized expressions
   - Unary: `-x`, `not x`, `+x`
   - Multiplicative: `*`, `/`, `%`
   - Additive: `+`, `-`
   - Comparison: `<`, `<=`, `>`, `>=`
   - Equality: `==`, `!=`
   - Logical AND: `and`
   - Logical OR: `or`
   - Assignment: `=`, `+=`, `-=`, `*=`, `/=`

2. **Statement Parsing**:
   - Expression statements
   - Function definitions (`def name(params): body`)
   - Class definitions (`class Name: body`)
   - Conditional statements (`if/elif/else`)
   - Loop statements (`while`, `for`)
   - Control flow (`return`, `break`, `continue`, `pass`)

3. **AST Node Types**:

**Expressions**:
```cpp
- LiteralExpression      // 42, "hello", True
- IdentifierExpression   // variable_name
- BinaryExpression       // a + b, x * y
- UnaryExpression        // -x, not y
- CallExpression         // func(arg1, arg2)
- MemberExpression       // obj.property
- AssignmentExpression   // x = 5, y += 10
- ListExpression         // [1, 2, 3]
- DictExpression         // {"key": "value"}
```

**Statements**:
```cpp
- ExpressionStatement    // expr;
- BlockStatement         // { stmt1; stmt2; }
- IfStatement            // if cond: body
- WhileStatement         // while cond: body
- ForStatement           // for x in iter: body
- FunctionDefinition     // def name(params): body
- ClassDefinition        // class Name: body
- ReturnStatement        // return expr
- BreakStatement         // break
- ContinueStatement      // continue
- PassStatement          // pass
```

**Example AST**:
```python
# Source code:
def factorial(n):
    if n <= 1:
        return 1
    return n * factorial(n - 1)

# AST (simplified):
FunctionDefinition {
    name: "factorial",
    parameters: [Parameter("n")],
    body: BlockStatement {
        statements: [
            IfStatement {
                condition: BinaryExpression {
                    left: Identifier("n"),
                    operator: <=,
                    right: Literal(1)
                },
                then_branch: BlockStatement {
                    statements: [
                        ReturnStatement {
                            value: Literal(1)
                        }
                    ]
                }
            },
            ReturnStatement {
                value: BinaryExpression {
                    left: Identifier("n"),
                    operator: *,
                    right: CallExpression {
                        callee: Identifier("factorial"),
                        arguments: [
                            BinaryExpression {
                                left: Identifier("n"),
                                operator: -,
                                right: Literal(1)
                            }
                        ]
                    }
                }
            }
        ]
    }
}
```

### Stage 3: Semantic Analysis (Type Checker)

**Purpose**: Verify semantic correctness and type safety

**Status**: 🚧 To be implemented

**Planned Features**:

1. **Symbol Table Management**:
   - Track variable declarations and scopes
   - Function signature validation
   - Class hierarchy verification
   - Detect undefined variables
   - Detect duplicate definitions

2. **Type Checking**:
   - Infer types for expressions
   - Validate function call arguments
   - Check operator compatibility
   - Verify return type consistency
   - Validate list/dict operations

3. **Semantic Validation**:
   - Break/continue only in loops
   - Return statements in functions
   - Class inheritance checks
   - Default parameter validation
   - Circular import detection (future)

**Implementation Plan**:
```cpp
class SemanticAnalyzer : public ASTVisitor {
private:
    SymbolTable symbol_table;
    TypeEnvironment type_env;
    std::vector<std::string> errors;
    
    // Scope management
    void enterScope();
    void exitScope();
    void declareVariable(const std::string& name, Type type);
    Type lookupVariable(const std::string& name);
    
    // Type inference
    Type inferType(Expression* expr);
    void checkType(Expression* expr, Type expected);
    
public:
    void analyze(Program* program);
    std::vector<std::string> getErrors() const;
};
```

### Stage 4: Intermediate Representation (IR)

**Purpose**: Generate platform-independent three-address code

**Implementation**: `src/ir/ir.cpp`, `include/caesar/ir.h`

**IR Format**: Three-Address Code (TAC) with virtual registers

**Key Concepts**:

1. **Three-Address Code**: Each instruction has at most three operands
   ```
   ADD %r0, %r1, %r2    // r0 = r1 + r2
   LOAD_CONST %r0, #42  // r0 = 42
   JUMP_IF_FALSE @L1, %r0  // if (!r0) goto L1
   ```

2. **Virtual Registers**: Unlimited virtual registers (named %r0, %r1, %r2, ...)
   - Allocated during IR generation
   - Mapped to physical registers or stack slots during code generation

3. **Basic Blocks**: Sequences of instructions with single entry and exit
   - Label at the beginning
   - No branches except at the end
   - Used for optimization and control flow analysis

**IR Instructions**:

```cpp
// Arithmetic Operations
ADD %dest, %src1, %src2      // dest = src1 + src2
SUB %dest, %src1, %src2      // dest = src1 - src2
MUL %dest, %src1, %src2      // dest = src1 * src2
DIV %dest, %src1, %src2      // dest = src1 / src2
MOD %dest, %src1, %src2      // dest = src1 % src2
NEG %dest, %src              // dest = -src

// Comparison Operations
EQ %dest, %src1, %src2       // dest = (src1 == src2)
NE %dest, %src1, %src2       // dest = (src1 != src2)
LT %dest, %src1, %src2       // dest = (src1 < src2)
LE %dest, %src1, %src2       // dest = (src1 <= src2)
GT %dest, %src1, %src2       // dest = (src1 > src2)
GE %dest, %src1, %src2       // dest = (src1 >= src2)

// Logical Operations
AND %dest, %src1, %src2      // dest = src1 && src2
OR %dest, %src1, %src2       // dest = src1 || src2
NOT %dest, %src              // dest = !src

// Memory Operations
LOAD_CONST %dest, #value     // dest = constant value
GET_VAR %dest, $name         // dest = variable[name]
SET_VAR $name, %src          // variable[name] = src
ALLOC %dest, #size           // dest = allocate(size)

// Control Flow
LABEL @name                  // Define label
JUMP @label                  // Unconditional jump
JUMP_IF_TRUE @label, %cond   // Jump if cond is true
JUMP_IF_FALSE @label, %cond  // Jump if cond is false
CALL %dest, $func            // dest = call function
PARAM %src                   // Push function parameter
RETURN                       // Return from function
RETURN %value                // Return value from function

// Special
PRINT %src                   // Print value (debug)
NOP                          // No operation
```

**Example IR Generation**:

```python
# Source code:
def fibonacci(n):
    if n <= 1:
        return n
    return fibonacci(n-1) + fibonacci(n-2)

# Generated IR:
func_fibonacci:
    LOAD_CONST %r0, #1
    GET_VAR %r1, $n
    LE %r2, %r1, %r0          # r2 = (n <= 1)
    JUMP_IF_FALSE @else0, %r2
    GET_VAR %r3, $n
    RETURN %r3                 # return n
    JUMP @endif1
else0:
    GET_VAR %r4, $n
    LOAD_CONST %r5, #1
    SUB %r6, %r4, %r5         # r6 = n - 1
    PARAM %r6
    CALL %r7, $fibonacci      # r7 = fibonacci(n-1)
    GET_VAR %r8, $n
    LOAD_CONST %r9, #2
    SUB %r10, %r8, %r9        # r10 = n - 2
    PARAM %r10
    CALL %r11, $fibonacci     # r11 = fibonacci(n-2)
    ADD %r12, %r7, %r11       # r12 = r7 + r11
    RETURN %r12                # return r12
endif1:
```

### Stage 5: Optimization

**Purpose**: Improve code performance and reduce size

**Status**: 🚧 To be implemented

**Planned Optimization Passes**:

1. **Local Optimizations** (within basic blocks):
   - **Constant Folding**: Evaluate constant expressions at compile time
     ```
     ADD %r0, #5, #3  →  LOAD_CONST %r0, #8
     ```
   - **Constant Propagation**: Replace variable uses with known constants
     ```
     LOAD_CONST %r0, #10
     ADD %r1, %r0, #5  →  LOAD_CONST %r1, #15
     ```
   - **Dead Code Elimination**: Remove unreachable or unused code
   - **Algebraic Simplification**: Apply mathematical identities
     ```
     MUL %r0, %r1, #1  →  LOAD %r0, %r1
     ADD %r0, %r1, #0  →  LOAD %r0, %r1
     ```

2. **Global Optimizations** (across basic blocks):
   - **Common Subexpression Elimination**: Reuse computed values
   - **Loop Invariant Code Motion**: Move unchanging code out of loops
   - **Strength Reduction**: Replace expensive operations with cheaper ones
     ```
     MUL %r0, %r1, #2  →  ADD %r0, %r1, %r1
     ```

3. **Control Flow Optimizations**:
   - **Branch Optimization**: Simplify conditional branches
   - **Jump Threading**: Eliminate redundant jumps
   - **Loop Unrolling**: Duplicate loop bodies for performance

4. **Register Allocation**:
   - **Graph Coloring**: Assign virtual registers to physical registers
   - **Spilling**: Move less-used values to stack when registers are exhausted
   - **Live Range Analysis**: Determine when variables are needed

### Stage 6: Code Generation

**Purpose**: Generate executable code for target platforms

**Implementation**: `src/codegen/codegen.cpp`, `include/caesar/codegen.h`

## Target Platforms

### 1. Bytecode VM (Stack-Based Virtual Machine)

**Purpose**: Platform-independent bytecode execution

**Architecture**: Stack-based virtual machine

**Bytecode Format**:
```
OPCODE [OPERAND1] [OPERAND2] ...

Examples:
PUSH 42              # Push constant 42
ADD                  # Pop two values, add, push result
LOAD_VAR "x"         # Push variable x
STORE_VAR "y"        # Pop value, store in y
JUMP_IF_FALSE 100    # Conditional jump to offset 100
CALL "print" 1       # Call function with 1 argument
```

**VM Components**:
- **Stack**: Operand stack for values
- **Heap**: Dynamic memory for objects
- **Call Stack**: Function call frames
- **Instruction Pointer**: Current bytecode position
- **Built-in Functions**: Native implementations

**Advantages**:
- Platform independent
- Easy to implement
- Good for debugging
- Fast startup time

**Disadvantages**:
- Slower than native code
- Interpretation overhead

**Status**: 🚧 Basic implementation exists, needs completion

### 2. C Code Generation (Transpiler)

**Purpose**: Generate C code that can be compiled with GCC/Clang

**Advantages**:
- Leverage existing C compiler optimizations
- Easy to debug (readable C code)
- Excellent portability
- Can link with C libraries

**Disadvantages**:
- Requires C compiler on target system
- Two-stage compilation process
- Some Caesar features don't map directly to C

**Current Output Format**:
```c
// Caesar C Code
// Generated by Caesar Compiler v1.5.1

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// Forward declarations
typedef struct CaesarValue {
    enum { INT, FLOAT, STRING, BOOL, NONE, LIST, DICT } type;
    union {
        int64_t i;
        double f;
        char* s;
        bool b;
        void* ptr;
    } data;
} CaesarValue;

// Built-in functions
void caesar_print(CaesarValue val);
CaesarValue caesar_len(CaesarValue val);
CaesarValue caesar_range(int start, int end, int step);
// ... more built-ins

// Generated code
int main() {
    // Function implementations
    // ...
}
```

**Status**: 🚧 Partial implementation, needs completion

### 3. x86-64 Assembly Generation

**Purpose**: Generate native x86-64 machine code

**Target**: Linux/Windows x86-64 systems

**Calling Convention**: System V AMD64 ABI (Linux) / Microsoft x64 (Windows)

**Register Usage**:
- `rax`: Return values, temporary
- `rbx`: Callee-saved
- `rcx`, `rdx`: Arguments (Windows), temporary (Linux)
- `rsi`, `rdi`: Arguments (Linux)
- `rsp`: Stack pointer
- `rbp`: Frame pointer
- `r8-r15`: General purpose / arguments

**Code Structure**:
```asm
section .data
    ; String literals
    str0: db "Hello, World!", 0
    
section .text
    global main
    extern printf
    
main:
    ; Function prologue
    push rbp
    mov rbp, rsp
    sub rsp, 16          ; Allocate stack space
    
    ; Function body
    lea rdi, [str0]      ; Load string address
    xor rax, rax         ; Clear rax (no vector args)
    call printf          ; Call printf
    
    ; Function epilogue
    xor rax, rax         ; Return 0
    add rsp, 16          ; Deallocate stack
    pop rbp
    ret
```

**Status**: 🚧 Basic structure exists, needs full implementation

### 4. ARM64 Assembly Generation (Future)

**Purpose**: Generate native ARM64 machine code

**Target**: ARM64 Linux/macOS systems

**Status**: 📋 Planned for future release

## Language Features Coverage

This section details how each Caesar language feature is compiled across different targets.

### 1. Variables and Assignments

**Caesar Syntax**:
```python
x = 42
y = x + 10
z = "hello"
```

**IR**:
```
LOAD_CONST %r0, #42
SET_VAR $x, %r0
GET_VAR %r1, $x
LOAD_CONST %r2, #10
ADD %r3, %r1, %r2
SET_VAR $y, %r3
LOAD_CONST %r4, #"hello"
SET_VAR $z, %r4
```

**C Code**:
```c
CaesarValue x = make_int(42);
CaesarValue y = add_values(x, make_int(10));
CaesarValue z = make_string("hello");
```

**x86-64 Assembly**:
```asm
mov QWORD [rbp-8], 42      ; x = 42
mov rax, [rbp-8]
add rax, 10
mov QWORD [rbp-16], rax    ; y = x + 10
lea rax, [str_hello]
mov QWORD [rbp-24], rax    ; z = "hello"
```

### 2. Arithmetic Operations

**Caesar Syntax**:
```python
result = (a + b) * c - d / e
```

**IR**:
```
GET_VAR %r0, $a
GET_VAR %r1, $b
ADD %r2, %r0, %r1          # r2 = a + b
GET_VAR %r3, $c
MUL %r4, %r2, %r3          # r4 = (a + b) * c
GET_VAR %r5, $d
GET_VAR %r6, $e
DIV %r7, %r5, %r6          # r7 = d / e
SUB %r8, %r4, %r7          # r8 = r4 - r7
SET_VAR $result, %r8
```

**C Code**:
```c
CaesarValue result = sub_values(
    mul_values(
        add_values(a, b),
        c
    ),
    div_values(d, e)
);
```

**x86-64 Assembly**:
```asm
mov rax, [rbp-8]           ; Load a
add rax, [rbp-16]          ; a + b
imul rax, [rbp-24]         ; (a+b) * c
mov rbx, [rbp-32]          ; Load d
mov rcx, [rbp-40]          ; Load e
xor rdx, rdx
idiv rcx                   ; d / e (result in rax)
sub rax, rbx               ; Subtract
mov [rbp-48], rax          ; Store result
```

### 3. Conditional Statements (if/elif/else)

**Caesar Syntax**:
```python
if x > 0:
    print("positive")
elif x < 0:
    print("negative")
else:
    print("zero")
```

**IR**:
```
GET_VAR %r0, $x
LOAD_CONST %r1, #0
GT %r2, %r0, %r1           # r2 = (x > 0)
JUMP_IF_FALSE @elif0, %r2
LOAD_CONST %r3, #"positive"
PARAM %r3
CALL %r4, $print
JUMP @endif2
elif0:
GET_VAR %r5, $x
LOAD_CONST %r6, #0
LT %r7, %r5, %r6           # r7 = (x < 0)
JUMP_IF_FALSE @else1, %r7
LOAD_CONST %r8, #"negative"
PARAM %r8
CALL %r9, $print
JUMP @endif2
else1:
LOAD_CONST %r10, #"zero"
PARAM %r10
CALL %r11, $print
endif2:
```

**C Code**:
```c
if (compare_gt(x, make_int(0)).data.b) {
    caesar_print(make_string("positive"));
} else if (compare_lt(x, make_int(0)).data.b) {
    caesar_print(make_string("negative"));
} else {
    caesar_print(make_string("zero"));
}
```

**x86-64 Assembly**:
```asm
mov rax, [rbp-8]           ; Load x
cmp rax, 0                 ; Compare x with 0
jle .elif0                 ; Jump if x <= 0
    lea rdi, [str_pos]     ; "positive"
    call print
    jmp .endif2
.elif0:
    cmp rax, 0
    jge .else1             ; Jump if x >= 0
    lea rdi, [str_neg]     ; "negative"
    call print
    jmp .endif2
.else1:
    lea rdi, [str_zero]    ; "zero"
    call print
.endif2:
```

### 4. While Loops

**Caesar Syntax**:
```python
i = 0
while i < 10:
    print(i)
    i = i + 1
```

**IR**:
```
LOAD_CONST %r0, #0
SET_VAR $i, %r0
loop_start0:
GET_VAR %r1, $i
LOAD_CONST %r2, #10
LT %r3, %r1, %r2           # r3 = (i < 10)
JUMP_IF_FALSE @loop_end1, %r3
GET_VAR %r4, $i
PARAM %r4
CALL %r5, $print
GET_VAR %r6, $i
LOAD_CONST %r7, #1
ADD %r8, %r6, %r7          # r8 = i + 1
SET_VAR $i, %r8
JUMP @loop_start0
loop_end1:
```

**C Code**:
```c
CaesarValue i = make_int(0);
while (compare_lt(i, make_int(10)).data.b) {
    caesar_print(i);
    i = add_values(i, make_int(1));
}
```

**x86-64 Assembly**:
```asm
mov QWORD [rbp-8], 0       ; i = 0
.loop_start:
    mov rax, [rbp-8]       ; Load i
    cmp rax, 10            ; Compare i with 10
    jge .loop_end          ; Jump if i >= 10
    mov rdi, rax           ; Argument for print
    call print
    mov rax, [rbp-8]       ; Load i
    add rax, 1             ; i + 1
    mov [rbp-8], rax       ; Store i
    jmp .loop_start
.loop_end:
```

### 5. For Loops with Range

**Caesar Syntax**:
```python
for i in range(5):
    print(i)
```

**IR**:
```
LOAD_CONST %r0, #0
LOAD_CONST %r1, #5
LOAD_CONST %r2, #1
# range(0, 5, 1) creates iterator
for_iter0:
GET_VAR %r3, $__iter_pos
LT %r4, %r3, %r1           # Check if iterator < end
JUMP_IF_FALSE @for_end1, %r4
SET_VAR $i, %r3            # i = current iterator value
GET_VAR %r5, $i
PARAM %r5
CALL %r6, $print
GET_VAR %r7, $__iter_pos
ADD %r8, %r7, %r2          # Increment iterator
SET_VAR $__iter_pos, %r8
JUMP @for_iter0
for_end1:
```

**C Code**:
```c
for (int64_t i = 0; i < 5; i++) {
    caesar_print(make_int(i));
}
```

**x86-64 Assembly**:
```asm
xor rax, rax               ; i = 0
.for_loop:
    cmp rax, 5             ; Compare i with 5
    jge .for_end           ; Jump if i >= 5
    mov rdi, rax           ; Argument for print
    push rax               ; Save i
    call print
    pop rax                ; Restore i
    inc rax                ; i++
    jmp .for_loop
.for_end:
```

### 6. Functions

**Caesar Syntax**:
```python
def add(a, b):
    return a + b

result = add(10, 20)
```

**IR**:
```
func_add:
    GET_VAR %r0, $a
    GET_VAR %r1, $b
    ADD %r2, %r0, %r1
    RETURN %r2

entry:
    LOAD_CONST %r3, #10
    PARAM %r3
    LOAD_CONST %r4, #20
    PARAM %r4
    CALL %r5, $add
    SET_VAR $result, %r5
```

**C Code**:
```c
CaesarValue func_add(CaesarValue a, CaesarValue b) {
    return add_values(a, b);
}

int main() {
    CaesarValue result = func_add(make_int(10), make_int(20));
}
```

**x86-64 Assembly**:
```asm
func_add:
    push rbp
    mov rbp, rsp
    mov rax, rdi           ; a (first argument)
    add rax, rsi           ; a + b (second argument)
    pop rbp
    ret

main:
    mov rdi, 10            ; First argument
    mov rsi, 20            ; Second argument
    call func_add
    mov [rbp-8], rax       ; Store result
```

### 7. Function Default Parameters

**Caesar Syntax**:
```python
def greet(name, greeting="Hello"):
    print(greeting, name)

greet("Alice")
greet("Bob", "Hi")
```

**IR**:
```
func_greet:
    # Check parameter count
    LOAD_CONST %r0, #1
    GET_VAR %r1, $__argc
    EQ %r2, %r1, %r0
    JUMP_IF_FALSE @no_default0, %r2
    LOAD_CONST %r3, #"Hello"
    SET_VAR $greeting, %r3
no_default0:
    GET_VAR %r4, $greeting
    PARAM %r4
    GET_VAR %r5, $name
    PARAM %r5
    CALL %r6, $print
    RETURN

entry:
    LOAD_CONST %r7, #"Alice"
    PARAM %r7
    LOAD_CONST %r8, #1
    SET_VAR $__argc, %r8
    CALL %r9, $greet
    
    LOAD_CONST %r10, #"Bob"
    PARAM %r10
    LOAD_CONST %r11, #"Hi"
    PARAM %r11
    LOAD_CONST %r12, #2
    SET_VAR $__argc, %r12
    CALL %r13, $greet
```

**C Code**:
```c
CaesarValue func_greet(int argc, CaesarValue* args) {
    CaesarValue name = args[0];
    CaesarValue greeting = (argc > 1) ? args[1] : make_string("Hello");
    caesar_print(greeting);
    caesar_print(name);
    return make_none();
}
```

### 8. Recursive Functions

**Caesar Syntax**:
```python
def factorial(n):
    if n <= 1:
        return 1
    return n * factorial(n - 1)
```

**IR**: (See Stage 4 example above)

**C Code**:
```c
CaesarValue func_factorial(CaesarValue n) {
    if (compare_le(n, make_int(1)).data.b) {
        return make_int(1);
    }
    return mul_values(n, func_factorial(sub_values(n, make_int(1))));
}
```

**x86-64 Assembly**:
```asm
func_factorial:
    push rbp
    mov rbp, rsp
    cmp rdi, 1             ; Compare n with 1
    jg .recursive          ; Jump if n > 1
    mov rax, 1             ; return 1
    pop rbp
    ret
.recursive:
    push rdi               ; Save n
    dec rdi                ; n - 1
    call func_factorial    ; factorial(n-1)
    pop rdi                ; Restore n
    imul rax, rdi          ; n * factorial(n-1)
    pop rbp
    ret
```

### 9. Lists

**Caesar Syntax**:
```python
numbers = [1, 2, 3, 4, 5]
print(len(numbers))
```

**IR**:
```
ALLOC %r0, #5              # Allocate list with 5 elements
LOAD_CONST %r1, #1
STORE %r0, #0, %r1         # list[0] = 1
LOAD_CONST %r2, #2
STORE %r0, #1, %r2         # list[1] = 2
LOAD_CONST %r3, #3
STORE %r0, #2, %r3         # list[2] = 3
LOAD_CONST %r4, #4
STORE %r0, #3, %r4         # list[3] = 4
LOAD_CONST %r5, #5
STORE %r0, #4, %r5         # list[4] = 5
SET_VAR $numbers, %r0
GET_VAR %r6, $numbers
PARAM %r6
CALL %r7, $len
PARAM %r7
CALL %r8, $print
```

**Status**: 🚧 Partial implementation in IR, needs full compilation support

### 10. Dictionaries

**Caesar Syntax**:
```python
person = {"name": "Alice", "age": 30}
print(len(person))
```

**IR**:
```
ALLOC %r0, #DICT           # Allocate dictionary
LOAD_CONST %r1, #"name"
LOAD_CONST %r2, #"Alice"
DICT_SET %r0, %r1, %r2     # dict["name"] = "Alice"
LOAD_CONST %r3, #"age"
LOAD_CONST %r4, #30
DICT_SET %r0, %r3, %r4     # dict["age"] = 30
SET_VAR $person, %r0
GET_VAR %r5, $person
PARAM %r5
CALL %r6, $len
PARAM %r6
CALL %r7, $print
```

**Status**: 🚧 Partial implementation in IR, needs full compilation support

### 11. Built-in Functions

**Supported Built-ins**:
- `print(*args)`: Output values to console
- `len(obj)`: Get length of string/list/dict
- `range(start, end, step)`: Generate number sequence
- `str(val)`: Convert to string
- `int(val)`: Convert to integer
- `float(val)`: Convert to float
- `type(val)`: Get type of value
- `abs(val)`: Absolute value

**Implementation Strategy**:

Each built-in function needs:
1. **IR Representation**: `CALL %dest, $builtin_name`
2. **VM Implementation**: Native C++ function in VM
3. **C Code**: Function call or inline implementation
4. **Assembly**: Call or inline optimized code

**Example - print() function**:

**C Code**:
```c
void caesar_print(int argc, CaesarValue* args) {
    for (int i = 0; i < argc; i++) {
        switch (args[i].type) {
            case INT: printf("%lld", args[i].data.i); break;
            case FLOAT: printf("%f", args[i].data.f); break;
            case STRING: printf("%s", args[i].data.s); break;
            case BOOL: printf("%s", args[i].data.b ? "True" : "False"); break;
            case NONE: printf("None"); break;
        }
        if (i < argc - 1) printf(" ");
    }
    printf("\n");
}
```

### 12. Classes (Future)

**Caesar Syntax**:
```python
class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y
    
    def distance(self):
        return (self.x ** 2 + self.y ** 2) ** 0.5

p = Point(3, 4)
print(p.distance())
```

**Status**: 📋 Planned, not yet implemented

## Implementation Details

### Current Implementation Status

#### ✅ Completed Features

1. **Lexer**: Full tokenization with indentation tracking
2. **Parser**: Complete AST generation for all syntax
3. **Interpreter**: Tree-walking interpreter for all features
4. **IR Generator**: Basic three-address code generation
5. **Bytecode Generator**: Partial implementation
6. **C Code Generator**: Partial implementation
7. **x86-64 Generator**: Basic structure

#### 🚧 In Progress Features

1. **Complete IR Generation**: All language features
2. **Full Bytecode Compilation**: Complete VM implementation
3. **Complete C Transpiler**: All features to C
4. **Complete x86-64 Generator**: Native code generation

#### 📋 Planned Features

1. **Semantic Analysis**: Type checking and validation
2. **Optimization Passes**: Code optimization
3. **Debug Information**: Source line mapping
4. **ARM64 Backend**: Native ARM compilation
5. **JIT Compilation**: Runtime compilation

### Building the Compiler

**Prerequisites**:
- CMake 3.15+
- C++17 compiler (GCC 8+, Clang 9+, MSVC 2019+)
- Git

**Build Steps**:
```bash
# Clone repository
git clone https://github.com/juliuspleunes4/Caesar.git
cd Caesar

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake ..

# Build
cmake --build . --parallel

# Run tests
ctest --output-on-failure
```

**Build Targets**:
```bash
make caesar          # Main compiler
make caesar_repl     # Interactive REPL
make caesar_lib      # Static library
make test_lexer      # Lexer tests
make test_parser     # Parser tests
make test_ir         # IR tests
make test_codegen    # Code generation tests (to be added)
```

### Testing the Compiler

**Test Structure**:
```
tests/
├── test_lexer.cpp              # Lexer tests
├── test_parser.cpp             # Parser tests
├── test_ir.cpp                 # IR generation tests
├── test_codegen.cpp            # Code generation tests (to be added)
├── test_compiler_e2e.cpp       # End-to-end compilation tests (to be added)
└── test_data/
    ├── syntax/                 # Syntax test files
    ├── semantic/               # Semantic test files
    └── codegen/                # Code generation test files
```

**Test Categories**:

1. **Unit Tests**: Test individual compiler components
   - Lexer token generation
   - Parser AST construction
   - IR generation correctness
   - Code generation output

2. **Integration Tests**: Test compiler pipeline
   - Source → IR → Bytecode
   - Source → IR → C code
   - Source → IR → Assembly

3. **End-to-End Tests**: Test complete compilation
   - Compile and execute programs
   - Verify output correctness
   - Performance benchmarks

4. **Regression Tests**: Ensure fixes don't break
   - Previously fixed bugs
   - Edge cases
   - Error handling

**Running Tests**:
```bash
# Run all tests
cd build && ctest

# Run specific test
./tests/test_ir

# Run with verbose output
ctest --verbose

# Run specific test pattern
ctest -R "ir"
```

### Compiler File Organization

```
Caesar/
├── include/caesar/
│   ├── caesar.h         # Main header
│   ├── token.h          # Token definitions
│   ├── lexer.h          # Lexer interface
│   ├── ast.h            # AST node definitions
│   ├── parser.h         # Parser interface
│   ├── interpreter.h    # Interpreter (for -i flag)
│   ├── ir.h             # IR definitions
│   └── codegen.h        # Code generation interface
├── src/
│   ├── lexer/
│   │   ├── lexer.cpp    # Lexer implementation
│   │   └── token.cpp    # Token utilities
│   ├── parser/
│   │   ├── parser.cpp   # Parser implementation
│   │   └── ast.cpp      # AST node implementations
│   ├── interpreter/
│   │   └── interpreter.cpp  # Interpreter (optional -i)
│   ├── ir/
│   │   └── ir.cpp       # IR generation
│   ├── codegen/
│   │   └── codegen.cpp  # Multi-target code generation
│   ├── optimizer/       # Optimization passes (to be added)
│   │   ├── constant_folding.cpp
│   │   ├── dead_code_elimination.cpp
│   │   └── register_allocation.cpp
│   └── main.cpp         # Compiler driver
├── tests/               # Test files
├── examples/            # Example Caesar programs
└── docs/
    ├── COMPILER.md      # This document
    ├── SYNTAX.md        # Language syntax reference
    └── ARCHITECTURE.md  # System architecture
```

## Usage Guide

### Basic Compilation

**Default behavior** (compile):
```bash
caesar program.csr          # Compiles to bytecode and executes
```

**Interpretation mode** (use interpreter):
```bash
caesar -i program.csr       # Run with tree-walking interpreter
caesar --interpret program.csr
```

### Compilation Targets

**Bytecode compilation**:
```bash
caesar program.csr          # Default: compile and run
caesar --bytecode program.csr -o output.bc  # Generate bytecode file
```

**C code generation** (transpilation):
```bash
caesar --c program.csr -o output.c          # Generate C code
gcc -O3 output.c -o program                 # Compile C code
./program                                   # Run executable
```

**x86-64 assembly generation**:
```bash
caesar --asm program.csr -o output.asm      # Generate assembly
nasm -f elf64 output.asm -o output.o        # Assemble
ld output.o -o program                      # Link
./program                                   # Run executable
```

**ARM64 assembly generation** (future):
```bash
caesar --arm64 program.csr -o output.asm    # Generate ARM assembly
```

### Debug and Analysis Options

**Show token stream**:
```bash
caesar --tokens program.csr
```

**Show AST**:
```bash
caesar --parse program.csr
```

**Show IR**:
```bash
caesar --ir program.csr
```

**Verbose compilation**:
```bash
caesar --verbose program.csr
```

**Optimization levels**:
```bash
caesar -O0 program.csr      # No optimization
caesar -O1 program.csr      # Basic optimization
caesar -O2 program.csr      # Standard optimization (default)
caesar -O3 program.csr      # Aggressive optimization
```

### Complete Command-Line Interface

```
Usage: caesar [options] <input_file>

Options:
  -h, --help              Show help message
  -v, --version           Show version information
  
  Execution modes:
  -i, --interpret         Run with interpreter (tree-walking)
  --compile               Compile to bytecode (default)
  
  Output formats:
  --bytecode              Generate bytecode (.bc)
  --c                     Generate C code (.c)
  --asm, --x86-64         Generate x86-64 assembly (.asm)
  --arm64                 Generate ARM64 assembly (.asm)
  -o <file>               Specify output file
  
  Analysis:
  -t, --tokens            Show token stream
  -p, --parse             Show abstract syntax tree
  --ir                    Show intermediate representation
  
  Optimization:
  -O0                     No optimization
  -O1                     Basic optimization
  -O2                     Standard optimization (default)
  -O3                     Aggressive optimization
  
  Debug:
  --verbose               Verbose output
  --debug                 Include debug information
  --timing                Show compilation timing
  
Examples:
  caesar program.csr                # Compile and run
  caesar -i program.csr             # Interpret
  caesar --c program.csr -o out.c   # Transpile to C
  caesar --ir program.csr           # Show IR
  caesar -O3 program.csr            # Optimize and compile
```

## Optimization Strategies

### Planned Optimization Passes

1. **Constant Folding**
2. **Constant Propagation**
3. **Dead Code Elimination**
4. **Common Subexpression Elimination**
5. **Loop Invariant Code Motion**
6. **Strength Reduction**
7. **Register Allocation**
8. **Inlining**
9. **Tail Call Optimization**

(See Stage 5: Optimization for details)

## Testing Framework

### Test Coverage Requirements

All language features must be tested across:
1. ✅ Lexer tokenization
2. ✅ Parser AST generation
3. 🚧 IR generation
4. 🚧 Bytecode compilation
5. 🚧 C code generation
6. 🚧 x86-64 code generation
7. 🚧 End-to-end execution

### Test Case Categories

1. **Basic Features**: Variables, expressions, operators
2. **Control Flow**: if/elif/else, while, for
3. **Functions**: Definitions, calls, defaults, recursion
4. **Data Structures**: Lists, dictionaries
5. **Built-ins**: All built-in functions
6. **Edge Cases**: Empty inputs, large numbers, deep nesting
7. **Error Handling**: Syntax errors, runtime errors

### Adding New Tests

**Example Test**:
```cpp
#include "caesar/lexer.h"
#include "caesar/parser.h"
#include "caesar/ir.h"
#include "caesar/codegen.h"
#include <cassert>

void test_factorial_compilation() {
    std::string source = R"(
def factorial(n):
    if n <= 1:
        return 1
    return n * factorial(n - 1)

print(factorial(5))
)";
    
    // Lex
    caesar::Lexer lexer(source);
    auto tokens = lexer.tokenize();
    assert(tokens.size() > 0);
    
    // Parse
    caesar::Parser parser(tokens);
    auto program = parser.parse();
    assert(program != nullptr);
    
    // Generate IR
    caesar::IRGenerator ir_gen;
    auto ir = ir_gen.generate(program.get());
    assert(ir.size() > 0);
    
    // Generate bytecode
    auto bytecode_gen = caesar::CodeGeneratorFactory::create(
        caesar::TargetArch::BYTECODE
    );
    std::string bytecode = bytecode_gen->generate(ir);
    assert(!bytecode.empty());
    
    // Execute and verify output
    // (VM execution and output capture needed)
    
    std::cout << "✅ Factorial compilation test passed\n";
}
```

## Future Enhancements

### Short Term (v1.6.0)
- Complete bytecode VM implementation
- Full C code generator
- Complete x86-64 backend
- Basic optimization passes
- Comprehensive test suite

### Medium Term (v2.0.0)
- Semantic analysis and type checking
- Advanced optimizations
- JIT compilation
- ARM64 backend
- Debug information generation

### Long Term (v2.1+)
- Optional static typing
- Module system compilation
- Link-time optimization
- Profile-guided optimization
- LLVM backend integration

## Contributing

### Areas for Contribution

1. **Code Generation**: Complete bytecode/C/assembly generators
2. **Optimization**: Implement optimization passes
3. **Testing**: Add comprehensive test coverage
4. **Documentation**: Expand and improve this document
5. **Performance**: Profile and optimize compiler
6. **Platforms**: Add support for more targets

### Getting Started

1. Read [CONTRIBUTING.md](CONTRIBUTING.md)
2. Check [open issues](https://github.com/juliuspleunes4/Caesar/issues)
3. Join [Discord community](https://discord.gg/BjQraBgQSs)
4. Submit pull requests

## References

### Books and Papers
- "Compilers: Principles, Techniques, and Tools" (Dragon Book)
- "Modern Compiler Implementation in C/Java/ML"
- "Engineering a Compiler" by Cooper and Torczon
- "Advanced Compiler Design and Implementation" by Muchnick

### Online Resources
- [LLVM Documentation](https://llvm.org/docs/)
- [GCC Internals](https://gcc.gnu.org/onlinedocs/gccint/)
- [Crafting Interpreters](https://craftinginterpreters.com/)
- [x86-64 ABI Reference](https://refspecs.linuxbase.org/elf/x86_64-abi-0.99.pdf)

### Similar Projects
- Python (CPython compiler)
- Lua (simple bytecode compiler)
- Julia (JIT compilation)
- V (C transpiler)

---

**Last Updated**: December 2, 2025
**Version**: 1.5.1
**Author**: J.J.G. Pleunes
**License**: MIT
