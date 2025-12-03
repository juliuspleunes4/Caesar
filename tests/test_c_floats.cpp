/**
 * @file test_c_floats.cpp
 * @brief Comprehensive C code generation tests for float/double operations
 * @author Caesar Compiler Project
 * @version 1.0.0
 * 
 * Tests all floating-point features with edge cases following production-quality standards.
 */

#include "caesar/lexer.h"
#include "caesar/parser.h"
#include "caesar/ir.h"
#include "caesar/codegen.h"
#include <iostream>
#include <cassert>
#include <fstream>
#include <cstdlib>
#include <unistd.h>

using namespace caesar;

// Helper function to test C code generation and GCC compilation
bool testCCodeGen(const std::string& caesarCode, std::string& cCode) {
    try {
        // Lex, parse, generate IR
        Lexer lexer(caesarCode);
        auto tokens = lexer.tokenize();
        Parser parser(tokens);
        auto program = parser.parse();
        IRGenerator ir_gen;
        auto ir_blocks = ir_gen.generate(program.get());
        
        // Generate C code
        auto c_gen = CodeGeneratorFactory::createCGenerator();
        cCode = c_gen->generate(ir_blocks);
        
        // Write to temporary file with unique name
        // Use multiple sources of entropy for better uniqueness
        srand(time(nullptr) + getpid() * 31);
        std::string tempFile = "/tmp/caesar_test_float_" + std::to_string(getpid()) + "_" + 
                              std::to_string(time(nullptr)) + "_" + std::to_string(rand()) + ".c";
        std::string exeFile = tempFile + ".exe";
        
        std::ofstream outFile(tempFile);
        if (!outFile) return false;
        outFile << cCode;
        outFile.close();
        
        // Try to compile with GCC
        std::string compileCmd = "gcc -std=c99 -o " + exeFile + " " + tempFile + " 2>&1";
        int result = system(compileCmd.c_str());
        
        // Cleanup
        remove(tempFile.c_str());
        remove(exeFile.c_str());
        
        return (result == 0);
    } catch (...) {
        return false;
    }
}

void test_simple_float() {
    std::string code = "x = 3.14";
    std::string cCode;
    assert(testCCodeGen(code, cCode) && "Simple float assignment should compile");
    assert(cCode.find("double") != std::string::npos && "Should generate double type");
    assert(cCode.find("3.14") != std::string::npos && "Should contain 3.14");
    std::cout << "✓ Simple float test passed\n";
}

void test_float_arithmetic() {
    std::string code = R"(
x = 5.5
y = 2.5
z = x + y
)";
    std::string cCode;
    assert(testCCodeGen(code, cCode) && "Float arithmetic should compile");
    assert(cCode.find("double") != std::string::npos && "Should generate double type");
    std::cout << "✓ Float arithmetic test passed\n";
}

void test_float_subtraction() {
    std::string code = R"(
a = 10.5
b = 3.2
c = a - b
)";
    std::string cCode;
    assert(testCCodeGen(code, cCode) && "Float subtraction should compile");
    std::cout << "✓ Float subtraction test passed\n";
}

void test_float_multiplication() {
    std::string code = R"(
x = 2.5
y = 4.0
z = x * y
)";
    std::string cCode;
    assert(testCCodeGen(code, cCode) && "Float multiplication should compile");
    std::cout << "✓ Float multiplication test passed\n";
}

void test_float_division() {
    std::string code = R"(
a = 10.0
b = 2.5
c = a / b
)";
    std::string cCode;
    assert(testCCodeGen(code, cCode) && "Float division should compile");
    std::cout << "✓ Float division test passed\n";
}

void test_mixed_int_float() {
    std::string code = R"(
x = 5
y = 2.5
z = x + y
)";
    std::string cCode;
    assert(testCCodeGen(code, cCode) && "Mixed int/float should compile");
    assert(cCode.find("double") != std::string::npos && "Result should be double type");
    std::cout << "✓ Mixed int/float test passed\n";
}

void test_float_comparison() {
    std::string code = R"(
x = 3.14
y = 2.71
if x > y:
    z = 1
)";
    std::string cCode;
    assert(testCCodeGen(code, cCode) && "Float comparison should compile");
    std::cout << "✓ Float comparison test passed\n";
}

void test_float_equality() {
    // Note: Direct float equality (==) is generally problematic due to precision issues.
    // This test verifies that the compiler generates correct C code; users should be
    // aware that float comparisons may not behave as expected for non-exact values.
    std::string code = R"(
x = 5.0
y = 5.0
if x == y:
    result = 1
)";
    std::string cCode;
    assert(testCCodeGen(code, cCode) && "Float equality should compile");
    std::cout << "✓ Float equality test passed\n";
}

