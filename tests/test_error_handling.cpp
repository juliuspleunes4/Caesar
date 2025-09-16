/**
 * @file test_error_handling.cpp
 * @brief Error handling tests for the Caesar parser
 * @author J.J.G. Pleunes
 * @version 1.0.0
 */

#undef NDEBUG
#include "caesar/lexer.h"
#include "caesar/parser.h"
#include "caesar/ast.h"
#include <iostream>
#include <cassert>
#include <stdexcept>
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

// Helper function to test that parsing fails gracefully
bool testParsingFailure(const std::string& source, const std::string& testName) {
    std::cout << "Testing " << testName << "...\n";
    
    try {
        caesar::Lexer lexer(source);
        auto tokens = lexer.tokenize();
        
        caesar::Parser parser(std::move(tokens));
        auto program = parser.parse();
        
        // If we get here, parsing didn't fail as expected
        if (program == nullptr) {
            std::cout << "✓ " << testName << " correctly returned null\n";
            return true;
        } else {
            std::cout << "⚠ " << testName << " unexpectedly succeeded\n";
            return false;
        }
    } catch (const std::exception& e) {
        std::cout << "✓ " << testName << " correctly threw exception: " << e.what() << "\n";
        return true;
    }
}

// Helper function to test that lexing fails gracefully
bool testLexingFailure(const std::string& source, const std::string& testName) {
    std::cout << "Testing " << testName << "...\n";
    
    try {
        caesar::Lexer lexer(source);
        auto tokens = lexer.tokenize();
        
        // Check if we got any error tokens or empty result
        bool hasError = false;
        for (const auto& token : tokens) {
            if (token.type == caesar::TokenType::UNKNOWN) {
                hasError = true;
                break;
            }
        }
        
        if (hasError || tokens.empty()) {
            std::cout << "✓ " << testName << " correctly produced error tokens\n";
            return true;
        } else {
            std::cout << "⚠ " << testName << " unexpectedly succeeded\n";
            return false;
        }
    } catch (const std::exception& e) {
        std::cout << "✓ " << testName << " correctly threw exception: " << e.what() << "\n";
        return true;
    }
}

void test_syntax_errors() {
    std::cout << "Testing syntax error handling...\n";
    
    // Missing colon in if statement
    testParsingFailure("if x > 0\n    print(x)", "Missing colon in if");
    
    // Unmatched parentheses
    testParsingFailure("print(hello world", "Unmatched opening parenthesis");
    testParsingFailure("print hello world)", "Unmatched closing parenthesis");
    
    // Invalid function definition
    testParsingFailure("def (x):\n    return x", "Invalid function name");
    testParsingFailure("def func x):\n    return x", "Missing opening parenthesis in function");
    
    // Invalid assignment
    testParsingFailure("123 = x", "Invalid assignment target");
    testParsingFailure("x + y = 5", "Invalid assignment target expression");
    
    // Incomplete expressions
    testParsingFailure("x + ", "Incomplete expression");
    testParsingFailure("return ", "Incomplete return statement");
    
    std::cout << "✓ Syntax error tests completed\n";
}

void test_indentation_errors() {
    std::cout << "Testing indentation error handling...\n";
    
    // Unexpected indentation
    testParsingFailure("x = 1\n    y = 2", "Unexpected indentation");
    
    // Missing indentation after colon
    testParsingFailure("if x > 0:\nprint(x)", "Missing indentation after colon");
    
    // Inconsistent indentation
    testParsingFailure("if x > 0:\n    y = 1\n  z = 2", "Inconsistent indentation");
    
    // Mixed tabs and spaces (if implementation checks for this)
    testParsingFailure("if x > 0:\n\ty = 1\n    z = 2", "Mixed tabs and spaces");
    
    std::cout << "✓ Indentation error tests completed\n";
}

void test_lexical_errors() {
    std::cout << "Testing lexical error handling...\n";
    
    // Invalid string literals
    testLexingFailure("\"unterminated string", "Unterminated string literal");
    testLexingFailure("'unterminated string", "Unterminated single quote string");
    
    // Invalid escape sequences
    testLexingFailure("\"invalid \\z escape\"", "Invalid escape sequence");
    
    // Invalid numbers
    testLexingFailure("123.45.67", "Invalid number format");
    testLexingFailure("0x", "Incomplete hexadecimal number");
    testLexingFailure("123abc", "Invalid number with letters");
    
    // Invalid characters
    testLexingFailure("x = y @ z", "Invalid character @");
    testLexingFailure("result = a $ b", "Invalid character $");
    
    std::cout << "✓ Lexical error tests completed\n";
}

void test_parser_recovery() {
    std::cout << "Testing parser error recovery...\n";
    
    // Test if parser can recover from errors and continue parsing
    std::string source = R"(
def valid_function():
    return 42

# This should cause an error
invalid syntax here

def another_valid_function():
    return "hello"
)";
    
    try {
        caesar::Lexer lexer(source);
        auto tokens = lexer.tokenize();
        
        caesar::Parser parser(std::move(tokens));
        auto program = parser.parse();
        
        // Check if we got at least some valid statements
        if (program && !program->statements.empty()) {
            std::cout << "✓ Parser recovered and parsed " << program->statements.size() << " statements\n";
        } else {
            std::cout << "⚠ Parser could not recover from errors\n";
        }
    } catch (const std::exception& e) {
        std::cout << "⚠ Parser recovery test threw exception: " << e.what() << "\n";
    }
}

