# Function Implementation Guide for C Code Generator

**Date**: December 3, 2025  
**Status**: Planning Document  
**Target**: Future implementation of user-defined functions in C code generator

## Current Problem

The C code generator currently emits user-defined functions as **labels inside main()** instead of proper C functions. This makes function calls impossible and breaks compiled code.

### Example: What's Generated Now (Broken)

**Caesar Input:**
```python
def add(a, b):
    return a + b

result = add(10, 20)
print(result)
```

**Current C Output (BROKEN):**
```c
int main() {
func_add:
    // DECLARE (not implemented)
    // DECLARE (not implemented)
    int64_t r0 = a;  // ERROR: 'a' undeclared
    int64_t r1 = b;  // ERROR: 'b' undeclared
    int64_t r2 = r0 + r1;
    return r2;  // ERROR: exits main(), not function
    return 0;
    int64_t r3 = 10;
    int64_t r4 = 20;
    // CALL add (not fully implemented)
    // ... rest of main
}
```

**Errors:**
1. Function is a label, not a C function
2. Parameters `a` and `b` are undeclared
3. `return r2;` exits `main()` instead of returning from function
4. CALL instruction doesn't actually call the function

### Example: What Should Be Generated (Correct)

**Needed C Output:**
```c
// Forward declarations
int64_t func_add(int64_t a, int64_t b);

// Function implementations
int64_t func_add(int64_t a, int64_t b) {
    int64_t r0 = a;
    int64_t r1 = b;
    int64_t r2 = r0 + r1;
    return r2;
}

// Main entry point
int main() {
    int64_t r3 = 10;
    int64_t r4 = 20;
    int64_t r5 = func_add(r3, r4);  // Actual function call
    int64_t result;
    result = r5;
    // ... print result
    return 0;
}
```

## Root Cause Analysis

The problem stems from the **single-pass architecture** in `CCodeGenerator::generate()`:

```cpp
std::string CCodeGenerator::generate(const std::vector<BasicBlock>& blocks) {
    output.str("");  // Single output buffer
    
    // Emit headers and runtime
    output << "// headers...\n";
    output << "int main() {\n";
    
    // Process ALL blocks sequentially into main()
    for (const auto& block : blocks) {
        if (!block.label.empty() && block.label != "entry") {
            output << block.label << ":\n";  // Function becomes label!
        }
        for (const auto& instr : block.instructions) {
            emitInstruction(instr);  // All code goes into main
        }
    }
    
    output << "}\n";
    return output.str();
}
```

**Problems:**
1. **Single buffer**: Everything writes to one `output` stream
2. **Sequential processing**: Functions and main code mixed together
3. **No function registry**: Can't track function signatures
4. **No function detection**: Treats function blocks like any other block

## Solution Architecture

### Phase 1: Restructure Data Structures

Add to `CCodeGenerator` class:

```cpp
class CCodeGenerator : public CodeGenerator {
private:
    // Existing
    int indent_level;
    std::unordered_map<std::string, std::string> variable_types;
    std::vector<std::string> pending_params;
    
    // NEW: Separate output buffers
    std::stringstream functions_output;  // For function definitions
    std::stringstream main_output;       // For main() code
    
    // NEW: Function registry
    struct FunctionInfo {
        std::string name;
        std::vector<std::string> parameters;
        std::string return_type;  // Default: int64_t
        std::vector<IRInstruction> body;
    };
    std::unordered_map<std::string, FunctionInfo> function_registry;
    
    // NEW: State tracking
    bool in_function = false;
    std::string current_function_name;
    
    // NEW: Helper methods
    bool isFunctionBlock(const BasicBlock& block);
    void registerFunction(const BasicBlock& block);
    void emitFunctionDefinition(const FunctionInfo& func);
    void emitFunctionCall(const std::string& func_name, const std::vector<std::string>& args);
};
```

### Phase 2: Two-Pass Generation

Restructure the `generate()` method:

