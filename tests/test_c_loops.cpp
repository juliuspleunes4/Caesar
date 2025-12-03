/**
 * @file test_c_loops.cpp
 * @brief Comprehensive tests for C code generation - loop structures
 * @version 1.0.0
 */

#include "caesar/caesar.h"
#include "caesar/lexer.h"
#include "caesar/parser.h"
#include "caesar/ir.h"
#include "caesar/codegen.h"
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <ctime>
#include <unistd.h>

using namespace caesar;

// Comprehensive loop testing including while and for loops
const int TOTAL_TESTS = 30;  // 10 while loops + 20 for/mixed loops

// Helper to generate unique temporary file names
std::string getTempFileName() {
    return "/tmp/test_loop_" + std::to_string(time(nullptr)) + "_" + 
           std::to_string(getpid()) + "_" + std::to_string(rand()) + ".c";
}

// Helper to compile and verify C code
bool compileAndVerify(const std::string& c_code) {
    std::string temp_file = getTempFileName();
    std::string exe_file = temp_file + ".exe";
    
    // Write C code to file
    std::ofstream out(temp_file);
    if (!out) {
        std::cerr << "Failed to create temp file: " << temp_file << std::endl;
        return false;
    }
    out << c_code;
    out.close();
    
    // Compile with gcc
    std::string compile_cmd = "gcc -std=c11 -o " + exe_file + " " + temp_file + " 2>&1";
    int result = system(compile_cmd.c_str());
    
    // Clean up
    remove(temp_file.c_str());
    if (result == 0) {
        remove(exe_file.c_str());
    }
    
    return result == 0;
}

// Test helper
bool testLoop(const std::string& caesar_code, const std::string& test_name) {
    try {
        // Lex
        Lexer lexer(caesar_code);
        auto tokens = lexer.tokenize();
        
        // Parse
        Parser parser(tokens);
        auto program = parser.parse();
        
        // Generate IR
        IRGenerator ir_gen;
        auto blocks = ir_gen.generate(program.get());
        
        // Generate C code
        auto c_gen = CodeGeneratorFactory::createCGenerator();
        std::string c_code = c_gen->generate(blocks);
        
        // Verify C code compiles
        if (!compileAndVerify(c_code)) {
            std::cerr << "FAIL: " << test_name << " - C code did not compile" << std::endl;
            std::cerr << "Generated C code:\n" << c_code << std::endl;
            return false;
        }
        
        std::cout << "PASS: " << test_name << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "FAIL: " << test_name << " - Exception: " << e.what() << std::endl;
        return false;
    }
}

// ============================================================================
// While Loop Tests
// ============================================================================

void test_simple_while() {
    std::string code = R"(
x = 0
while x < 5:
    x = x + 1
)";
    assert(testLoop(code, "Simple while loop"));
}

void test_while_with_multiple_statements() {
    std::string code = R"(
x = 0
y = 0
while x < 10:
    x = x + 1
    y = y + 2
)";
    assert(testLoop(code, "While loop with multiple statements"));
}

void test_nested_while_loops() {
    std::string code = R"(
i = 0
while i < 3:
    j = 0
    while j < 3:
        j = j + 1
    i = i + 1
)";
    assert(testLoop(code, "Nested while loops"));
}

void test_while_with_break_condition() {
    std::string code = R"(
x = 0
while x < 100:
    x = x + 1
    if x == 10:
        x = 100
)";
    assert(testLoop(code, "While loop with break condition"));
}

void test_while_with_complex_condition() {
    std::string code = R"(
x = 0
y = 10
while x < 5 and y > 0:
    x = x + 1
    y = y - 1
)";
    assert(testLoop(code, "While loop with complex condition"));
}

void test_while_zero_iterations() {
    std::string code = R"(
x = 10
while x < 5:
    x = x + 1
)";
    assert(testLoop(code, "While loop with zero iterations"));
}

