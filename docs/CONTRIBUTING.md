# Contributing to Caesar Programming Language

Thank you for your interest in contributing to Caesar! This document provides guidelines and information for contributors.

## Table of Contents
- [Code of Conduct](#code-of-conduct)
- [Getting Started](#getting-started)
- [Development Environment](#development-environment)
- [Contributing Guidelines](#contributing-guidelines)
- [Pull Request Process](#pull-request-process)
- [Coding Standards](#coding-standards)
- [Testing Guidelines](#testing-guidelines)
- [Documentation](#documentation)

## Code of Conduct

By participating in this project, you agree to abide by our [Code of Conduct](CODE_OF_CONDUCT.md). Please read it before contributing.

## Getting Started

### Ways to Contribute

- 🐛 **Report Bugs** - Found an issue? Let us know!
- 💡 **Suggest Features** - Have ideas for improvements?
- 📝 **Improve Documentation** - Help make Caesar more accessible
- 🔧 **Fix Issues** - Solve existing problems
- ✨ **Add Features** - Implement new language features
- 🧪 **Write Tests** - Improve test coverage
- 🎨 **Improve Examples** - Create better learning materials

### Finding Issues to Work On

- Check [GitHub Issues](https://github.com/juliuspleunes4/Caesar/issues)
- Look for labels:
  - `good-first-issue` - Perfect for newcomers
  - `help-wanted` - We'd love assistance
  - `bug` - Something is broken
  - `enhancement` - New feature requests
  - `documentation` - Documentation improvements

## Development Environment

### Prerequisites

- **CMake 3.15+**
- **C++17 compatible compiler** (GCC 8+, Clang 9+, MSVC 2019+)
- **Git**
- **Text editor/IDE** (VS Code, CLion, etc.)

### Setup

1. **Fork the repository**
   ```bash
   # Click "Fork" on GitHub, then clone your fork
   git clone https://github.com/YOUR_USERNAME/Caesar.git
   cd Caesar
   ```

2. **Set up development environment**
   ```bash
   # Add upstream remote
   git remote add upstream https://github.com/juliuspleunes4/Caesar.git
   
   # Create development branch
   git checkout -b feature/your-feature-name
   ```

3. **Build the project**
   ```bash
   mkdir build && cd build
   cmake ..
   cmake --build . --parallel
   ```

4. **Run tests to ensure everything works**
   ```bash
   ctest --output-on-failure
   ```

### Development Workflow

```bash
# Keep your fork updated
git fetch upstream
git checkout main
git merge upstream/main
git push origin main

# Create feature branch
git checkout -b feature/amazing-feature

# Make changes, commit frequently
git add .
git commit -m "feat: add amazing feature"

# Push and create Pull Request
git push origin feature/amazing-feature
```

## Contributing Guidelines

### Issue Reporting

When reporting bugs:

1. **Check existing issues** first
2. **Use the bug report template**
3. **Provide minimal reproduction case**
4. **Include system information:**
   - OS and version
   - Compiler version
   - Caesar version
   - CMake version

### Feature Requests

When suggesting features:

1. **Check if it's already requested**
2. **Explain the use case** and motivation
3. **Provide examples** of how it would work
4. **Consider implementation complexity**

### Code Contributions

1. **Start with an issue** - Discuss before implementing
2. **Follow coding standards** (see below)
3. **Write tests** for new functionality
4. **Update documentation** as needed
5. **Keep commits focused** and atomic

## Pull Request Process

### Before Creating a Pull Request

- [ ] Code follows project style guidelines
- [ ] All tests pass (`ctest`)
- [ ] New functionality includes tests
- [ ] Documentation is updated
- [ ] Commit messages follow conventions
- [ ] No merge conflicts with main branch

### Pull Request Template

```markdown
## Description
Brief description of changes

## Type of Change
- [ ] Bug fix
- [ ] New feature
- [ ] Breaking change
- [ ] Documentation update

## Testing
- [ ] All existing tests pass
- [ ] New tests added for new functionality
- [ ] Manual testing completed

## Checklist
- [ ] Code follows style guidelines
- [ ] Self-review completed
- [ ] Documentation updated
- [ ] No breaking changes (or clearly documented)
```

### Review Process

1. **Automated checks** must pass (builds, tests)
2. **Code review** by maintainers
3. **Address feedback** promptly
4. **Squash commits** if requested
5. **Merge** after approval

## Coding Standards

### C++ Style Guidelines

```cpp
// File: src/example/example.cpp

#include "caesar/example.h"
#include <iostream>
#include <memory>

namespace caesar {

// Class names: PascalCase
class ExampleClass {
public:
    // Public members first
    ExampleClass(int value);
    
    // Method names: camelCase
    void processData();
    int getValue() const;
    
private:
    // Private members with trailing underscore
    int value_;
    std::unique_ptr<SomeType> data_;
};

// Function names: camelCase
bool isValidInput(const std::string& input) {
    // Use early returns
    if (input.empty()) {
        return false;
    }
    
    // Prefer const auto for type deduction
    const auto length = input.length();
    
    // Use meaningful variable names
    return length > 0 && length < 1000;
}

} // namespace caesar
```

### Code Organization

```
src/
├── lexer/          # Lexical analysis
├── parser/         # Syntax analysis  
├── interpreter/    # Code execution
├── ast/           # Abstract syntax tree
└── utils/         # Utility functions

include/caesar/     # Public headers
tests/             # Test files
examples/          # Example programs
docs/              # Documentation
```

### Naming Conventions

- **Files:** `snake_case.cpp`, `snake_case.h`
- **Classes:** `PascalCase`
- **Functions/Methods:** `camelCase`
- **Variables:** `snake_case`
- **Constants:** `UPPER_CASE`
- **Private members:** `trailing_underscore_`

### Comments and Documentation

```cpp
/**
 * @brief Brief description of the function
 * 
 * Detailed description of what the function does,
 * its parameters, and return value.
 * 
 * @param input The input string to process
 * @param options Processing options
 * @return true if processing successful, false otherwise
 * 
 * @throws std::invalid_argument if input is invalid
 * 
 * Example usage:
 * @code
 * if (processInput("hello", options)) {
 *     // Success
 * }
 * @endcode
 */
bool processInput(const std::string& input, const Options& options);
```

## Testing Guidelines

### Test Structure

```cpp
// File: tests/test_feature.cpp

#include "caesar/feature.h"
#include <gtest/gtest.h>

class FeatureTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup test data
    }
    
    void TearDown() override {
        // Cleanup
    }
    
    FeatureClass feature_;
};

TEST_F(FeatureTest, BasicFunctionality) {
    // Arrange
    const std::string input = "test input";
    
    // Act
    auto result = feature_.process(input);
    
    // Assert
    EXPECT_TRUE(result.isValid());
    EXPECT_EQ("expected output", result.getValue());
}

TEST_F(FeatureTest, ErrorHandling) {
    // Test error conditions
    EXPECT_THROW(feature_.process(""), std::invalid_argument);
}
```

### Test Categories

1. **Unit Tests** - Test individual components
2. **Integration Tests** - Test component interactions
3. **End-to-End Tests** - Test complete workflows
4. **Performance Tests** - Test performance characteristics

### Running Tests

```bash
# All tests
ctest

# Specific test
./build/tests/test_lexer

# With verbose output
ctest --output-on-failure --verbose

# Performance tests
./build/tests/test_stress
```

## Documentation

### Documentation Types

1. **Code Documentation** - Inline comments and Doxygen
2. **User Documentation** - How to use Caesar
3. **Developer Documentation** - How Caesar works internally
4. **API Documentation** - Generated from code comments

### Writing Documentation

- Use clear, concise language
- Provide examples
- Keep it up-to-date with code changes
- Follow markdown conventions

### Building Documentation

```bash
# Install Doxygen
sudo apt-get install doxygen  # Ubuntu
brew install doxygen          # macOS

# Generate documentation
cd build
make doc

# View documentation
open docs/html/index.html
```

## Commit Message Guidelines

### Format

```
type(scope): description

[optional body]

[optional footer]
```

### Types

- **feat:** New feature
- **fix:** Bug fix
- **docs:** Documentation changes
- **style:** Code style changes (formatting, etc.)
- **refactor:** Code refactoring
- **test:** Adding or updating tests
- **chore:** Maintenance tasks

### Examples

```bash
feat(lexer): add support for hexadecimal literals

Add tokenization support for hexadecimal numbers like 0xFF.
This enables more flexible numeric input in Caesar programs.

Closes #123

fix(parser): resolve operator precedence issue

Binary operators were not respecting mathematical precedence.
Fixed by updating the precedence table in parser.cpp.

docs(contributing): add testing guidelines

Added comprehensive testing guidelines including:
- Test structure examples
- Running test instructions  
- Performance testing info
```

## Release Process

### Versioning

Caesar follows [Semantic Versioning](https://semver.org/):

- **MAJOR.MINOR.PATCH** (e.g., 1.2.3)
- **MAJOR:** Breaking changes
- **MINOR:** New features (backward compatible)
- **PATCH:** Bug fixes (backward compatible)

### Creating Releases

1. Update version numbers
2. Update CHANGELOG.md
3. Create release branch
4. Generate release package
5. Create GitHub release
6. Announce release

## Getting Help

### Development Questions

- **GitHub Discussions** - General questions
- **GitHub Issues** - Bug reports and feature requests
- **Code Review** - Ask during pull request process

### Communication

- Be respectful and constructive
- Ask questions if unsure
- Provide context for your contributions
- Be patient with the review process

### Resources

- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/)
- [CMake Documentation](https://cmake.org/documentation/)
- [Git Best Practices](https://git-scm.com/book)
- [Semantic Versioning](https://semver.org/)

---

Thank you for contributing to Caesar! Your efforts help make programming more accessible and enjoyable for everyone. 🏛️