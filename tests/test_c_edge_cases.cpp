/**
 * @file test_c_edge_cases.cpp
 * @brief Edge case tests for C code generation
 * @author J.J.G. Pleunes
 * @version 1.5.1
 * 
 * Tests edge cases and boundary conditions for production-quality compiler
 */

#include "caesar/lexer.h"
#include "caesar/parser.h"
#include "caesar/ir.h"
#include "caesar/codegen.h"
#include <iostream>
#include <cassert>
#include <fstream>
#include <cstdlib>

using namespace caesar;

// Helper to compile Caesar to C
std::pair<bool, std::string> compileToC(const std::string& caesar_code) {
    try {
        Lexer lexer(caesar_code);
        auto tokens = lexer.tokenize();
        Parser parser(tokens);
        auto program = parser.parse();
        IRGenerator ir_gen;
        auto ir_blocks = ir_gen.generate(program.get());
        auto c_gen = CodeGeneratorFactory::createCGenerator();
        std::string c_code = c_gen->generate(ir_blocks);
        
        // Write and compile
        std::string temp_c = "/tmp/test_edge_" + std::to_string(rand()) + ".c";
        std::string temp_exe = temp_c + ".out";
        std::ofstream out(temp_c);
        out << c_code;
        out.close();
        
        std::string compile_cmd = "gcc -std=c99 -o " + temp_exe + " " + temp_c + " 2>&1";
        FILE* pipe = popen(compile_cmd.c_str(), "r");
        if (!pipe) return {false, "Failed to compile"};
        
        char buffer[256];
        std::string compile_output;
        while (fgets(buffer, sizeof(buffer), pipe)) {
            compile_output += buffer;
        }
        int status = pclose(pipe);
        
        remove(temp_c.c_str());
        remove(temp_exe.c_str());
        
        if (status != 0) {
            return {false, "Compilation failed: " + compile_output};
        }
        
        return {true, "Success"};
    } catch (const std::exception& e) {
        return {false, std::string("Exception: ") + e.what()};
    }
}

// Test 1: Negative numbers
void test_negative_numbers() {
    std::cout << "Test 1: Negative numbers... ";
    
    std::string code = R"(
x = -5
y = -10
z = x + y
w = -x
)";
    
    auto result = compileToC(code);
    std::cout << (result.first ? "✅ PASS" : "❌ FAIL") << "\n";
    if (!result.first) std::cout << "  Error: " << result.second << "\n";
}

// Test 2: Zero in operations
void test_zero_operations() {
    std::cout << "Test 2: Zero in operations... ";
    
    std::string code = R"(
x = 0
y = x + 0
z = x * 100
w = 100 * x
)";
    
    auto result = compileToC(code);
    std::cout << (result.first ? "✅ PASS" : "❌ FAIL") << "\n";
    if (!result.first) std::cout << "  Error: " << result.second << "\n";
}

// Test 3: Large numbers
void test_large_numbers() {
    std::cout << "Test 3: Large numbers... ";
    
    std::string code = R"(
x = 1000000
y = 9999999
z = x + y
)";
    
    auto result = compileToC(code);
    std::cout << (result.first ? "✅ PASS" : "❌ FAIL") << "\n";
    if (!result.first) std::cout << "  Error: " << result.second << "\n";
}

// Test 4: Mixed type operations (int)
void test_mixed_int_operations() {
    std::cout << "Test 4: Mixed integer operations... ";
    
    std::string code = R"(
a = 1
b = 2
c = 3
result = a + b * c - a / b
)";
    
    auto result = compileToC(code);
    std::cout << (result.first ? "✅ PASS" : "❌ FAIL") << "\n";
    if (!result.first) std::cout << "  Error: " << result.second << "\n";
}

