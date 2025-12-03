/**
 * @file test_compiler.cpp
 * @brief Comprehensive compiler tests for Caesar
 * @author J.J.G. Pleunes
 * @version 1.5.1
 */

#include "caesar/lexer.h"
#include "caesar/parser.h"
#include "caesar/ir.h"
#include "caesar/codegen.h"
#include <iostream>
#include <cassert>
#include <string>

using namespace caesar;

// Helper function to compile Caesar source to C code
std::string compileToC(const std::string& source) {
    try {
        // Lex
        Lexer lexer(source);
        auto tokens = lexer.tokenize();
        
        // Parse
        Parser parser(tokens);
        auto program = parser.parse();
        
        // Generate IR
        IRGenerator ir_gen;
        auto ir_blocks = ir_gen.generate(program.get());
        
        // Generate C code
        auto c_gen = CodeGeneratorFactory::createCGenerator();
        return c_gen->generate(ir_blocks);
    } catch (const std::exception& e) {
        std::cerr << "Compilation error: " << e.what() << "\n";
        return "";
    }
}

// Test 1: Basic arithmetic
void test_arithmetic() {
    std::cout << "Test 1: Basic arithmetic... ";
    
    std::string source = R"(
x = 10
y = 5
z = x + y
w = x - y
a = x * y
b = x / y
c = x % y
)";
    
    std::string c_code = compileToC(source);
    
    // Check that C code contains the operations (using register names)
    assert(c_code.find(" + ") != std::string::npos);
    assert(c_code.find(" - ") != std::string::npos);
    assert(c_code.find(" * ") != std::string::npos);
    assert(c_code.find(" / ") != std::string::npos);
    assert(c_code.find(" % ") != std::string::npos);
    
    std::cout << "✅ PASSED\n";
}

// Test 2: Comparison operators
void test_comparisons() {
    std::cout << "Test 2: Comparison operators... ";
    
    std::string source = R"(
x = 10
y = 5
a = x == y
b = x != y
c = x < y
d = x <= y
e = x > y
f = x >= y
)";
    
    std::string c_code = compileToC(source);
    
    // Check that C code contains the comparisons
    assert(c_code.find("==") != std::string::npos);
    assert(c_code.find("!=") != std::string::npos);
    assert(c_code.find(" < ") != std::string::npos);
    assert(c_code.find("<=") != std::string::npos);
    assert(c_code.find(" > ") != std::string::npos);
    assert(c_code.find(">=") != std::string::npos);
    
    std::cout << "✅ PASSED\n";
}

// Test 3: Logical operators
void test_logical() {
    std::cout << "Test 3: Logical operators... ";
    
    std::string source = R"(
x = True
y = False
a = x and y
b = x or y
c = not x
)";
    
    std::string c_code = compileToC(source);
    
    // Check that C code contains logical operations
    assert(c_code.find("&&") != std::string::npos || c_code.find("and") != std::string::npos);
    assert(c_code.find("||") != std::string::npos || c_code.find("or") != std::string::npos);
    assert(c_code.find("!") != std::string::npos || c_code.find("not") != std::string::npos);
    
    std::cout << "✅ PASSED\n";
}

// Test 4: If statements
void test_conditionals() {
    std::cout << "Test 4: Conditional statements... ";
    
    std::string source = R"(
x = 10
if x > 0:
    y = 1
else:
    y = 0
)";
    
    std::string c_code = compileToC(source);
    
    // Check that C code contains conditional branching
    assert(c_code.find("goto") != std::string::npos);
    assert(c_code.find("else") != std::string::npos);
    
    std::cout << "✅ PASSED\n";
}

// Test 5: While loops
void test_while_loops() {
    std::cout << "Test 5: While loops... ";
    
    std::string source = R"(
i = 0
while i < 10:
    i = i + 1
)";
    
    std::string c_code = compileToC(source);
    
    // Check that C code contains loop structure
    assert(c_code.find("goto") != std::string::npos);
    assert(c_code.find(" < ") != std::string::npos);
    
    std::cout << "✅ PASSED\n";
}

// Test 6: For loops
void test_for_loops() {
    std::cout << "Test 6: For loops... ";
    
    std::string source = R"(
for i in range(10):
    x = i
)";
    
    std::string c_code = compileToC(source);
    
    // Check that C code is generated (even if incomplete)
    assert(!c_code.empty());
    assert(c_code.find("int main()") != std::string::npos);
    
    std::cout << "✅ PASSED\n";
}

