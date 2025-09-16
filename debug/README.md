# Debug Tools & Test Programs

This directory contains debugging tools and test programs used during Caesar language development. These utilities were instrumental in identifying and resolving parser issues during the systematic debugging process.

## Directory Structure

```
debug/
├── src/           # Debug program source files
├── bin/           # Compiled debug executables (generated)
├── README.md      # This file
└── build.ps1      # Build script for all debug programs
```

## Debug Programs

### Core Parser Testing
- **`debug_simple.cpp`** - Basic statement parsing validation
- **`debug_nesting.cpp`** - Complex nested structure testing
- **`debug_two_level.cpp`** - Two-level nesting validation

### Control Flow Testing  
- **`debug_both_else.cpp`** - if/else statement parsing
- **`debug_indentation.cpp`** - Indentation and DEDENT token generation

### Data Structure Testing
- **`debug_list.cpp`** - List literal parsing `[1, 2, 3]`
- **`debug_multiline_list.cpp`** - Multi-line list structure validation

### Object-Oriented Testing
- **`debug_class.cpp`** - Class definition and member access testing

## Usage

### Building Debug Programs

Use the provided PowerShell script to build all debug programs:

```powershell
# Build all debug programs
.\debug\build.ps1

# Or build individually
g++ -std=c++17 -I. debug\src\debug_simple.cpp build\src\libcaesar_lib.a -o debug\bin\debug_simple.exe
```

### Running Debug Programs

```powershell
# Run a specific debug program
.\debug\bin\debug_simple.exe

# Run all debug programs (validates parser functionality)
Get-ChildItem debug\bin\*.exe | ForEach-Object { & $_.FullName }
```

## Development History

These debug programs were created during the systematic debugging process on **2025-09-16** to resolve critical parser issues:

### Issues Resolved
1. **Nested Control Structures** - `debug_nesting.cpp` identified lexer DEDENT generation bug
2. **Class Definitions** - `debug_class.cpp` validated OOP implementation
3. **Data Structures** - `debug_list.cpp` and `debug_multiline_list.cpp` tested literal parsing
4. **Indentation Handling** - `debug_indentation.cpp` verified Python-style block structure

### Methodology
- **Incremental Testing** - Each debug program tested a specific feature
- **Isolation Strategy** - Minimal test cases to identify root causes
- **Systematic Validation** - Step-by-step feature verification

## Integration with Main Project

These debug tools complement the main test suite (`tests/`) by providing:
- **Targeted Testing** - Specific issue reproduction
- **Development Validation** - Quick feature verification during development
- **Regression Testing** - Ensure fixes don't break existing functionality

## Best Practices

### Creating New Debug Programs
1. **Minimal Examples** - Focus on specific features or issues
2. **Clear Naming** - Use descriptive filenames indicating purpose
3. **Documentation** - Include comments explaining the test case
4. **Error Handling** - Proper exception catching and reporting

### Example Template
```cpp
#include "include/caesar/lexer.h"
#include "include/caesar/parser.h"
#include <iostream>

int main() {
    std::string source = R"(
    // Your test code here
    )";

    try {
        caesar::Lexer lexer(source);
        caesar::Parser parser(lexer);
        auto ast = parser.parse();
        
        std::cout << "✓ Test passed: " << ast->toString() << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cout << "✗ Test failed: " << e.what() << std::endl;
        return 1;
    }
}
```

## Maintenance

### Cleanup
- Debug executables are generated in `bin/` and can be safely deleted
- Source files in `src/` should be preserved for future debugging needs
- Consider archiving old debug programs after major releases

### Updates
- Update debug programs when core language features change
- Add new debug programs for new language features
- Maintain compatibility with current build system

---

*These debug tools were essential in achieving 100% test success rate for the Caesar Programming Language.* 🔧