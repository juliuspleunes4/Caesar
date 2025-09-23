# Caesar Language Tests

This directory contains various test suites for the Caesar programming language.

## Directory Structure

```
tests/
├── enhanced-data-structures/  # Tests for lists and dictionaries (v1.4.0)
├── manual/                    # Manual test scripts for language features
├── comparison/                # Comparison tests with other languages
├── CMakeLists.txt            # CMake configuration for C++ unit tests
└── test_*.cpp                # C++ unit test files
```

## Test Categories

### C++ Unit Tests
- `test_lexer.cpp` - Lexical analysis tests
- `test_parser.cpp` - Parser functionality tests
- `test_lexer_advanced.cpp` - Advanced lexer features
- `test_parser_advanced.cpp` - Complex parsing scenarios
- `test_integration.cpp` - End-to-end integration tests
- `test_stress.cpp` - Performance and stress tests
- `test_error_handling.cpp` - Error handling validation
- `test_build_system.cpp` - Build system validation
- `test_coverage_analysis.cpp` - Code coverage analysis

### Enhanced Data Structures Tests (v1.4.0)
- `test_dict.csr` - Dictionary functionality tests
- `test_enhanced_data.csr` - Comprehensive data structures tests
- `test_simple_data.csr` - Basic data structures validation

### Manual Test Scripts
Located in `manual/` directory - these are Caesar language files (.csr) that test specific language features:
- Functions, control flow, built-ins, etc.

## Running Tests

### C++ Unit Tests
```bash
# Build and run all tests
.\build-and-test.ps1

# Run specific test
cd build && ctest -R test_lexer --output-on-failure
```

### Enhanced Data Structures Tests
```bash
# Run with development script
.\dev-run.ps1 tests\enhanced-data-structures\test_dict.csr

# Run directly
bin\caesar.exe --interpret tests\enhanced-data-structures\test_dict.csr
```

### Manual Tests
```bash
.\dev-run.ps1 tests\manual\test_function.csr
```

All tests should pass with the current implementation. The C++ unit tests validate core language functionality, while the .csr test files validate practical usage scenarios.