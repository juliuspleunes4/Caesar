/**
 * @file test_c_strings.cpp
 * @brief Comprehensive string literal tests for C code generation
 * @author J.J.G. Pleunes
 * @version 1.5.1
 * 
 * Tests string literal support with extensive edge cases
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

// Helper to compile Caesar to C and verify GCC compilation
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
        std::string temp_c = "/tmp/test_str_" + std::to_string(rand()) + ".c";
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

// Test 1: Simple string assignment
void test_simple_string() {
    std::cout << "Test 1: Simple string assignment... ";
    
    std::string code = R"(
x = "Hello"
)";
    
    auto result = compileToC(code);
    std::cout << (result.first ? "✅ PASS" : "❌ FAIL") << "\n";
    if (!result.first) std::cout << "  Error: " << result.second << "\n";
}

// Test 2: Multiple string assignments
void test_multiple_strings() {
    std::cout << "Test 2: Multiple string assignments... ";
    
    std::string code = R"(
x = "Hello"
y = "World"
z = "Caesar"
)";
    
    auto result = compileToC(code);
    std::cout << (result.first ? "✅ PASS" : "❌ FAIL") << "\n";
    if (!result.first) std::cout << "  Error: " << result.second << "\n";
}

// Test 3: Empty string
void test_empty_string() {
    std::cout << "Test 3: Empty string... ";
    
    std::string code = R"(
x = ""
)";
    
    auto result = compileToC(code);
    std::cout << (result.first ? "✅ PASS" : "❌ FAIL") << "\n";
    if (!result.first) std::cout << "  Error: " << result.second << "\n";
}

// Test 4: String with spaces
void test_string_with_spaces() {
    std::cout << "Test 4: String with spaces... ";
    
    std::string code = R"(
x = "Hello World"
y = "  spaces  "
)";
    
    auto result = compileToC(code);
    std::cout << (result.first ? "✅ PASS" : "❌ FAIL") << "\n";
    if (!result.first) std::cout << "  Error: " << result.second << "\n";
}

// Test 5: String with special characters
void test_string_special_chars() {
    std::cout << "Test 5: String with special characters... ";
    
    std::string code = R"(
x = "Hello\nWorld"
y = "Tab\there"
)";
    
    auto result = compileToC(code);
    std::cout << (result.first ? "✅ PASS" : "❌ FAIL") << "\n";
    if (!result.first) std::cout << "  Error: " << result.second << "\n";
}

// Test 6: Long string
void test_long_string() {
    std::cout << "Test 6: Long string... ";
    
    std::string code = R"(
x = "This is a very long string that contains many characters to test the handling of longer strings in the C code generator"
)";
    
    auto result = compileToC(code);
    std::cout << (result.first ? "✅ PASS" : "❌ FAIL") << "\n";
    if (!result.first) std::cout << "  Error: " << result.second << "\n";
}

// Test 7: String with numbers
void test_string_with_numbers() {
    std::cout << "Test 7: String with numbers... ";
    
    std::string code = R"(
x = "123"
y = "Version 1.5.0"
)";
    
    auto result = compileToC(code);
    std::cout << (result.first ? "✅ PASS" : "❌ FAIL") << "\n";
    if (!result.first) std::cout << "  Error: " << result.second << "\n";
}

// Test 8: Mixed string and integer assignments
void test_mixed_string_int() {
    std::cout << "Test 8: Mixed string and integer... ";
    
    std::string code = R"(
x = "Hello"
y = 42
z = "World"
)";
    
    auto result = compileToC(code);
    std::cout << (result.first ? "✅ PASS" : "❌ FAIL") << "\n";
    if (!result.first) std::cout << "  Error: " << result.second << "\n";
}

// Test 9: String reassignment
void test_string_reassignment() {
    std::cout << "Test 9: String reassignment... ";
    
    std::string code = R"(
x = "Hello"
x = "World"
)";
    
    auto result = compileToC(code);
    std::cout << (result.first ? "✅ PASS" : "❌ FAIL") << "\n";
    if (!result.first) std::cout << "  Error: " << result.second << "\n";
}

// Test 10: String in conditional
void test_string_in_conditional() {
    std::cout << "Test 10: String in conditional... ";
    
    std::string code = R"(
x = 1
if x > 0:
    msg = "positive"
else:
    msg = "negative"
)";
    
    auto result = compileToC(code);
    std::cout << (result.first ? "✅ PASS" : "❌ FAIL") << "\n";
    if (!result.first) std::cout << "  Error: " << result.second << "\n";
}

// Test 11: Multiple strings in different scopes
void test_strings_different_scopes() {
    std::cout << "Test 11: Strings in different scopes... ";
    
    std::string code = R"(
x = "outer"
if True:
    y = "inner"
)";
    
    auto result = compileToC(code);
    std::cout << (result.first ? "✅ PASS" : "❌ FAIL") << "\n";
    if (!result.first) std::cout << "  Error: " << result.second << "\n";
}

// Test 12: String with punctuation
void test_string_punctuation() {
    std::cout << "Test 12: String with punctuation... ";
    
    std::string code = R"(
x = "Hello, World!"
y = "What's up?"
)";
    
    auto result = compileToC(code);
    std::cout << (result.first ? "✅ PASS" : "❌ FAIL") << "\n";
    if (!result.first) std::cout << "  Error: " << result.second << "\n";
}

// Test 13: Single character string
void test_single_char_string() {
    std::cout << "Test 13: Single character string... ";
    
    std::string code = R"(
x = "a"
y = "Z"
)";
    
    auto result = compileToC(code);
    std::cout << (result.first ? "✅ PASS" : "❌ FAIL") << "\n";
    if (!result.first) std::cout << "  Error: " << result.second << "\n";
}

// Test 14: String with escaped quotes (if supported)
void test_string_escaped_quotes() {
    std::cout << "Test 14: String with escaped quotes... ";
    
    std::string code = R"(
x = "He said \"Hello\""
)";
    
    auto result = compileToC(code);
    std::cout << (result.first ? "✅ PASS" : "❌ FAIL") << "\n";
    if (!result.first) std::cout << "  Error: " << result.second << "\n";
}

// Test 15: Multiple string declarations
void test_many_strings() {
    std::cout << "Test 15: Many string declarations... ";
    
    std::string code = R"(
s1 = "one"
s2 = "two"
s3 = "three"
s4 = "four"
s5 = "five"
)";
    
    auto result = compileToC(code);
    std::cout << (result.first ? "✅ PASS" : "❌ FAIL") << "\n";
    if (!result.first) std::cout << "  Error: " << result.second << "\n";
}

int main() {
    std::cout << "=========================================\n";
    std::cout << "C Code Generation String Test Suite\n";
    std::cout << "=========================================\n\n";
    
    test_simple_string();
    test_multiple_strings();
    test_empty_string();
    test_string_with_spaces();
    test_string_special_chars();
    test_long_string();
    test_string_with_numbers();
    test_mixed_string_int();
    test_string_reassignment();
    test_string_in_conditional();
    test_strings_different_scopes();
    test_string_punctuation();
    test_single_char_string();
    test_string_escaped_quotes();
    test_many_strings();
    
    std::cout << "\n=========================================\n";
    std::cout << "String Tests Complete - 15 tests\n";
    std::cout << "All tests verify GCC compilation success\n";
    std::cout << "=========================================\n";
    
    return 0;
}
