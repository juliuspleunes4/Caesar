# Caesar Language Examples

This directory contains example programs demonstrating various features of the Caesar programming language.

## Files

- `fibonacci.csr` - Recursive Fibonacci calculation
- `hello_world.csr` - Simple "Hello, World!" program
- `control_flow.csr` - Control flow examples (if/else, while, for)
- `functions.csr` - Function definition and calling examples

## Running Examples

To run any example:

```bash
# From the build directory
./src/caesar -t ../examples/fibonacci.csr    # Show tokens
./src/caesar ../examples/fibonacci.csr       # Parse and show AST
```

Note: Full execution requires IR generation and code generation, which are planned for future implementation.