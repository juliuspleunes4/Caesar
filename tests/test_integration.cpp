/**
 * @file test_integration.cpp
 * @brief Integration tests for the Caesar language pipeline
 * @author Julius Pleunes
 * @version 1.0.0
 */

#undef NDEBUG
#include "caesar/lexer.h"
#include "caesar/parser.h"
#include "caesar/ast.h"
#include <iostream>
#include <cassert>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>

// Ensure std types are available
using std::vector;

// Simple assert replacement for debugging
#define my_assert(condition) \
    do { \
        if (!(condition)) { \
            std::cerr << "Assertion failed: " << #condition << " at line " << __LINE__ << std::endl; \
            std::abort(); \
        } \
    } while(0)

#ifndef assert
#define assert my_assert
#endif

// Helper function to read file contents
std::string readFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Warning: Could not open file " << filepath << std::endl;
        return "";
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Helper function to test complete pipeline
bool testPipeline(const std::string& source, const std::string& testName) {
    std::cout << "Testing " << testName << "...\n";
    
    try {
        // Step 1: Lexical analysis
        caesar::Lexer lexer(source);
        auto tokens = lexer.tokenize();
        
        if (tokens.empty()) {
            std::cerr << "❌ " << testName << ": No tokens produced\n";
            return false;
        }
        
        // Step 2: Parsing
        caesar::Parser parser(std::move(tokens));
        auto program = parser.parse();
        
        if (!program) {
            std::cerr << "❌ " << testName << ": Parser returned null program\n";
            return false;
        }
        
        // Step 3: AST validation
        std::string ast_string = program->toString();
        if (ast_string.empty()) {
            std::cerr << "❌ " << testName << ": AST toString returned empty string\n";
            return false;
        }
        
        std::cout << "✓ " << testName << " pipeline test passed\n";
        std::cout << "AST Preview: " << ast_string.substr(0, 100) << "...\n";
        return true;
    } catch (const std::exception& e) {
        std::cerr << "❌ " << testName << " failed with exception: " << e.what() << "\n";
        return false;
    }
}

void test_hello_world_integration() {
    std::cout << "Testing hello world integration...\n";
    
    // Try to read from examples directory first
    std::string source = readFile("../examples/hello_world.csr");
    
    // If file doesn't exist, use embedded source
    if (source.empty()) {
        source = R"(
# Hello World Program
print("Hello, World!")
print("Welcome to Caesar!")
)";
    }
    
    bool success = testPipeline(source, "Hello World");
    assert(success);
}

void test_fibonacci_integration() {
    std::cout << "Testing fibonacci integration...\n";
    
    std::string source = readFile("../examples/fibonacci.csr");
    
    if (source.empty()) {
        source = R"(
def fibonacci(n):
    if n <= 1:
        return n
    else:
        return fibonacci(n - 1) + fibonacci(n - 2)

def main():
    for i in range(10):
        result = fibonacci(i)
        print(f"fibonacci({i}) = {result}")

main()
)";
    }
    
    bool success = testPipeline(source, "Fibonacci");
    assert(success);
}

void test_functions_integration() {
    std::cout << "Testing functions integration...\n";
    
    std::string source = readFile("../examples/functions.csr");
    
    if (source.empty()) {
        source = R"(
# Function definitions and calls
def add(a, b):
    return a + b

def multiply(x, y):
    return x * y

def factorial(n):
    if n <= 1:
        return 1
    else:
        return n * factorial(n - 1)

# Function calls
result1 = add(5, 3)
result2 = multiply(result1, 2)
result3 = factorial(5)

print(f"5 + 3 = {result1}")
print(f"Result * 2 = {result2}")
print(f"5! = {result3}")
)";
    }
    
    bool success = testPipeline(source, "Functions");
    assert(success);
}

void test_control_flow_integration() {
    std::cout << "Testing control flow integration...\n";
    
    std::string source = readFile("../examples/control_flow.csr");
    
    if (source.empty()) {
        source = R"(
# Control flow examples
def test_conditionals(x):
    if x > 0:
        print("Positive")
    elif x == 0:
        print("Zero")
    else:
        print("Negative")

def test_loops():
    # For loop
    for i in range(5):
        if i == 2:
            continue
        if i == 4:
            break
        print(f"For loop: {i}")
    
    # While loop
    count = 0
    while count < 3:
        print(f"While loop: {count}")
        count = count + 1

# Test the functions
test_conditionals(5)
test_conditionals(0)
test_conditionals(-3)
test_loops()
)";
    }
    
    bool success = testPipeline(source, "Control Flow");
    assert(success);
}

void test_complex_program_integration() {
    std::cout << "Testing complex program integration...\n";
    
    std::string source = R"(
# Complex program with multiple features
class Calculator:
    def __init__(self):
        self.history = []
    
    def add(self, a, b):
        result = a + b
        self.history.append(f"{a} + {b} = {result}")
        return result
    
    def subtract(self, a, b):
        result = a - b
        self.history.append(f"{a} - {b} = {result}")
        return result
    
    def multiply(self, a, b):
        result = a * b
        self.history.append(f"{a} * {b} = {result}")
        return result
    
    def divide(self, a, b):
        if b == 0:
            raise ValueError("Division by zero")
        result = a / b
        self.history.append(f"{a} / {b} = {result}")
        return result
    
    def get_history(self):
        return self.history

def main():
    calc = Calculator()
    
    # Perform calculations
    result1 = calc.add(10, 5)
    result2 = calc.subtract(result1, 3)
    result3 = calc.multiply(result2, 2)
    
    try:
        result4 = calc.divide(result3, 2)
        result5 = calc.divide(100, 0)  # This should raise an error
    except ValueError as e:
        print(f"Error: {e}")
    
    # Print history
    history = calc.get_history()
    for operation in history:
        print(operation)

if __name__ == "__main__":
    main()
)";
    
    bool success = testPipeline(source, "Complex Program");
    assert(success);
}