```cpp
std::string CCodeGenerator::generate(const std::vector<BasicBlock>& blocks) {
    output.str("");
    functions_output.str("");
    main_output.str("");
    function_registry.clear();
    
    // PASS 1: Identify and register all functions
    for (const auto& block : blocks) {
        if (isFunctionBlock(block)) {
            registerFunction(block);
        }
    }
    
    // PASS 2: Generate code
    // Step 1: Emit headers and runtime
    output << "// Caesar C Code\n";
    output << "// ... headers ...\n";
    output << "// ... runtime functions ...\n\n";
    
    // Step 2: Emit forward declarations
    output << "// Forward declarations\n";
    for (const auto& [name, info] : function_registry) {
        output << info.return_type << " " << sanitizeName(name) 
               << "(";
        for (size_t i = 0; i < info.parameters.size(); i++) {
            if (i > 0) output << ", ";
            output << "int64_t " << sanitizeName(info.parameters[i]);
        }
        output << ");\n";
    }
    output << "\n";
    
    // Step 3: Emit function definitions
    output << "// Function implementations\n";
    for (const auto& [name, info] : function_registry) {
        emitFunctionDefinition(info);
    }
    output << "\n";
    
    // Step 4: Emit main()
    output << "int main() {\n";
    indent_level = 1;
    
    for (const auto& block : blocks) {
        if (!isFunctionBlock(block)) {
            // Process non-function blocks into main
            for (const auto& instr : block.instructions) {
                emitInstruction(instr);
            }
        }
    }
    
    indent_level = 0;
    output << "    return 0;\n";
    output << "}\n";
    
    return output.str();
}
```

### Phase 3: Implement Helper Methods

#### Detect Function Blocks

```cpp
bool CCodeGenerator::isFunctionBlock(const BasicBlock& block) {
    // Function blocks have labels like "func_name"
    return !block.label.empty() 
        && block.label != "entry"
        && block.label.substr(0, 5) == "func_";
}
```

#### Register Functions

```cpp
void CCodeGenerator::registerFunction(const BasicBlock& block) {
    FunctionInfo info;
    info.name = block.label;  // e.g., "func_add"
    info.return_type = "int64_t";  // Default, could infer later
    
    // Extract parameters from DECLARE instructions
    for (const auto& instr : block.instructions) {
        if (instr.opcode == IROpcode::DECLARE) {
            info.parameters.push_back(instr.dest.value);
        }
    }
    
    // Store all instructions (we'll process them later)
    info.body = block.instructions;
    
    function_registry[info.name] = info;
}
```

#### Emit Function Definition

```cpp
void CCodeGenerator::emitFunctionDefinition(const FunctionInfo& func) {
    // Function signature
    output << func.return_type << " " << sanitizeName(func.name) << "(";
    
    for (size_t i = 0; i < func.parameters.size(); i++) {
        if (i > 0) output << ", ";
        output << "int64_t " << sanitizeName(func.parameters[i]);
    }
    output << ") {\n";
    
    indent_level = 1;
    in_function = true;
    current_function_name = func.name;
    
    // Function body (skip DECLARE instructions, we handled them)
    for (const auto& instr : func.body) {
        if (instr.opcode != IROpcode::DECLARE) {
            emitInstruction(instr);
        }
    }
    
    in_function = false;
    indent_level = 0;
    output << "}\n\n";
}
```

### Phase 4: Handle CALL Instructions

Modify `emitInstruction()` to handle user-defined functions:

```cpp
case IROpcode::CALL: {
    std::string func_name = instr.src1.value;
    
    // Check if it's a user-defined function
    if (function_registry.count("func_" + func_name) > 0) {
        // User-defined function
        std::string result_reg = sanitizeName(instr.dest.value);
        std::string call_name = sanitizeName("func_" + func_name);
        
        // Build argument list from pending_params
        std::stringstream args;
        for (size_t i = 0; i < pending_params.size(); i++) {
            if (i > 0) args << ", ";
            args << sanitizeName(pending_params[i]);
        }
        
        emitLine("int64_t " + result_reg + " = " + call_name 
                 + "(" + args.str() + ");");
        variable_types[instr.dest.value] = "int64_t";
        pending_params.clear();
    }
    else if (func_name == "print" || func_name == "len" || ...) {
        // Built-in functions (existing code)
        // ... current implementation ...
    }
    else {
        emitLine("// ERROR: Unknown function: " + func_name);
        pending_params.clear();
    }
    break;
}
```

