/**
 * @file test_c_codegen.cpp
 * @brief Comprehensive C code generation tests
 * @author J.J.G. Pleunes
 * @version 1.5.1
 * 
 * This test suite validates C code generation with extensive edge case coverage
 * following production-quality standards.
 */

#include "caesar/lexer.h"
#include "caesar/parser.h"
#include "caesar/ir.h"
#include "caesar/codegen.h"
#include <iostream>
#include <cassert>
#include <fstream>
#include <cstdlib>
#include <sstream>

using namespace caesar;

// Helper to compile Caesar to C and then compile with GCC
std::pair<bool, std::string> compileAndRun(const std::string& caesar_code) {
    try {
        // Lex, parse, generate IR
        Lexer lexer(caesar_code);
        auto tokens = lexer.tokenize();
        Parser parser(tokens);
        auto program = parser.parse();
        IRGenerator ir_gen;
        auto ir_blocks = ir_gen.generate(program.get());
        
        // Generate C code
        auto c_gen = CodeGeneratorFactory::createCGenerator();
        std::string c_code = c_gen->generate(ir_blocks);
        
        // Write to temp file
        std::string temp_c = "/tmp/test_caesar_" + std::to_string(rand()) + ".c";
        std::string temp_exe = temp_c + ".out";
        std::ofstream out(temp_c);
        out << c_code;
        out.close();
        
        // Compile with GCC
        std::string compile_cmd = "gcc -std=c99 -o " + temp_exe + " " + temp_c + " 2>&1";
        FILE* pipe = popen(compile_cmd.c_str(), "r");
        if (!pipe) return {false, "Failed to compile"};
        
        char buffer[256];
        std::string compile_output;
        while (fgets(buffer, sizeof(buffer), pipe)) {
            compile_output += buffer;
        }
        int compile_status = pclose(pipe);
        
        if (compile_status != 0) {
            return {false, "Compilation failed: " + compile_output};
        }
        
        // Run the executable
        std::string run_cmd = temp_exe + " 2>&1";
        pipe = popen(run_cmd.c_str(), "r");
        if (!pipe) return {false, "Failed to run"};
        
        std::string output;
        while (fgets(buffer, sizeof(buffer), pipe)) {
            output += buffer;
        }
        pclose(pipe);
        
        // Cleanup
        remove(temp_c.c_str());
        remove(temp_exe.c_str());
        
        return {true, output};
    } catch (const std::exception& e) {
        return {false, std::string("Exception: ") + e.what()};
    }
}

// Test 1: Simple arithmetic
void test_simple_arithmetic() {
    std::cout << "Test 1: Simple arithmetic... ";
    
    std::string code = R"(
x = 5
y = 10
z = x + y
)";
    
    auto result = compileAndRun(code);
    // For now, just check that C code compiles
    // TODO: Add proper output checking once functions work
    
    std::cout << (result.first ? "✅ PASS" : "❌ FAIL") << "\n";
    if (!result.first) {
        std::cout << "  Error: " << result.second << "\n";
    }
}

// Test 2: All arithmetic operations
void test_all_arithmetic() {
    std::cout << "Test 2: All arithmetic operations... ";
    
    std::string code = R"(
a = 10
b = 3
add = a + b
sub = a - b
mul = a * b
div = a / b
mod = a % b
neg = -a
)";
    
    auto result = compileAndRun(code);
    std::cout << (result.first ? "✅ PASS" : "❌ FAIL") << "\n";
    if (!result.first) {
        std::cout << "  Error: " << result.second << "\n";
    }
}

// Test 3: Comparison operators
void test_comparisons() {
    std::cout << "Test 3: Comparison operators... ";
    
    std::string code = R"(
x = 5
y = 10
eq = x == y
ne = x != y
lt = x < y
le = x <= y
gt = x > y
ge = x >= y
)";
    
    auto result = compileAndRun(code);
    std::cout << (result.first ? "✅ PASS" : "❌ FAIL") << "\n";
    if (!result.first) {
        std::cout << "  Error: " << result.second << "\n";
    }
}

// Test 4: Logical operators
void test_logical_ops() {
    std::cout << "Test 4: Logical operators... ";
    
    std::string code = R"(
x = True
y = False
a = x and y
b = x or y
c = not x
)";
    
    auto result = compileAndRun(code);
    std::cout << (result.first ? "✅ PASS" : "❌ FAIL") << "\n";
    if (!result.first) {
        std::cout << "  Error: " << result.second << "\n";
    }
}

// Test 5: If statement (basic)
void test_if_basic() {
    std::cout << "Test 5: Basic if statement... ";
    
    std::string code = R"(
x = 10
if x > 5:
    y = 1
else:
    y = 0
)";
    
    auto result = compileAndRun(code);
    std::cout << (result.first ? "✅ PASS" : "❌ FAIL") << "\n";
    if (!result.first) {
        std::cout << "  Error: " << result.second << "\n";
    }
}

// Test 6: If-elif-else chain
void test_if_elif_else() {
    std::cout << "Test 6: If-elif-else chain... ";
    
    std::string code = R"(
x = 0
if x > 0:
    result = 1
elif x < 0:
    result = -1
else:
    result = 0
)";
    
    auto result = compileAndRun(code);
    std::cout << (result.first ? "✅ PASS" : "❌ FAIL") << "\n";
    if (!result.first) {
        std::cout << "  Error: " << result.second << "\n";
    }
}

