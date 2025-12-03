# Caesar Compiler Implementation Plan

## Overview

This document outlines the systematic, production-quality implementation plan for completing the Caesar compiler. Each feature must be implemented with comprehensive testing before moving to the next.

## Guiding Principles

1. **No Shortcuts**: Implement features properly, not quick workarounds
2. **Extensive Testing**: Test all edge cases before considering a feature complete
3. **Production Quality**: Code must be maintainable, documented, and robust
4. **Incremental Approach**: Complete one feature fully before starting the next
5. **Clear Documentation**: Update COMPILER.md with implementation status

## Phase 1: C Code Generator (Complete Implementation)

### Priority: HIGH | Estimated Effort: 3-5 days

**Goal**: Make C code generator production-ready with full function support

#### 1.1 Restructure C Code Generation Architecture

**Current Problem**: Functions are generated as labels, not C functions

**Solution**:
```cpp
class CCodeGenerator {
private:
    std::stringstream functions_code;  // Separate buffer for functions
    std::stringstream main_code;       // Buffer for main code
    std::unordered_map<std::string, FunctionInfo> function_registry;
    
    struct FunctionInfo {
        std::string name;
        std::vector<std::string> parameters;
        std::string return_type;
    };
};
```

**Implementation Steps**:
1. Separate function generation from main code generation
2. Build function registry during first pass
3. Generate proper C function signatures
4. Generate function bodies with proper parameter handling
5. Generate main() that calls functions properly

**Test Cases Required** (minimum 20):
- Functions with no parameters
- Functions with 1 parameter
- Functions with multiple parameters
- Functions returning int, float, string, bool
- Recursive functions
- Mutually recursive functions
- Functions calling built-in functions
- Functions with local variables
- Functions with nested calls
- Edge cases: empty functions, single-line functions, etc.

#### 1.2 Implement Built-in Function Runtime

**Create**: `runtime/caesar_runtime.h`

```c
// Caesar Runtime Library for C Code Generation

#ifndef CAESAR_RUNTIME_H
#define CAESAR_RUNTIME_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

// Value type for Caesar
typedef enum {
    CAESAR_INT,
    CAESAR_FLOAT,
    CAESAR_STRING,
    CAESAR_BOOL,
    CAESAR_NONE,
    CAESAR_LIST,
    CAESAR_DICT
} CaesarType;

typedef struct CaesarValue {
    CaesarType type;
    union {
        int64_t i;
        double f;
        char* s;
        bool b;
        void* ptr;  // For lists/dicts
    } data;
} CaesarValue;

// Built-in functions
void caesar_print(int argc, CaesarValue* args);
CaesarValue caesar_range(int start, int stop, int step);
CaesarValue caesar_len(CaesarValue val);
CaesarValue caesar_str(CaesarValue val);
CaesarValue caesar_int(CaesarValue val);
CaesarValue caesar_float(CaesarValue val);
CaesarValue caesar_type(CaesarValue val);
CaesarValue caesar_abs(CaesarValue val);

// Memory management
CaesarValue caesar_make_int(int64_t val);
CaesarValue caesar_make_float(double val);
CaesarValue caesar_make_string(const char* val);
CaesarValue caesar_make_bool(bool val);
CaesarValue caesar_make_none();

#endif
```

**Test Cases Required** (minimum 30):
- Each built-in function with valid inputs
- Each built-in function with invalid inputs
- Edge cases: empty strings, zero, negative numbers
- Memory leak tests
- Type conversion edge cases

#### 1.3 Implement Data Structures in C

**Lists**:
```c
typedef struct CaesarList {
    CaesarValue* items;
    size_t length;
    size_t capacity;
} CaesarList;

CaesarValue caesar_list_create();
void caesar_list_append(CaesarValue list, CaesarValue item);
CaesarValue caesar_list_get(CaesarValue list, int64_t index);
void caesar_list_set(CaesarValue list, int64_t index, CaesarValue value);
```

**Dictionaries**:
```c
typedef struct CaesarDict {
    // Hash table implementation
    struct Entry* entries;
    size_t length;
    size_t capacity;
} CaesarDict;

CaesarValue caesar_dict_create();
void caesar_dict_set(CaesarValue dict, const char* key, CaesarValue value);
CaesarValue caesar_dict_get(CaesarValue dict, const char* key);
bool caesar_dict_has(CaesarValue dict, const char* key);
```

**Test Cases Required** (minimum 40):
- List creation and appending
- List indexing (positive and negative)
- List slicing
- Empty lists
- Large lists (performance)
- Dict creation and setting
- Dict key lookups
- Missing keys
- Key collisions
- Empty dicts
- Large dicts (performance)

#### 1.4 End-to-End C Compilation Tests

**Create**: Comprehensive test suite that:
1. Generates C code for Caesar programs
2. Compiles C code with GCC
3. Executes compiled binary
4. Verifies output matches expected

