# Manual Test Files

This directory contains manual test files used during development to verify specific Caesar language features.

## Files

- `test_builtins.csr` - Tests built-in functions (print, range, len, etc.)
- `test_defaults.csr` - Tests function default parameters
- `test_fibonacci.csr` - Tests recursive function implementation
- `test_for.csr` - Tests for loop functionality
- `test_function.csr` - Tests function definitions and calls
- `test_hello.csr` - Simple hello world test
- `test_if.csr` - Tests conditional statements
- `test_math.csr` - Tests mathematical operations
- `test_scoping.csr` - Tests variable scoping
- `test_while.csr` - Tests while loop functionality

## Usage

Run any test file with:
```bash
caesar --interpret tests/manual/test_name.csr
```

For automated testing, see the `/tests` directory with the formal test suite.