// Test 7: Nested if statements
void test_nested_if() {
    std::cout << "Test 7: Nested if statements... ";
    
    std::string code = R"(
x = 10
y = 20
if x > 0:
    if y > 0:
        result = 1
    else:
        result = 2
else:
    result = 3
)";
    
    auto result = compileAndRun(code);
    std::cout << (result.first ? "✅ PASS" : "❌ FAIL") << "\n";
    if (!result.first) {
        std::cout << "  Error: " << result.second << "\n";
    }
}

// Test 8: While loop
void test_while_loop() {
    std::cout << "Test 8: While loop... ";
    
    std::string code = R"(
i = 0
sum = 0
while i < 5:
    sum = sum + i
    i = i + 1
)";
    
    auto result = compileAndRun(code);
    std::cout << (result.first ? "✅ PASS" : "❌ FAIL") << "\n";
    if (!result.first) {
        std::cout << "  Error: " << result.second << "\n";
    }
}

// Test 9: Complex expressions
void test_complex_expressions() {
    std::cout << "Test 9: Complex expressions... ";
    
    std::string code = R"(
a = 5
b = 10
c = 3
result = (a + b) * c - a / b
)";
    
    auto result = compileAndRun(code);
    std::cout << (result.first ? "✅ PASS" : "❌ FAIL") << "\n";
    if (!result.first) {
        std::cout << "  Error: " << result.second << "\n";
    }
}

// Test 10: Variable shadowing in nested scopes
void test_variable_scope() {
    std::cout << "Test 10: Variable scope... ";
    
    std::string code = R"(
x = 10
if True:
    x = 20
    y = x
)";
    
    auto result = compileAndRun(code);
    std::cout << (result.first ? "✅ PASS" : "❌ FAIL") << "\n";
    if (!result.first) {
        std::cout << "  Error: " << result.second << "\n";
    }
}

// Test 11: Edge case - division by non-zero
void test_division_valid() {
    std::cout << "Test 11: Valid division... ";
    
    std::string code = R"(
x = 10
y = 2
z = x / y
)";
    
    auto result = compileAndRun(code);
    std::cout << (result.first ? "✅ PASS" : "❌ FAIL") << "\n";
    if (!result.first) {
        std::cout << "  Error: " << result.second << "\n";
    }
}

// Test 12: Edge case - modulo operation
void test_modulo() {
    std::cout << "Test 12: Modulo operation... ";
    
    std::string code = R"(
x = 17
y = 5
z = x % y
)";
    
    auto result = compileAndRun(code);
    std::cout << (result.first ? "✅ PASS" : "❌ FAIL") << "\n";
    if (!result.first) {
        std::cout << "  Error: " << result.second << "\n";
    }
}

// Test 13: Boolean literals
void test_boolean_literals() {
    std::cout << "Test 13: Boolean literals... ";
    
    std::string code = R"(
t = True
f = False
)";
    
    auto result = compileAndRun(code);
    std::cout << (result.first ? "✅ PASS" : "❌ FAIL") << "\n";
    if (!result.first) {
        std::cout << "  Error: " << result.second << "\n";
    }
}

// Test 14: Multiple assignments
void test_multiple_assignments() {
    std::cout << "Test 14: Multiple assignments... ";
    
    std::string code = R"(
a = 1
b = 2
c = 3
a = b
b = c
c = a
)";
    
    auto result = compileAndRun(code);
    std::cout << (result.first ? "✅ PASS" : "❌ FAIL") << "\n";
    if (!result.first) {
        std::cout << "  Error: " << result.second << "\n";
    }
}

// Test 15: Chained comparisons as separate statements
void test_chained_comparisons() {
    std::cout << "Test 15: Multiple comparisons... ";
    
    std::string code = R"(
x = 5
a = x > 0
b = x < 10
c = a and b
)";
    
    auto result = compileAndRun(code);
    std::cout << (result.first ? "✅ PASS" : "❌ FAIL") << "\n";
    if (!result.first) {
        std::cout << "  Error: " << result.second << "\n";
    }
}

void test_simple_function() {
    std::cout << "Test 16: Simple Function (no params)... ";
    
    std::string code = R"(
def get_five():
    return 5

result = get_five()
print(result)
)";
    
    auto result = compileAndRun(code);
    std::cout << (result.first ? "✅ PASS" : "❌ FAIL") << "\n";
    if (!result.first) {
        std::cout << "  Error: " << result.second << "\n";
    }
}

int main() {
    std::cout << "========================================\n";
    std::cout << "C Code Generation Test Suite\n";
    std::cout << "========================================\n\n";
    
    // Run all tests
    test_simple_arithmetic();
    test_all_arithmetic();
    test_comparisons();
    test_logical_ops();
    test_if_basic();
    test_if_elif_else();
    test_nested_if();
    test_while_loop();
    test_complex_expressions();
    test_variable_scope();
    test_division_valid();
    test_modulo();
    test_boolean_literals();
    test_multiple_assignments();
    test_chained_comparisons();
    test_simple_function();
    
    std::cout << "\n========================================\n";
    std::cout << "C Code Generation Tests Complete\n";
    std::cout << "Note: These tests verify C code compiles.\n";
    std::cout << "Function support needed for full validation.\n";
    std::cout << "========================================\n";
    
    return 0;
}