**Test Programs** (minimum 25):
- Hello world
- Arithmetic operations
- Variables and assignment
- Functions (simple and complex)
- Recursion (fibonacci, factorial, ackermann)
- Control flow (all combinations)
- Loops (while, for, nested)
- Built-in functions
- Lists
- Dictionaries
- Mixed features
- Edge cases

**Estimated Completion**: After 150+ test cases pass

## Phase 2: Bytecode Virtual Machine

### Priority: HIGH | Estimated Effort: 5-7 days

**Goal**: Implement production-ready bytecode VM with full feature support

#### 2.1 Design Production-Ready Value System

**Problem**: std::variant with recursive types causes template issues

**Solution**: Use inheritance-based approach

```cpp
class VMValue {
public:
    enum class Type { NONE, INT, FLOAT, STRING, BOOL, LIST, DICT, FUNCTION };
    
    virtual ~VMValue() = default;
    virtual Type getType() const = 0;
    virtual std::string toString() const = 0;
    virtual VMValue* clone() const = 0;
};

class IntValue : public VMValue {
    int64_t value;
public:
    explicit IntValue(int64_t v) : value(v) {}
    Type getType() const override { return Type::INT; }
    int64_t getValue() const { return value; }
    // ... other methods
};

// Similar for Float, String, Bool, List, Dict, Function
```

**Benefits**:
- No recursive template issues
- Clean polymorphic design
- Easy to extend
- Proper memory management with smart pointers

#### 2.2 Implement Call Stack and Frame Management

```cpp
struct CallFrame {
    std::string function_name;
    size_t return_address;
    std::unique_ptr<Scope> local_scope;
    std::vector<std::unique_ptr<VMValue>> parameters;
};

class CallStack {
    std::vector<CallFrame> frames;
public:
    void push(CallFrame frame);
    CallFrame pop();
    CallFrame& current();
    size_t depth() const;
};
```

**Test Cases Required** (minimum 25):
- Simple function calls
- Nested function calls
- Recursive calls
- Tail recursion
- Call stack limits
- Parameter passing
- Return values
- Local variable scope

#### 2.3 Implement All VM Instructions

**Complete implementation for**:
- Arithmetic (ADD, SUB, MUL, DIV, MOD, NEG)
- Comparison (EQ, NE, LT, LE, GT, GE)
- Logical (AND, OR, NOT)
- Memory (LOAD, STORE, LOAD_CONST, ALLOC)
- Control (LABEL, JUMP, JUMP_IF_TRUE, JUMP_IF_FALSE)
- Functions (CALL, RETURN, PARAM)
- Variables (DECLARE, ASSIGN, GET_VAR, SET_VAR)

**Test Cases Required** (minimum 100):
- Each instruction with valid operands
- Each instruction with edge cases
- Instruction combinations
- Error conditions
- Performance tests

#### 2.4 Implement Built-in Functions in VM

**Each built-in needs**:
- Full implementation
- Type checking
- Error handling
- Edge case handling
- Performance optimization

**Built-ins to implement**:
- print(), range(), len(), str(), int(), float(), type(), abs()

**Test Cases Required** (minimum 50):
- Each function with various input types
- Edge cases
- Error conditions
- Performance tests

#### 2.5 Memory Management

**Implement**:
- Garbage collection or reference counting
- Memory leak detection
- Resource cleanup

**Test Cases Required** (minimum 20):
- Memory allocation/deallocation
- Circular references
- Large allocations
- Memory leak tests

**Estimated Completion**: After 250+ test cases pass

## Phase 3: Semantic Analysis

### Priority: MEDIUM | Estimated Effort: 4-6 days

#### 3.1 Symbol Table Implementation

```cpp
class SymbolTable {
    std::vector<std::unordered_map<std::string, Symbol>> scopes;
public:
    void enterScope();
    void exitScope();
    void declare(const std::string& name, Symbol symbol);
    Symbol* lookup(const std::string& name);
    bool isDefined(const std::string& name) const;
};

struct Symbol {
    std::string name;
    SymbolType type;  // Variable, Function, Class
    DataType dataType;  // int, float, string, etc.
    bool is_mutable;
    Position declaration_pos;
};
```

#### 3.2 Type Checking

**Implement**:
- Type inference for expressions
- Type checking for operations
- Function signature validation
- Return type checking

**Test Cases Required** (minimum 60):
- Valid type combinations
- Invalid type combinations
- Type coercion
- Function argument types
- Return type mismatches

#### 3.3 Scope Resolution

**Check for**:
- Undefined variables
- Duplicate definitions
- Shadowing
- Closure variable access

**Test Cases Required** (minimum 40):
- Valid scoping
- Invalid references
- Nested scopes
- Closure captures

**Estimated Completion**: After 100+ test cases pass

## Phase 4: Optimization Passes