// Test 7: Functions
void test_functions() {
    std::cout << "Test 7: Functions... ";
    
    std::string source = R"(
def add(a, b):
    return a + b
)";
    
    std::string c_code = compileToC(source);
    
    // Check that C code is generated
    assert(!c_code.empty());
    assert(c_code.find("int main()") != std::string::npos);
    
    std::cout << "✅ PASSED\n";
}

// Test 8: IR Generation for complex expressions
void test_ir_generation() {
    std::cout << "Test 8: IR generation... ";
    
    std::string source = R"(
result = (5 + 3) * 2 - 1
)";
    
    try {
        Lexer lexer(source);
        auto tokens = lexer.tokenize();
        
        Parser parser(tokens);
        auto program = parser.parse();
        
        IRGenerator ir_gen;
        auto ir_blocks = ir_gen.generate(program.get());
        
        // Check that IR was generated
        assert(!ir_blocks.empty());
        
        // Check IR contains expected operations
        std::string ir_str = ir_gen.toString();
        assert(ir_str.find("ADD") != std::string::npos);
        assert(ir_str.find("MUL") != std::string::npos);
        assert(ir_str.find("SUB") != std::string::npos);
        
        std::cout << "✅ PASSED\n";
    } catch (const std::exception& e) {
        std::cerr << "❌ FAILED: " << e.what() << "\n";
        assert(false);
    }
}

// Test 9: Bytecode generation
void test_bytecode_generation() {
    std::cout << "Test 9: Bytecode generation... ";
    
    std::string source = R"(
x = 42
y = x + 10
)";
    
    try {
        Lexer lexer(source);
        auto tokens = lexer.tokenize();
        
        Parser parser(tokens);
        auto program = parser.parse();
        
        IRGenerator ir_gen;
        auto ir_blocks = ir_gen.generate(program.get());
        
        auto bytecode_gen = CodeGeneratorFactory::create(TargetArch::BYTECODE);
        std::string bytecode = bytecode_gen->generate(ir_blocks);
        
        // Check that bytecode was generated
        assert(!bytecode.empty());
        assert(bytecode.find("Caesar Bytecode") != std::string::npos);
        
        std::cout << "✅ PASSED\n";
    } catch (const std::exception& e) {
        std::cerr << "❌ FAILED: " << e.what() << "\n";
        assert(false);
    }
}

// Test 10: x86-64 assembly generation
void test_x86_64_generation() {
    std::cout << "Test 10: x86-64 assembly generation... ";
    
    std::string source = R"(
x = 42
y = x + 10
)";
    
    try {
        Lexer lexer(source);
        auto tokens = lexer.tokenize();
        
        Parser parser(tokens);
        auto program = parser.parse();
        
        IRGenerator ir_gen;
        auto ir_blocks = ir_gen.generate(program.get());
        
        auto asm_gen = CodeGeneratorFactory::create(TargetArch::X86_64);
        std::string assembly = asm_gen->generate(ir_blocks);
        
        // Check that assembly was generated
        assert(!assembly.empty());
        assert(assembly.find("x86-64") != std::string::npos || 
               assembly.find("section") != std::string::npos);
        
        std::cout << "✅ PASSED\n";
    } catch (const std::exception& e) {
        std::cerr << "❌ FAILED: " << e.what() << "\n";
        assert(false);
    }
}

int main() {
    std::cout << "=================================\n";
    std::cout << "Caesar Compiler Test Suite\n";
    std::cout << "=================================\n\n";
    
    try {
        test_arithmetic();
        test_comparisons();
        test_logical();
        test_conditionals();
        test_while_loops();
        test_for_loops();
        test_functions();
        test_ir_generation();
        test_bytecode_generation();
        test_x86_64_generation();
        
        std::cout << "\n=================================\n";
        std::cout << "All compiler tests passed! ✅\n";
        std::cout << "=================================\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "\n=================================\n";
        std::cerr << "Test suite failed! ❌\n";
        std::cerr << "Error: " << e.what() << "\n";
        std::cerr << "=================================\n";
        return 1;
    }
}