void test_negative_float() {
    std::string code = R"(
x = 3.14
y = -x
)";
    std::string cCode;
    assert(testCCodeGen(code, cCode) && "Negative float should compile");
    std::cout << "✓ Negative float test passed\n";
}

void test_zero_float() {
    std::string code = "x = 0.0";
    std::string cCode;
    assert(testCCodeGen(code, cCode) && "Zero float should compile");
    assert(cCode.find("0.0") != std::string::npos && "Should contain 0.0");
    std::cout << "✓ Zero float test passed\n";
}

void test_very_small_float() {
    std::string code = "x = 0.0001";
    std::string cCode;
    assert(testCCodeGen(code, cCode) && "Very small float should compile");
    std::cout << "✓ Very small float test passed\n";
}

void test_very_large_float() {
    std::string code = "x = 999999.999";
    std::string cCode;
    assert(testCCodeGen(code, cCode) && "Very large float should compile");
    std::cout << "✓ Very large float test passed\n";
}

void test_float_in_loop() {
    std::string code = R"(
i = 0.0
while i < 5.0:
    i = i + 1.0
)";
    std::string cCode;
    assert(testCCodeGen(code, cCode) && "Float in loop should compile");
    std::cout << "✓ Float in loop test passed\n";
}

void test_multiple_float_ops() {
    std::string code = R"(
a = 1.5
b = 2.5
c = 3.5
result = a + b * c
)";
    std::string cCode;
    assert(testCCodeGen(code, cCode) && "Multiple float ops should compile");
    std::cout << "✓ Multiple float ops test passed\n";
}

void test_float_reassignment() {
    std::string code = R"(
x = 1.0
x = 2.0
x = 3.0
)";
    std::string cCode;
    assert(testCCodeGen(code, cCode) && "Float reassignment should compile");
    std::cout << "✓ Float reassignment test passed\n";
}

void test_float_conditional_assignment() {
    std::string code = R"(
x = 5.5
if x > 5.0:
    y = 10.0
else:
    y = 0.0
)";
    std::string cCode;
    assert(testCCodeGen(code, cCode) && "Float conditional assignment should compile");
    std::cout << "✓ Float conditional assignment test passed\n";
}

void test_complex_float_expression() {
    std::string code = R"(
x = 2.5
y = 3.5
z = 4.5
result = (x + y) * z - x / y
)";
    std::string cCode;
    assert(testCCodeGen(code, cCode) && "Complex float expression should compile");
    std::cout << "✓ Complex float expression test passed\n";
}

void test_float_less_than_equal() {
    std::string code = R"(
x = 3.14
y = 3.14
if x <= y:
    result = 1
)";
    std::string cCode;
    assert(testCCodeGen(code, cCode) && "Float <= should compile");
    std::cout << "✓ Float <= test passed\n";
}

void test_float_greater_than_equal() {
    std::string code = R"(
x = 5.0
y = 4.9
if x >= y:
    result = 1
)";
    std::string cCode;
    assert(testCCodeGen(code, cCode) && "Float >= should compile");
    std::cout << "✓ Float >= test passed\n";
}

void test_float_not_equal() {
    std::string code = R"(
x = 3.14
y = 2.71
if x != y:
    result = 1
)";
    std::string cCode;
    assert(testCCodeGen(code, cCode) && "Float != should compile");
    std::cout << "✓ Float != test passed\n";
}

int main() {
    std::cout << "Running comprehensive float C code generation tests...\n\n";
    
    // Run all float tests
    const int TOTAL_TESTS = 20;  // Update this if tests are added/removed
    int tests_run = 0;
    
    test_simple_float(); tests_run++;
    test_float_arithmetic(); tests_run++;
    test_float_subtraction(); tests_run++;
    test_float_multiplication(); tests_run++;
    test_float_division(); tests_run++;
    test_mixed_int_float(); tests_run++;
    test_float_comparison(); tests_run++;
    test_float_equality(); tests_run++;
    test_negative_float(); tests_run++;
    test_zero_float(); tests_run++;
    test_very_small_float(); tests_run++;
    test_very_large_float(); tests_run++;
    test_float_in_loop(); tests_run++;
    test_multiple_float_ops(); tests_run++;
    test_float_reassignment(); tests_run++;
    test_float_conditional_assignment(); tests_run++;
    test_complex_float_expression(); tests_run++;
    test_float_less_than_equal(); tests_run++;
    test_float_greater_than_equal(); tests_run++;
    test_float_not_equal(); tests_run++;
    
    std::cout << "\n✅ All " << tests_run << " float C code generation tests passed!\n";
    if (tests_run != TOTAL_TESTS) {
        std::cerr << "⚠️  Warning: Expected " << TOTAL_TESTS << " tests but ran " << tests_run << "\n";
    }
    return 0;
}