void test_while_with_negative_numbers() {
    std::string code = R"(
x = -5
while x < 0:
    x = x + 1
)";
    assert(testLoop(code, "While loop with negative numbers"));
}

void test_while_with_floats() {
    std::string code = R"(
x = 0.0
while x < 5.0:
    x = x + 0.5
)";
    assert(testLoop(code, "While loop with floats"));
}

void test_deeply_nested_while() {
    std::string code = R"(
i = 0
while i < 2:
    j = 0
    while j < 2:
        k = 0
        while k < 2:
            k = k + 1
        j = j + 1
    i = i + 1
)";
    assert(testLoop(code, "Deeply nested while loops"));
}

void test_while_with_multiple_conditions() {
    std::string code = R"(
x = 0
y = 0
z = 0
while x < 5 and y < 10 and z < 15:
    x = x + 1
    y = y + 2
    z = z + 3
)";
    assert(testLoop(code, "While loop with multiple conditions"));
}

// ============================================================================
// For Loop Tests (range-based)
// ============================================================================
// For Loop Tests
// ============================================================================

void test_simple_for_loop() {
    std::string code = R"(
total = 0
for i in range(5):
    total = total + i
)";
    assert(testLoop(code, "Simple for loop"));
}

void test_for_loop_with_start_stop() {
    std::string code = R"(
total = 0
for i in range(2, 8):
    total = total + i
)";
    assert(testLoop(code, "For loop with start and stop"));
}

void test_for_loop_with_step() {
    std::string code = R"(
total = 0
for i in range(0, 10, 2):
    total = total + i
)";
    assert(testLoop(code, "For loop with step"));
}

void test_for_loop_negative_range() {
    std::string code = R"(
total = 0
for i in range(-5, 5):
    total = total + i
)";
    assert(testLoop(code, "For loop with negative range"));
}

void test_for_loop_descending() {
    std::string code = R"(
total = 0
for i in range(10, 0, -1):
    total = total + i
)";
    assert(testLoop(code, "For loop descending"));
}

void test_nested_for_loops() {
    std::string code = R"(
total = 0
for i in range(3):
    for j in range(3):
        total = total + i + j
)";
    assert(testLoop(code, "Nested for loops"));
}

void test_for_loop_with_if() {
    std::string code = R"(
total = 0
for i in range(10):
    if i % 2 == 0:
        total = total + i
)";
    assert(testLoop(code, "For loop with if statement"));
}

void test_for_loop_empty_range() {
    std::string code = R"(
total = 0
for i in range(5, 5):
    total = total + i
)";
    assert(testLoop(code, "For loop with empty range"));
}

void test_for_loop_large_range() {
    std::string code = R"(
total = 0
for i in range(100):
    total = total + 1
)";
    assert(testLoop(code, "For loop with large range"));
}

void test_for_loop_with_multiple_statements() {
    std::string code = R"(
x = 0
y = 0
for i in range(5):
    x = x + i
    y = y + i * 2
)";
    assert(testLoop(code, "For loop with multiple statements"));
}

// ============================================================================
// Mixed Loop Tests
// ============================================================================

void test_while_then_for() {
    std::string code = R"(
x = 0
while x < 3:
    x = x + 1

total = 0
for i in range(5):
    total = total + i
)";
    assert(testLoop(code, "While loop followed by for loop"));
}

void test_for_inside_while() {
    std::string code = R"(
i = 0
while i < 3:
    total = 0
    for j in range(5):
        total = total + j
    i = i + 1
)";
    assert(testLoop(code, "For loop inside while loop"));
}

void test_while_inside_for() {
    std::string code = R"(
for i in range(3):
    j = 0
    while j < 5:
        j = j + 1
)";
    assert(testLoop(code, "While loop inside for loop"));
}