### Priority: MEDIUM | Estimated Effort: 5-7 days

#### 4.1 Constant Folding

**Implement**:
- Compile-time evaluation of constant expressions
- Propagation of known values

**Test Cases**: 30+ programs with constant expressions

#### 4.2 Dead Code Elimination

**Implement**:
- Remove unreachable code
- Remove unused variables
- Remove no-op instructions

**Test Cases**: 25+ programs with dead code

#### 4.3 Common Subexpression Elimination

**Implement**:
- Identify repeated computations
- Reuse computed values

**Test Cases**: 20+ programs with redundant computations

#### 4.4 Register Allocation

**Implement**:
- Graph coloring algorithm
- Spilling strategy
- Live range analysis

**Test Cases**: 30+ programs testing register pressure

**Estimated Completion**: After 105+ test cases pass

## Phase 5: x86-64 Native Code Generation

### Priority: LOW | Estimated Effort: 7-10 days

#### 5.1 Complete Function Calls

- Implement proper calling conventions
- Stack frame management
- Parameter passing via registers/stack
- Return value handling

#### 5.2 System Integration

- System calls for I/O
- Dynamic library linking
- ELF/PE file generation

#### 5.3 Optimization

- Register allocation
- Instruction selection
- Peephole optimization

**Estimated Completion**: After 150+ test cases pass

## Testing Strategy

### Test Categories

1. **Unit Tests**: Individual components (30% of total tests)
2. **Integration Tests**: Component interactions (30% of total tests)
3. **End-to-End Tests**: Full compilation and execution (30% of total tests)
4. **Performance Tests**: Benchmarks and stress tests (5% of total tests)
5. **Regression Tests**: Previously fixed bugs (5% of total tests)

### Test Coverage Goals

- **Line Coverage**: Minimum 80%
- **Branch Coverage**: Minimum 70%
- **Path Coverage**: Critical paths 100%

### Testing Tools

- Google Test for C++ unit tests
- Custom test harness for Caesar programs
- Valgrind for memory leak detection
- Performance profiling tools

## Success Criteria

### Phase 1 Complete When:
- [ ] 150+ C code generation tests pass
- [ ] Can compile and run all example programs via C
- [ ] No memory leaks in generated C code
- [ ] Performance within 2x of hand-written C

### Phase 2 Complete When:
- [ ] 250+ VM tests pass
- [ ] Can execute all example programs with VM
- [ ] No memory leaks in VM
- [ ] Performance within 5x of interpreter

### Phase 3 Complete When:
- [ ] 100+ semantic analysis tests pass
- [ ] Catches all type errors
- [ ] Catches all undefined variable errors
- [ ] Provides helpful error messages

### Phase 4 Complete When:
- [ ] 105+ optimization tests pass
- [ ] Measurable performance improvements
- [ ] No regressions in correctness

### Phase 5 Complete When:
- [ ] 150+ native code tests pass
- [ ] Performance matches C code
- [ ] Platform-specific tests pass

## Timeline Estimate

**Total Estimated Time**: 24-35 days

- Phase 1: 3-5 days
- Phase 2: 5-7 days
- Phase 3: 4-6 days
- Phase 4: 5-7 days
- Phase 5: 7-10 days

**Note**: These are working days with focused development. Actual calendar time will vary.

## Current Progress

- [x] Architecture documented
- [x] Implementation plan created
- [ ] Phase 1 started
- [ ] Phase 2 started
- [ ] Phase 3 started
- [ ] Phase 4 started
- [ ] Phase 5 started

## Risks and Mitigation

### Risk 1: Template/Type System Complexity
**Impact**: High
**Probability**: Medium
**Mitigation**: Use inheritance-based approach for VM values

### Risk 2: Performance Goals Not Met
**Impact**: Medium
**Probability**: Low
**Mitigation**: Profile early, optimize critical paths

### Risk 3: Memory Management Issues
**Impact**: High
**Probability**: Medium
**Mitigation**: Extensive testing with Valgrind, smart pointers

### Risk 4: Platform-Specific Issues
**Impact**: Low
**Probability**: Medium
**Mitigation**: Test on multiple platforms, abstract OS-specific code

## Maintenance Plan

### Code Quality Standards

- All code must be reviewed
- All code must have comments
- All code must pass static analysis
- All code must have tests

### Documentation Requirements

- Update COMPILER.md after each feature
- Maintain this implementation plan
- Keep API documentation current
- Document known limitations

### Continuous Integration

- Run all tests on every commit
- Check code coverage
- Run memory leak detection
- Performance benchmarks

## Conclusion

This plan provides a systematic, production-quality approach to completing the Caesar compiler. By following this plan and maintaining the principles of no shortcuts and extensive testing, the compiler will be robust, maintainable, and production-ready.

**Last Updated**: 2025-12-03
**Status**: Phase 1 Ready to Start