// Test 5: Deep nesting of if statements
void test_deep_if_nesting() {
    std::cout << "Test 5: Deep if nesting... ";
    
    std::string code = R"(
x = 1
if x > 0:
    if x > 1:
        if x > 2:
            y = 1
        else:
            y = 2
    else:
        y = 3
else:
    y = 4
)";
    
    auto result = compileToC(code);
    std::cout << (result.first ? "✅ PASS" : "❌ FAIL") << "\n";
    if (!result.first) std::cout << "  Error: " << result.second << "\n";
}

// Test 6: Multiple elif branches
void test_multiple_elif() {
    std::cout << "Test 6: Multiple elif branches... ";
    
    std::string code = R"(
x = 5
if x < 0:
    y = 1
elif x < 5:
    y = 2
elif x < 10:
    y = 3
elif x < 15:
    y = 4
else:
    y = 5
)";
    
    auto result = compileToC(code);
    std::cout << (result.first ? "✅ PASS" : "❌ FAIL") << "\n";
    if (!result.first) std::cout << "  Error: " << result.second << "\n";
}

// Test 7: Boolean expressions in conditions
void test_boolean_conditions() {
    std::cout << "Test 7: Boolean expressions... ";
    
    std::string code = R"(
a = True
b = False
if a and not b:
    x = 1
elif a or b:
    x = 2
else:
    x = 3
)";
    
    auto result = compileToC(code);
    std::cout << (result.first ? "✅ PASS" : "❌ FAIL") << "\n";
    if (!result.first) std::cout << "  Error: " << result.second << "\n";
}

// Test 8: Comparison chains
void test_comparison_chains() {
    std::cout << "Test 8: Comparison chains... ";
    
    std::string code = R"(
x = 5
a = x > 0
b = x < 10
c = a and b
)";
    
    auto result = compileToC(code);
    std::cout << (result.first ? "✅ PASS" : "❌ FAIL") << "\n";
    if (!result.first) std::cout << "  Error: " << result.second << "\n";
}

// Test 9: Precedence of operations
void test_operator_precedence() {
    std::cout << "Test 9: Operator precedence... ";
    
    std::string code = R"(
a = 2 + 3 * 4
b = (2 + 3) * 4
c = 10 - 2 - 3
d = 10 / 2 / 2
)";
    
    auto result = compileToC(code);
    std::cout << (result.first ? "✅ PASS" : "❌ FAIL") << "\n";
    if (!result.first) std::cout << "  Error: " << result.second << "\n";
}

// Test 10: Empty if body (should have at least some code)
void test_minimal_if() {
    std::cout << "Test 10: Minimal if statement... ";
    
    std::string code = R"(
x = 5
if x > 0:
    y = 1
)";
    
    auto result = compileToC(code);
    std::cout << (result.first ? "✅ PASS" : "❌ FAIL") << "\n";
    if (!result.first) std::cout << "  Error: " << result.second << "\n";
}

// Test 11: Multiple variables in one scope
void test_many_variables() {
    std::cout << "Test 11: Many variables... ";
    
    std::string code = R"(
a = 1
b = 2
c = 3
d = 4
e = 5
f = 6
g = 7
h = 8
i = 9
j = 10
)";
    
    auto result = compileToC(code);
    std::cout << (result.first ? "✅ PASS" : "❌ FAIL") << "\n";
    if (!result.first) std::cout << "  Error: " << result.second << "\n";
}

// Test 12: Reusing variable names
void test_variable_reuse() {
    std::cout << "Test 12: Variable reuse... ";
    
    std::string code = R"(
x = 1
x = 2
x = 3
x = x + 1
)";
    
    auto result = compileToC(code);
    std::cout << (result.first ? "✅ PASS" : "❌ FAIL") << "\n";
    if (!result.first) std::cout << "  Error: " << result.second << "\n";
}

// Test 13: All comparison operators in sequence
void test_all_comparisons() {
    std::cout << "Test 13: All comparisons... ";
    
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
    
    auto result = compileToC(code);
    std::cout << (result.first ? "✅ PASS" : "❌ FAIL") << "\n";
    if (!result.first) std::cout << "  Error: " << result.second << "\n";
}

