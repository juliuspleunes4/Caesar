# Simple Caesar Examples (No Functions)

These examples demonstrate Caesar features that work with **both** the interpreter (`-i` flag) and C code compilation (`--c` flag).

## Why These Examples?

Caesar's C code generator is currently limited - **user-defined functions are not yet supported** in compiled code. These examples showcase what **does work**:

- ✅ Variables and assignments
- ✅ Arithmetic operations (+, -, *, /, %)
- ✅ Boolean logic (and, or, not)
- ✅ Comparison operators (<, >, ==, !=, <=, >=)
- ✅ Control flow (if/elif/else)
- ✅ Loops (while, for with range())
- ✅ Built-in functions (print, len, str, int, float, type, abs)
- ✅ Mixed type operations (int + float)
- ❌ User-defined functions (not yet working in C compilation)
- ❌ Classes (not yet working in C compilation)

## Running the Examples

### With Interpreter (All Features Work)
```bash
caesar -i examples/simple_arithmetic.csr
caesar -i examples/simple_loops.csr
caesar -i examples/simple_conditionals.csr
caesar -i examples/builtin_functions.csr
```

### With C Compilation (Limited Features)
```bash
# Compile to C
caesar --c examples/simple_arithmetic.csr -o /tmp/test.c

# Compile C to executable
gcc -std=c11 /tmp/test.c -o /tmp/test -lm

# Run the executable
/tmp/test
```

## Example Files

### `simple_arithmetic.csr`
Demonstrates basic arithmetic operations, mixed-type operations, and boolean logic.
- Integer and float arithmetic
- Type promotion (int + float = float)
- Boolean operations

### `simple_loops.csr`
Demonstrates loop constructs that work without functions.
- While loops with counters
- For loops with range()
- Range with start, stop, and step parameters
- Nested loops

### `simple_conditionals.csr`
Demonstrates control flow without functions.
- Basic if-else statements
- If-elif-else chains
- Nested conditionals
- Comparison operations

### `builtin_functions.csr`
Demonstrates all built-in functions that work in compiled code.
- `print()` - output to console
- `len()` - length of strings
- `str()` - convert to string
- `int()` - convert to integer
- `float()` - convert to floating point
- `type()` - get type name
- `abs()` - absolute value

## Notes

1. **String Escape Sequences**: Currently, `\n` in string literals may not work correctly in C compilation. Use separate `print()` statements instead.

2. **Boolean Output**: The interpreter prints booleans as `True`/`False`, while C compilation prints them as `1`/`0`.

3. **Function Support**: For examples using functions, use the interpreter mode (`-i` flag) until function support is added to the C code generator. See `docs/COMPILER.md` and `docs/IMPLEMENTATION_PLAN.md` for the roadmap.

## Full Examples (Functions Required)

These examples in the parent directory require the interpreter:
- `hello_world.csr` - uses `main()` function
- `fibonacci.csr` - uses recursive functions
- `functions.csr` - demonstrates function features
- `control_flow.csr` - uses functions for organization

Run them with:
```bash
caesar -i examples/hello_world.csr
caesar -i examples/fibonacci.csr
```

## Current Status

**Date**: December 3, 2024
**C Code Generator**: 75% complete (functions not yet supported)
**Testing**: 24/24 tests passing (100% for supported features)

See `docs/COMPILER.md` for complete implementation status and roadmap.