## Implementation Checklist

### Phase 1: Data Structures (1-2 days)
- [ ] Add `functions_output` and `main_output` buffers
- [ ] Add `FunctionInfo` struct
- [ ] Add `function_registry` map
- [ ] Add state tracking variables
- [ ] Add helper method declarations

### Phase 2: Two-Pass Architecture (2-3 days)
- [ ] Restructure `generate()` method
- [ ] Implement Pass 1: Function identification
- [ ] Implement Pass 2: Separate generation
- [ ] Test with simple function example

### Phase 3: Helper Methods (1-2 days)
- [ ] Implement `isFunctionBlock()`
- [ ] Implement `registerFunction()`
- [ ] Implement `emitFunctionDefinition()`
- [ ] Test function registration

### Phase 4: CALL Handler (2-3 days)
- [ ] Modify CALL case in `emitInstruction()`
- [ ] Handle user-defined function calls
- [ ] Handle parameter passing
- [ ] Test function calls

### Phase 5: Testing (3-5 days)
Per IMPLEMENTATION_PLAN.md, minimum 20 test cases:
- [ ] Functions with no parameters
- [ ] Functions with 1 parameter
- [ ] Functions with multiple parameters
- [ ] Functions returning int
- [ ] Functions returning float
- [ ] Functions returning string
- [ ] Functions returning bool
- [ ] Recursive functions (factorial)
- [ ] Mutually recursive functions
- [ ] Functions calling other functions
- [ ] Functions with local variables
- [ ] Functions with nested calls
- [ ] Functions with conditionals
- [ ] Functions with loops
- [ ] Edge case: empty function
- [ ] Edge case: single-line function
- [ ] Edge case: deeply nested calls
- [ ] Edge case: many parameters
- [ ] Integration test: multiple functions
- [ ] Integration test: complex program

## Testing Strategy

Create `tests/test_c_functions.cpp`:

```cpp
bool test_simple_function() {
    std::string code = R"(
def add(a, b):
    return a + b

result = add(10, 20)
print(result)
)";
    return compileAndTest(code, "simple_function", "30\n");
}

bool test_recursive_function() {
    std::string code = R"(
def factorial(n):
    if n <= 1:
        return 1
    return n * factorial(n - 1)

result = factorial(5)
print(result)
)";
    return compileAndTest(code, "recursive", "120\n");
}

// ... 18 more tests ...
```

## Estimated Timeline

- **Phase 1**: 1-2 days (data structures)
- **Phase 2**: 2-3 days (two-pass architecture)
- **Phase 3**: 1-2 days (helper methods)
- **Phase 4**: 2-3 days (CALL handler)
- **Phase 5**: 3-5 days (comprehensive testing)

**Total**: 9-15 days for production-quality implementation

## Success Criteria

1. All existing tests continue to pass (24/24)
2. At least 20 new function tests pass
3. Can compile and run recursive functions
4. Can compile and run mutually recursive functions
5. Function calls work with correct parameter passing
6. Return values work correctly
7. No memory leaks in generated code
8. Generated C code compiles without warnings

## References

- `docs/COMPILER.md` - Current status and limitations
- `docs/IMPLEMENTATION_PLAN.md` - Phase 1.1 details
- `src/codegen/codegen.cpp` - Current implementation
- `src/ir/ir.cpp` - IR generation (already working)
- `tests/test_c_*.cpp` - Existing test patterns

## Notes

1. This is a **major architectural change** requiring careful implementation
2. Follow the principle: **"No shortcuts, extensive testing, production quality"**
3. Complete one phase fully before moving to the next
4. Update COMPILER.md after each phase
5. Keep the implementation incremental and testable

---

**Last Updated**: December 3, 2025  
**Author**: Based on analysis during documentation update session  
**Status**: Ready for implementation