// Test 14: All logical operators
void test_all_logical() {
    std::cout << "Test 14: All logical operators... ";
    
    std::string code = R"(
a = True
b = False
c = a and b
d = a or b
e = not a
f = not b
)";
    
    auto result = compileToC(code);
    std::cout << (result.first ? "✅ PASS" : "❌ FAIL") << "\n";
    if (!result.first) std::cout << "  Error: " << result.second << "\n";
}

// Test 15: Complex boolean expression
void test_complex_boolean() {
    std::cout << "Test 15: Complex boolean expression... ";
    
    std::string code = R"(
a = True
b = False
c = True
result = (a or b) and (not c or a)
)";
    
    auto result = compileToC(code);
    std::cout << (result.first ? "✅ PASS" : "❌ FAIL") << "\n";
    if (!result.first) std::cout << "  Error: " << result.second << "\n";
}

// Test 16: While loop with counter
void test_while_counter() {
    std::cout << "Test 16: While loop counter... ";
    
    std::string code = R"(
i = 0
while i < 10:
    i = i + 1
)";
    
    auto result = compileToC(code);
    std::cout << (result.first ? "✅ PASS" : "❌ FAIL") << "\n";
    if (!result.first) std::cout << "  Error: " << result.second << "\n";
}

// Test 17: Nested while loops
void test_nested_while() {
    std::cout << "Test 17: Nested while loops... ";
    
    std::string code = R"(
i = 0
while i < 3:
    j = 0
    while j < 3:
        j = j + 1
    i = i + 1
)";
    
    auto result = compileToC(code);
    std::cout << (result.first ? "✅ PASS" : "❌ FAIL") << "\n";
    if (!result.first) std::cout << "  Error: " << result.second << "\n";
}

// Test 18: Single statement  
void test_single_statement() {
    std::cout << "Test 18: Single statement... ";
    
    std::string code = "x = 42";
    
    auto result = compileToC(code);
    std::cout << (result.first ? "✅ PASS" : "❌ FAIL") << "\n";
    if (!result.first) std::cout << "  Error: " << result.second << "\n";
}

// Test 19: Variable used before assignment in different branch
void test_conditional_assignment() {
    std::cout << "Test 19: Conditional assignment... ";
    
    std::string code = R"(
x = 1
if x > 0:
    y = 10
else:
    y = 20
z = y
)";
    
    auto result = compileToC(code);
    std::cout << (result.first ? "✅ PASS" : "❌ FAIL") << "\n";
    if (!result.first) std::cout << "  Error: " << result.second << "\n";
}

// Test 20: Edge case - only declarations
void test_only_declarations() {
    std::cout << "Test 20: Only variable declarations... ";
    
    std::string code = R"(
a = 1
b = 2
c = 3
)";
    
    auto result = compileToC(code);
    std::cout << (result.first ? "✅ PASS" : "❌ FAIL") << "\n";
    if (!result.first) std::cout << "  Error: " << result.second << "\n";
}

int main() {
    std::cout << "=========================================\n";
    std::cout << "C Code Generation Edge Case Test Suite\n";
    std::cout << "=========================================\n\n";
    
    test_negative_numbers();
    test_zero_operations();
    test_large_numbers();
    test_mixed_int_operations();
    test_deep_if_nesting();
    test_multiple_elif();
    test_boolean_conditions();
    test_comparison_chains();
    test_operator_precedence();
    test_minimal_if();
    test_many_variables();
    test_variable_reuse();
    test_all_comparisons();
    test_all_logical();
    test_complex_boolean();
    test_while_counter();
    test_nested_while();
    test_single_statement();
    test_conditional_assignment();
    test_only_declarations();
    
    std::cout << "\n=========================================\n";
    std::cout << "Edge Case Tests Complete - 20 tests\n";
    std::cout << "All tests verify GCC compilation success\n";
    std::cout << "=========================================\n";
    
    return 0;
}