void test_complex_nested_loops() {
    std::string code = R"(
result = 0
for i in range(3):
    x = 0
    while x < 3:
        for k in range(2):
            result = result + i + x + k
        x = x + 1
)";
    assert(testLoop(code, "Complex nested loops (for/while/for)"));
}

void test_loop_with_multiple_variables() {
    std::string code = R"(
a = 0
b = 0
c = 0
for i in range(10):
    a = a + i
    b = b + i * 2
    c = c + i * 3
)";
    assert(testLoop(code, "Loop with multiple variables"));
}

void test_loop_with_conditional_assignment() {
    std::string code = R"(
result = 0
for i in range(20):
    if i % 5 == 0:
        result = result + i
    elif i % 3 == 0:
        result = result + i * 2
    else:
        result = result + 1
)";
    assert(testLoop(code, "Loop with conditional assignment"));
}

void test_loop_with_boolean_operations() {
    std::string code = R"(
count = 0
for i in range(15):
    if i > 5 and i < 10:
        count = count + 1
    elif i <= 5 or i >= 10:
        count = count + 2
)";
    assert(testLoop(code, "Loop with boolean operations"));
}

void test_loop_with_float_arithmetic() {
    std::string code = R"(
total = 0.0
for i in range(10):
    total = total + 1.5
)";
    assert(testLoop(code, "Loop with float arithmetic"));
}

void test_loop_variable_reuse() {
    std::string code = R"(
total = 0
for i in range(5):
    total = total + i

for i in range(10):
    total = total + i
)";
    assert(testLoop(code, "Loop variable reuse"));
}

void test_loop_with_negative_step() {
    std::string code = R"(
total = 0
for i in range(20, 10, -2):
    total = total + i
)";
    assert(testLoop(code, "Loop with negative step"));
}

// ============================================================================
// Main Test Runner
// ============================================================================

int main() {
    std::cout << "Running comprehensive loop tests for C code generation..." << std::endl;
    std::cout << "Total tests: " << TOTAL_TESTS << std::endl << std::endl;
    
    int passed = 0;
    
    // While loop tests
    test_simple_while(); passed++;
    test_while_with_multiple_statements(); passed++;
    test_nested_while_loops(); passed++;
    test_while_with_break_condition(); passed++;
    test_while_with_complex_condition(); passed++;
    test_while_zero_iterations(); passed++;
    test_while_with_negative_numbers(); passed++;
    test_while_with_floats(); passed++;
    test_deeply_nested_while(); passed++;
    test_while_with_multiple_conditions(); passed++;
    
    // For loop tests
    test_simple_for_loop(); passed++;
    test_for_loop_with_start_stop(); passed++;
    test_for_loop_with_step(); passed++;
    test_for_loop_negative_range(); passed++;
    test_for_loop_descending(); passed++;
    test_nested_for_loops(); passed++;
    test_for_loop_with_if(); passed++;
    test_for_loop_empty_range(); passed++;
    test_for_loop_large_range(); passed++;
    test_for_loop_with_multiple_statements(); passed++;
    
    // Mixed loop tests
    test_while_then_for(); passed++;
    test_for_inside_while(); passed++;
    test_while_inside_for(); passed++;
    test_complex_nested_loops(); passed++;
    test_loop_with_multiple_variables(); passed++;
    test_loop_with_conditional_assignment(); passed++;
    test_loop_with_boolean_operations(); passed++;
    test_loop_with_float_arithmetic(); passed++;
    test_loop_variable_reuse(); passed++;
    test_loop_with_negative_step(); passed++;
    
    std::cout << "\n========================================" << std::endl;
    std::cout << "Loop Tests Complete: " << passed << "/" << TOTAL_TESTS << " passed" << std::endl;
    std::cout << "========================================" << std::endl;
    
    if (passed != TOTAL_TESTS) {
        std::cerr << "Warning: Expected " << TOTAL_TESTS << " tests but " << passed << " passed" << std::endl;
    }
    
    return (passed == TOTAL_TESTS) ? 0 : 1;
}
