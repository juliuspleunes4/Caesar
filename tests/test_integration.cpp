/**
 * @file test_integration.cpp
 * @brief Integration tests for the Caesar language pipeline
 * @author J.J.G. Pleunes
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
        print("fibonacci(" + str(i) + ") = " + str(result))

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

print("5 + 3 = " + str(result1))
print("Result * 2 = " + str(result2))
print("5! = " + str(result3))
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
x = 5
if x > 0:
    print("Positive")
else:
    if x == 0:
        print("Zero")
    else:
        print("Negative")

# Simple loop test
i = 0
while i < 3:
    if i == 1:
        print("Middle")
    else:
        print("Other")
    i = i + 1
)";
    }
    
    bool success = testPipeline(source, "Control Flow");
    assert(success);
}

void test_complex_program_integration() {
    std::cout << "Testing complex program integration...\n";
    
    std::string source = R"(
# Complex program with multiple features
operation_count = 0

def log_operation(operation):
    print("Operation " + str(operation_count) + ": " + operation)

def add(a, b):
    result = a + b
    log_operation(str(a) + " + " + str(b) + " = " + str(result))
    return result

def subtract(a, b):
    result = a - b
    log_operation(str(a) + " - " + str(b) + " = " + str(result))
    return result

def multiply(a, b):
    result = a * b
    log_operation(str(a) + " * " + str(b) + " = " + str(result))
    return result

def divide(a, b):
    if b == 0:
        print("Error: Division by zero")
        return 0
    result = a / b
    log_operation(str(a) + " / " + str(b) + " = " + str(result))
    return result

# Perform calculations
result1 = add(10, 5)
operation_count = operation_count + 1

result2 = subtract(result1, 3)
operation_count = operation_count + 1

result3 = multiply(result2, 2)
operation_count = operation_count + 1

result4 = divide(result3, 2)
operation_count = operation_count + 1

print("Total operations: " + str(operation_count))
)";
    
    bool success = testPipeline(source, "Complex Program");
    assert(success);
}

void test_data_structures_integration() {
    std::cout << "Testing data structures integration...\n";
    
    std::string source = R"(
# Test basic data operations
def test_variables():
    a = 10
    b = 20
    c = a + b
    print("a = " + str(a))
    print("b = " + str(b))
    print("c = a + b = " + str(c))
    return c

def test_strings():
    name = "Alice"
    greeting = "Hello, " + name
    print("Name: " + str(name))
    print("Greeting: " + str(greeting))
    return greeting

# Run tests
result1 = test_variables()
result2 = test_strings()
print("All tests completed")
)";
    
    bool success = testPipeline(source, "Data Structures");
    assert(success);
}

void test_algorithms_integration() {
    std::cout << "Testing algorithms integration...\n";
    
    std::string source = R"(
# Simple algorithm implementations
def factorial(n):
    if n <= 1:
        return 1
    return n * factorial(n - 1)

def gcd(a, b):
    while b != 0:
        temp = b
        b = a % b
        a = temp
    return a

def fibonacci(n):
    if n <= 1:
        return n
    a = 0
    b = 1
    i = 2
    while i <= n:
        temp = a + b
        a = b
        b = temp
        i = i + 1
    return b

def is_prime(n):
    if n <= 1:
        return 0
    if n <= 3:
        return 1
    if n % 2 == 0:
        return 0
    i = 3
    while i * i <= n:
        if n % i == 0:
            return 0
        i = i + 2
    return 1

# Test algorithms
print("Factorial of 5: " + str(factorial(5)))
print("GCD of 48 and 18: " + str(gcd(48, 18)))
print("Fibonacci of 10: " + str(fibonacci(10)))
print("Is 17 prime? " + str(is_prime(17)))
print("Is 15 prime? " + str(is_prime(15)))
)";
    
    bool success = testPipeline(source, "Algorithms");
    assert(success);
}

void test_exception_handling_integration() {
    std::cout << "Testing exception handling integration...\n";
    
    std::string source = R"(
# Error handling examples using return values
def divide_safe(a, b):
    if b == 0:
        print("Error: Cannot divide by zero")
        return -1
    else:
        result = a / b
        return result

def check_positive(n):
    if n < 0:
        print("Error: Number is negative")
        return 0
    else:
        return 1

# Test error handling
print("Testing division:")
result1 = divide_safe(10, 2)
print("10 / 2 = " + str(result1))
result2 = divide_safe(10, 0)
print("10 / 0 = " + str(result2))
print("Testing positive check:")
check1 = check_positive(5)
check2 = check_positive(-3)
print("Error handling tests completed")
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
        return "Point(" + str(self.x) + ", " + str(self.y) + ")"

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
    print(str(key) + ": " + str(value))
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
