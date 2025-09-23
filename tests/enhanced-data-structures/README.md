# Enhanced Data Structures Tests

This directory contains test files for Caesar Language v1.4.0 enhanced data structures implementation.

## Features Tested

### Lists
- List creation: `[1, 2, 3]`
- Mixed types: `[1, "hello", 4.5]`
- Nested lists: `[1, [2, 3], 4]`
- Empty lists: `[]`
- List operations: `len([1, 2, 3])` → `3`
- Type checking: `type([1, 2, 3])` → `<class 'list'>`

### Dictionaries
- Dictionary creation: `{"key": "value"}`
- Mixed value types: `{"name": "Caesar", "version": 1.4}`
- Nested dictionaries: `{"data": {"inner": "value"}}`
- Dictionary with lists: `{"numbers": [1, 2, 3]}`
- Empty dictionaries: `{}`
- Dictionary operations: `len({"a": 1, "b": 2})` → `2`
- Type checking: `type({"key": "value"})` → `<class 'dict'>`

## Test Files

### `test_dict.csr`
Basic functionality test covering:
- Dictionary creation and display
- List length operations
- Type checking for both lists and dictionaries

### `test_enhanced_data.csr`
Comprehensive test suite with:
- Boolean literals and type handling
- Complex nested structures
- Variable assignments with data structures
- Edge cases and error conditions

### `test_simple_data.csr`
Simple validation tests for:
- Basic list and dictionary creation
- Fundamental operations
- Quick validation scenarios

## Running Tests

```bash
# Run all enhanced data structure tests
.\dev-run.ps1 tests\enhanced-data-structures\test_dict.csr
.\dev-run.ps1 tests\enhanced-data-structures\test_enhanced_data.csr
.\dev-run.ps1 tests\enhanced-data-structures\test_simple_data.csr

# Or run directly
bin\caesar.exe --interpret tests\enhanced-data-structures\test_dict.csr
```

## Expected Output

### Lists
```
List: [1, 2, 3, hello, 4.500000]
List length: 5
List type: <class 'list'>
```

### Dictionaries
```
Dictionary: {"count": 42, "version": 1.400000, "name": Caesar}
Dictionary length: 3
Dictionary type: <class 'dict'>
```

### Nested Structures
```
Nested list: [1, {"inner": value}, [2, 3]]
Complex dict: {"text": success, "data": [1, 2, 3]}
```

All tests validate the successful implementation of enhanced data structures in Caesar Language v1.4.0.