void test_data_structures_integration() {
    std::cout << "Testing data structures integration...\n";
    
    std::string source = R"(
# Test various data structures
def test_lists():
    # List operations
    numbers = [1, 2, 3, 4, 5]
    numbers.append(6)
    numbers.insert(0, 0)
    
    for num in numbers:
        print(f"Number: {num}")
    
    return numbers

def test_dictionaries():
    # Dictionary operations
    person = {
        "name": "Alice",
        "age": 30,
        "city": "New York"
    }
    
    person["email"] = "alice@example.com"
    
    for key in person:
        print(f"{key}: {person[key]}")
    
    return person

def test_strings():
    # String operations
    text = "Hello, World!"
    uppercase = text.upper()
    lowercase = text.lower()
    length = len(text)
    
    print(f"Original: {text}")
    print(f"Uppercase: {uppercase}")
    print(f"Lowercase: {lowercase}")
    print(f"Length: {length}")
    
    return text

# Run tests
numbers = test_lists()
person = test_dictionaries()
text = test_strings()
)";
    
    bool success = testPipeline(source, "Data Structures");
    assert(success);
}

void test_algorithms_integration() {
    std::cout << "Testing algorithms integration...\n";
    
    std::string source = R"(
# Algorithm implementations
def bubble_sort(arr):
    n = len(arr)
    for i in range(n):
        for j in range(0, n - i - 1):
            if arr[j] > arr[j + 1]:
                arr[j], arr[j + 1] = arr[j + 1], arr[j]
    return arr

def binary_search(arr, target):
    left = 0
    right = len(arr) - 1
    
    while left <= right:
        mid = (left + right) // 2
        if arr[mid] == target:
            return mid
        elif arr[mid] < target:
            left = mid + 1
        else:
            right = mid - 1
    
    return -1

def quick_sort(arr):
    if len(arr) <= 1:
        return arr
    
    pivot = arr[len(arr) // 2]
    left = [x for x in arr if x < pivot]
    middle = [x for x in arr if x == pivot]
    right = [x for x in arr if x > pivot]
    
    return quick_sort(left) + middle + quick_sort(right)

# Test algorithms
test_array = [64, 34, 25, 12, 22, 11, 90]
print(f"Original array: {test_array}")

sorted_bubble = bubble_sort(test_array.copy())
print(f"Bubble sorted: {sorted_bubble}")

sorted_quick = quick_sort(test_array.copy())
print(f"Quick sorted: {sorted_quick}")

index = binary_search(sorted_quick, 25)
print(f"Index of 25: {index}")
)";
    
    bool success = testPipeline(source, "Algorithms");
    assert(success);
}

void test_exception_handling_integration() {
    std::cout << "Testing exception handling integration...\n";
    
    std::string source = R"(
# Exception handling examples
def divide_safe(a, b):
    try:
        result = a / b
        return result
    except ZeroDivisionError:
        print("Error: Cannot divide by zero")
        return None
    except Exception as e:
        print(f"Unexpected error: {e}")
        return None

def file_operations():
    try:
        with open("test.txt", "r") as file:
            content = file.read()
            return content
    except FileNotFoundError:
        print("File not found")
        return None
    except IOError as e:
        print(f"IO Error: {e}")
        return None
    finally:
        print("File operation completed")

def custom_exception_example():
    class CustomError(Exception):
        def __init__(self, message):
            self.message = message
            super().__init__(self.message)
    
    try:
        raise CustomError("This is a custom error")
    except CustomError as e:
        print(f"Caught custom error: {e.message}")

# Test exception handling
result1 = divide_safe(10, 2)
result2 = divide_safe(10, 0)

content = file_operations()
custom_exception_example()
)";
    
    bool success = testPipeline(source, "Exception Handling");
    assert(success);
}

void test_mixed_features_integration() {
    std::cout << "Testing mixed features integration...\n";
    
    std::string source = R"DELIMITER(
# Program using multiple language features together
import math

class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y
    
    def distance_to(self, other):
        dx = self.x - other.x
        dy = self.y - other.y
        return math.sqrt(dx * dx + dy * dy)
    
    def __str__(self):
        return f"Point({self.x}, {self.y})"

def process_points(points):
    # Using list comprehension, lambda, and built-in functions
    distances = [p1.distance_to(p2) for p1, p2 in zip(points[:-1], points[1:])]
    
    total_distance = sum(distances)
    average_distance = total_distance / len(distances) if distances else 0
    
    # Using filter and lambda
    close_points = list(filter(lambda d: d < average_distance, distances))
    
    return {
        "total_distance": total_distance,
        "average_distance": average_distance,
        "close_points_count": len(close_points)
    }

# Create test data
points = [
    Point(0, 0),
    Point(1, 1),
    Point(2, 0),
    Point(3, 1),
    Point(4, 0)
]

# Process points
result = process_points(points)

# Display results
for point in points:
    print(point)

for key, value in result.items():
    print(f"{key}: {value}")
)DELIMITER";
}

int main() {
    std::cout << "Running Caesar integration tests...\n\n";
    
    try {
        test_hello_world_integration();
        test_fibonacci_integration();
        test_functions_integration();
        test_control_flow_integration();
        test_complex_program_integration();
        test_data_structures_integration();
        test_algorithms_integration();
        test_exception_handling_integration();
        test_mixed_features_integration();
        
        std::cout << "\n✅ All integration tests passed!\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "\n❌ Integration test failed: " << e.what() << "\n";
        return 1;
    }
}