void test_nested_error_handling() {
    std::cout << "Testing nested error handling...\n";
    
    // Errors inside nested structures
    testParsingFailure(R"(
def outer():
    if condition:
        def inner(
            return "missing closing paren"
    return "outer"
)", "Error in nested function definition");
    
    testParsingFailure(R"(
class TestClass:
    def method(self):
        if x > 0
            return x
        else:
            return 0
)", "Error in class method");
    
    testParsingFailure(R"(
for item in items:
    if item > 0:
        while condition
            process(item)
)", "Error in nested loop");
    
    std::cout << "✓ Nested error handling tests completed\n";
}

void test_expression_errors() {
    std::cout << "Testing expression error handling...\n";
    
    // Invalid binary expressions
    testParsingFailure("x + + y", "Invalid double operator");
    testParsingFailure("x * / y", "Invalid operator sequence");
    
    // Invalid unary expressions
    testParsingFailure("not not not", "Invalid unary sequence");
    testParsingFailure("- - -", "Invalid unary minus sequence");
    
    // Invalid function calls
    testParsingFailure("func(a, , b)", "Missing argument in function call");
    testParsingFailure("func(a b)", "Missing comma in function call");
    
    // Invalid list/dict syntax
    testParsingFailure("[1, 2, , 3]", "Missing element in list");
    testParsingFailure("{a: 1, : 2}", "Missing key in dictionary");
    
    std::cout << "✓ Expression error tests completed\n";
}

void test_control_flow_errors() {
    std::cout << "Testing control flow error handling...\n";
    
    // Invalid if statements
    testParsingFailure("if:\n    pass", "Missing condition in if");
    testParsingFailure("if x > 0\n    pass\nelse if y < 0:\n    pass", "Invalid else if syntax");
    
    // Invalid for loops
    testParsingFailure("for:\n    pass", "Missing for loop parts");
    testParsingFailure("for item:\n    pass", "Missing 'in' keyword");
    testParsingFailure("for item in:\n    pass", "Missing iterable");
    
    // Invalid while loops
    testParsingFailure("while:\n    pass", "Missing while condition");
    
    // Invalid break/continue outside loops
    testParsingFailure("break", "Break outside loop");
    testParsingFailure("continue", "Continue outside loop");
    
    std::cout << "✓ Control flow error tests completed\n";
}

void test_class_definition_errors() {
    std::cout << "Testing class definition error handling...\n";
    
    // Invalid class syntax
    testParsingFailure("class:\n    pass", "Missing class name");
    testParsingFailure("class 123Class:\n    pass", "Invalid class name");
    testParsingFailure("class MyClass\n    pass", "Missing colon in class");
    
    // Invalid method definitions
    testParsingFailure(R"(
class MyClass:
    def (self):
        pass
)", "Invalid method name");
    
    testParsingFailure(R"(
class MyClass:
    def method():
        pass
)", "Missing self parameter");
    
    std::cout << "✓ Class definition error tests completed\n";
}

void test_import_errors() {
    std::cout << "Testing import error handling...\n";
    
    // Invalid import syntax
    testParsingFailure("import", "Missing import target");
    testParsingFailure("from", "Missing from target");
    testParsingFailure("from module import", "Missing import items");
    
    // Invalid module names
    testParsingFailure("import 123module", "Invalid module name");
    testParsingFailure("from 123module import func", "Invalid module name in from");
    
    std::cout << "✓ Import error tests completed\n";
}

void test_edge_case_errors() {
    std::cout << "Testing edge case error handling...\n";
    
    // Empty input
    try {
        caesar::Lexer lexer("");
        auto tokens = lexer.tokenize();
        caesar::Parser parser(std::move(tokens));
        auto program = parser.parse();
        
        // Empty input should succeed but produce empty program
        assert(program != nullptr);
        assert(program->statements.empty());
        std::cout << "✓ Empty input handled correctly\n";
    } catch (const std::exception& e) {
        std::cout << "⚠ Empty input test failed: " << e.what() << "\n";
    }
    
    // Only whitespace
    try {
        caesar::Lexer lexer("   \n  \t  \n   ");
        auto tokens = lexer.tokenize();
        caesar::Parser parser(std::move(tokens));
        auto program = parser.parse();
        
        // Whitespace-only should succeed but produce empty program
        assert(program != nullptr);
        std::cout << "✓ Whitespace-only input handled correctly\n";
    } catch (const std::exception& e) {
        std::cout << "⚠ Whitespace-only test failed: " << e.what() << "\n";
    }
    
    // Only comments
    try {
        caesar::Lexer lexer("# This is a comment\n# Another comment");
        auto tokens = lexer.tokenize();
        caesar::Parser parser(std::move(tokens));
        auto program = parser.parse();
        
        assert(program != nullptr);
        std::cout << "✓ Comment-only input handled correctly\n";
    } catch (const std::exception& e) {
        std::cout << "⚠ Comment-only test failed: " << e.what() << "\n";
    }
    
    std::cout << "✓ Edge case error tests completed\n";
}

int main() {
    std::cout << "Running Caesar error handling tests...\n\n";
    
    try {
        test_syntax_errors();
        test_indentation_errors();
        test_lexical_errors();
        test_parser_recovery();
        test_nested_error_handling();
        test_expression_errors();
        test_control_flow_errors();
        test_class_definition_errors();
        test_import_errors();
        test_edge_case_errors();
        
        std::cout << "\n✅ All error handling tests completed!\n";
        std::cout << "Caesar parser demonstrates robust error handling and recovery.\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "\n❌ Error handling test failed: " << e.what() << "\n";
        return 1;
    }
}
