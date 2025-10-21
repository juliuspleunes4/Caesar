# Caesar Language Syntax Reference

Caesar is a Python-like programming language with C++ performance. This document describes **only the features that are actually implemented** in Caesar v1.5.0, verified against the source code.

**Last Verified:** October 21, 2025  
**Source Code Review:** Complete implementation verification performed

## Table of Contents

- [Comments](#comments)
- [Data Types](#data-types)
- [Enhanced Data Structures](#enhanced-data-structures)
- [Variables](#variables)
- [Operators](#operators)
- [Control Flow](#control-flow)
- [Functions](#functions)
- [Built-in Functions](#built-in-functions)
- [Error Handling](#error-handling)
- [Current Limitations](#current-limitations)
- [Language Constructs](#language-constructs)
- [Future Features](#future-features)
- [Example Programs](#example-programs)

## Comments

Caesar supports single-line comments using the `#` symbol:

```python
# This is a comment
print("Hello, World!")  # This is also a comment
```

## Data Types

Caesar supports the following data types:

### Primitive Types

#### Integers
```python
x = 42
y = -123
z = 0
```

#### Floating Point Numbers
```python
pi = 3.14159
temperature = -273.15
```

#### Strings
```python
name = "Caesar"
message = "Hello, World!"
empty = ""
```

#### Booleans
```python
is_ready = True
is_finished = False
```

#### None
```python
result = None
```

### String Literals

Strings in Caesar are enclosed in double quotes:
```python
greeting = "Hello, World!"
multiword = "This is a longer string"
```

**Note**: Single quotes are not currently supported for string literals.

### Boolean Literals

Caesar uses Python-like uppercase booleans.
```python
true_value = True
false_value = False
```

## Enhanced Data Structures

Caesar v1.4.0 introduces enhanced data structures that provide powerful collection types with familiar syntax.

### Lists

Lists are ordered collections that can hold any type of value.

```python
# Create a list
numbers = [1, 2, 3, 4, 5]
mixed = ["hello", 42, True, 3.14]
empty = []

# Lists can be printed and checked with len()
print(numbers)          # [1, 2, 3, 4, 5]
print(len(numbers))     # 5
print(type(numbers))    # <class 'list'>
```

**Note**: List indexing (e.g., `numbers[0]`) and list methods (e.g., `.append()`) are not yet implemented.

### Dictionaries

Dictionaries are key-value pairs.

```python
# Create a dictionary
person = {"name": "Alice", "age": 30, "city": "New York"}
empty_dict = {}

# Dictionaries can be printed and checked with len()
print(person)           # {"name": "Alice", "age": 30, "city": "New York"}
print(len(person))      # 3
print(type(person))     # <class 'dict'>

# Dictionary examples
config = {
    "debug": True,
    "max_users": 100,
    "version": "1.4.0"
}
```

**Note**: Dictionary indexing (e.g., `person["name"]`) and dictionary methods (e.g., `.keys()`, `.values()`) are not yet implemented.

### Nested Structures

Lists and dictionaries can be nested to create complex data structures.

```python
# List of dictionaries
users = [
    {"name": "Alice", "age": 30},
    {"name": "Bob", "age": 25},
    {"name": "Charlie", "age": 35}
]

# Dictionary with lists
data = {
    "numbers": [1, 2, 3, 4, 5],
    "names": ["Alice", "Bob", "Charlie"],
    "active": True
}

# Nested structures can be created and printed
print(users)
print(data)
print(len(users))       # 3
```

**Note**: Accessing nested elements (e.g., `users[0]["name"]`) is not yet implemented.

### Built-in Functions for Data Structures

Enhanced data structures work with Caesar's built-in functions:

```python
# Using len() with lists and dictionaries
numbers = [1, 2, 3, 4, 5]
person = {"name": "Alice", "age": 30}

print("List length:", len(numbers))    # 5
print("Dict length:", len(person))     # 2

# Using type() to check data structure types
print("Type of list:", type(numbers))      # <class 'list'>
print("Type of dict:", type(person))       # <class 'dict'>

# Empty collections
empty_list = []
empty_dict = {}
print("Empty list length:", len(empty_list))   # 0
print("Empty dict length:", len(empty_dict))   # 0

# Empty lists and dicts are falsy in conditions
if empty_list:
    print("Has items")
else:
    print("Empty")  # This will print
```

### Practical Examples

```python
# Configuration management
config = {
    "debug": True,
    "max_connections": 100,
    "allowed_ips": ["127.0.0.1", "192.168.1.1"],
    "database": {
        "host": "localhost",
        "port": 5432,
        "name": "caesar_app"
    }
}

# Working with mixed data types
mixed_data = [
    "hello",
    42,
    True,
    {"nested": "value"},
    [1, 2, 3]
]

print("Mixed data length:", len(mixed_data))   # 5
print("Mixed data:", mixed_data)               # Prints the full list
```

**Note**: The examples showing indexed access like `mixed_data[0]` or `mixed_data[3]["nested"]` are not yet implemented.

## Variables

### Variable Declaration and Assignment

Variables are declared and assigned using the `=` operator:

```python
# Simple assignment
x = 10
name = "Caesar"
is_active = True

# Multiple assignments
a = 5
b = a
c = a + b
```

### Variable Naming Rules

- Variable names must start with a letter (a-z, A-Z)
- Can contain letters, numbers, and underscores
- Case-sensitive
- Cannot be a reserved keyword

```python
# Valid variable names
counter = 0
user_name = "john"
MAX_SIZE = 100
_private = 42

# Invalid variable names (would cause errors)
# 123invalid = 0  # Cannot start with number
# if = 5          # Cannot use reserved keywords
```

## Operators

### Arithmetic Operators

```python
# Basic arithmetic
a = 10 + 5      # Addition: 15
b = 10 - 3      # Subtraction: 7
c = 4 * 6       # Multiplication: 24
d = 15 / 3      # Division: 5.0
e = 17 % 5      # Modulo: 2
```

### Comparison Operators

```python
# Comparison operations
a = 5 == 5      # Equal: True
b = 5 != 3      # Not equal: True
c = 5 < 10      # Less than: True
d = 5 <= 5      # Less than or equal: True
e = 10 > 5      # Greater than: True
f = 10 >= 10    # Greater than or equal: True
```

### Logical Operators

```python
# Logical operations
a = True and False   # Logical AND: False
b = True or False    # Logical OR: True
c = not True         # Logical NOT: False

# With expressions
result = (x > 0) and (y < 100)
valid = (age >= 18) or (has_permission == True)
```

### Unary Operators

```python
# Unary minus
x = 5
y = -x          # y = -5

# Logical NOT
is_valid = True
is_invalid = not is_valid    # False
```

## Control Flow

### Conditional Statements

#### If Statements

```python
if condition:
    # statements
    pass

if x > 0:
    print("Positive number")
```

#### If-Elif-Else Statements

```python
def check_number(x):
    if x > 0:
        print("Positive")
    elif x < 0:
        print("Negative")
    else:
        print("Zero")
```

### Loops

#### While Loops

```python
# Basic while loop
i = 0
while i < 5:
    print("Count:", i)
    i = i + 1

# While loop with break and continue
i = 0
while i < 10:
    if i == 3:
        i = i + 1
        continue    # Skip iteration
    if i == 7:
        break       # Exit loop
    print(i)
    i = i + 1
```

#### For Loops

For loops work with the `range()` function:

```python
# For loop with range
for i in range(5):
    print("Iteration:", i)    # Prints 0, 1, 2, 3, 4

# For loop with start and end
for i in range(2, 8):
    print(i)                  # Prints 2, 3, 4, 5, 6, 7

# For loop with step
for i in range(0, 10, 2):
    print(i)                  # Prints 0, 2, 4, 6, 8
```

### Loop Control

#### Break Statement

```python
for i in range(10):
    if i == 5:
        break
    print(i)    # Prints 0, 1, 2, 3, 4
```

#### Continue Statement

```python
for i in range(5):
    if i == 2:
        continue
    print(i)    # Prints 0, 1, 3, 4
```

#### Pass Statement

```python
# Placeholder for empty blocks
if condition:
    pass    # Do nothing
else:
    print("Condition was false")
```

## Functions

### Function Definition

Functions are defined using the `def` keyword:

```python
def function_name():
    # function body
    pass

def greet(name):
    print("Hello,", name)

def add(a, b):
    return a + b
```

### Function Parameters

```python
# Function with parameters
def multiply(x, y):
    return x * y

# Function with default parameters
def greet(name, greeting="Hello"):
    print(greeting + ",", name)
```

### Return Statement

```python
def square(x):
    return x * x

def get_max(a, b):
    if a > b:
        return a
    else:
        return b

# Function without explicit return (returns None)
def print_info():
    print("This function returns None")
```

### Function Calls

```python
# Calling functions
result = add(5, 3)
greet("Alice")
max_value = get_max(10, 20)

# Nested function calls
final_result = multiply(add(2, 3), 4)
```

### Recursive Functions

```python
def factorial(n):
    if n <= 1:
        return 1
    else:
        return n * factorial(n - 1)

# Usage
fact_5 = factorial(5)    # Returns 120
```

## Built-in Functions

Caesar provides several built-in functions:

### I/O Functions

#### print()
```python
print("Hello, World!")           # Single argument
print("Name:", name)             # Multiple arguments
print("Value:", 42, "End")       # Mixed types
```

### Utility Functions

#### range()
```python
# range(end)
for i in range(5):          # 0, 1, 2, 3, 4
    print(i)

# range(start, end)
for i in range(2, 7):       # 2, 3, 4, 5, 6
    print(i)

# range(start, end, step)
for i in range(0, 10, 2):   # 0, 2, 4, 6, 8
    print(i)
```

#### len()
```python
text = "Caesar"
length = len(text)      # Returns 6
```

### Type Conversion Functions

#### str()
```python
number = 42
text = str(number)      # "42"
bool_text = str(True)   # "True"
```

#### int()
```python
text_num = "123"
number = int(text_num)  # 123
bool_num = int(True)    # 1
float_num = int(3.14)   # 3
```

#### float()
```python
text_num = "3.14"
number = float(text_num)    # 3.14
int_num = float(42)         # 42.0
bool_num = float(True)      # 1.0
```

#### type()
```python
print(type(42))         # <class 'int'>
print(type("hello"))    # <class 'str'>
print(type(True))       # <class 'bool'>
print(type(None))       # <class 'NoneType'>
```

### Mathematical Functions

#### abs()
```python
positive = abs(-5)      # 5
zero = abs(0)           # 0
float_abs = abs(-3.14)  # 3.14
```

## Error Handling

Caesar provides runtime error detection for common issues:

### Division by Zero
```python
# This will raise a runtime error
result = 10 / 0
```

### Invalid Type Operations
```python
# This will raise a runtime error
result = "text" + 42
```

### Function Call Errors
```python
# Wrong number of arguments
print(len())            # Error: len() takes exactly one argument
print(int(1, 2, 3))     # Error: int() takes exactly one argument
```

## Current Limitations

### What Works in v1.5.0

✅ **Fully Implemented:**
- Primitive types (int, float, str, bool, None)
- All arithmetic and comparison operators
- Variables and assignment
- Control flow (if/elif/else, while, for with range)
- Functions with default parameters
- Recursive functions
- Built-in functions: print, len, range, str, int, float, type, abs
- Lists and dictionaries (creation and len() only)
- Break, continue, pass statements
- `__name__` variable

### What Doesn't Work Yet

❌ **Not Yet Implemented:**
- **Indexing**: `list[0]`, `dict["key"]` will cause errors
- **Assignment via Index**: `list[0] = value` will cause errors
- **List/Dict Methods**: `.append()`, `.keys()`, etc. will cause errors
- **Member Access**: `object.method()` will cause errors
- **Classes**: Parsed but not executed (creates placeholder only)
- **Exception Handling**: No try/except blocks
- **String Methods**: No `.split()`, `.upper()`, etc.
- **List Comprehensions**: Not supported
- **Lambda Functions**: Not supported
- **Import Statements**: Not supported
- **Advanced Built-ins**: No map(), filter(), sum(), min(), max(), input(), etc.

### Working Around Limitations

```python
# ❌ This will NOT work (indexing not implemented):
# my_list = [1, 2, 3]
# first = my_list[0]

# ✅ This works (you can create and print lists):
my_list = [1, 2, 3]
print(my_list)           # [1, 2, 3]
print(len(my_list))      # 3

# ❌ This will NOT work (dict indexing not implemented):
# person = {"name": "Alice"}
# name = person["name"]

# ✅ This works (you can create and print dicts):
person = {"name": "Alice", "age": 30}
print(person)            # {"name": "Alice", "age": 30}
print(len(person))       # 2
```

## Language Constructs

### Block Structure

Caesar uses indentation to define code blocks (like Python):

```python
if condition:
    # This is inside the if block
    print("Condition is true")
    if nested_condition:
        # This is a nested block
        print("Nested condition")
    print("Still in the if block")
# This is outside the if block
print("Outside the block")
```

### Identifiers

Valid identifiers (variable names, function names):
- Must start with a letter (a-z, A-Z) or underscore (_)
- Can contain letters, numbers, and underscores
- Case-sensitive

### Reserved Keywords

The following words are reserved and cannot be used as identifiers:

```
if, elif, else, while, for, in, def, class, return, break, continue, pass,
and, or, not, is, True, False, None
```

**Note**: `class` is recognized as a keyword but class functionality is not fully implemented yet.

### Special Variables

#### __name__
```python
if __name__ == "__main__":
    # Code executed when script is run directly
    main()
```

## Future Features

The following features are planned for future versions of Caesar but are not yet implemented:

### Object-Oriented Programming (Planned)
- **Classes**: Class syntax is parsed but not executed (placeholder only)
- **Methods**: Instance and class methods
- **Inheritance**: Class inheritance and polymorphism
- **Member Access**: Object.method() and Object.property
- **Constructors**: `__init__` and other special methods

### Data Structure Access (Planned)
- **List Indexing**: Access list elements with `list[index]`
- **List Assignment**: Modify list elements with `list[index] = value`
- **Dict Indexing**: Access dictionary values with `dict[key]`
- **Dict Assignment**: Set dictionary values with `dict[key] = value`
- **List Methods**: append(), pop(), remove(), sort(), reverse()
- **Dict Methods**: keys(), values(), items(), get(), pop()
- **List Slicing**: Extract sublists with `list[start:end]`

### Advanced Features (Planned)
- **Import System**: Module imports and packages
- **Exception Handling**: try/except/finally blocks
- **List Comprehensions**: `[x*2 for x in range(10)]`
- **Dict Comprehensions**: `{k: v*2 for k, v in dict.items()}`
- **Generators**: yield statements and iterators
- **Decorators**: Function and class decorators
- **Lambda Functions**: Anonymous functions
- **Context Managers**: with statements

### Extended Built-ins (Planned)
- **File I/O**: open(), read(), write(), close()
- **String Methods**: split(), join(), replace(), upper(), lower(), strip()
- **Advanced Functions**: map(), filter(), zip(), enumerate(), sorted(), reversed()
- **Aggregation**: sum(), min(), max()
- **User Input**: input()
- **Introspection**: isinstance(), hasattr(), getattr(), setattr()
- **Math Functions**: sin(), cos(), sqrt(), pow(), etc.

## Example Programs

### Hello World
```python
def main():
    print("Hello, World!")
    print("Welcome to Caesar programming language!")

if __name__ == "__main__":
    main()
```

### Calculator
```python
def add(a, b):
    return a + b

def multiply(x, y):
    return x * y

def factorial(n):
    if n <= 1:
        return 1
    else:
        return n * factorial(n - 1)

def main():
    sum_result = add(10, 20)
    product = multiply(6, 7)
    fact_5 = factorial(5)
    
    print("Sum:", sum_result)
    print("Product:", product)
    print("Factorial of 5:", fact_5)

if __name__ == "__main__":
    main()
```

### Control Flow
```python
def test_conditionals(x):
    if x > 0:
        print("Positive")
    elif x < 0:
        print("Negative")
    else:
        print("Zero")

def test_loops():
    # While loop
    i = 0
    while i < 5:
        print("Count:", i)
        i = i + 1
    
    # For loop
    for j in range(3):
        print("Iteration:", j)

def main():
    test_conditionals(5)
    test_conditionals(-3)
    test_conditionals(0)
    test_loops()

if __name__ == "__main__":
    main()
```

---

*Caesar Programming Language v1.5.0 - A Python-like language with C++ performance*
