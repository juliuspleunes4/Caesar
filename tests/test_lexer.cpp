/**
 * @file test_lexer.cpp
 * @brief Unit tests for the Caesar lexer
 * @author J.J.G. Pleunes
 * @version 1.0.0
 */

#undef NDEBUG
#include "caesar/lexer.h"
#include <iostream>
#include <cassert>
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

// Also define regular assert for consistency
#ifndef assert
#define assert my_assert
#endif

void test_basic_tokens() {
    std::cout << "Testing basic tokens...\n";
    
    caesar::Lexer lexer("123 3.14 \"hello\" + - * /");
    auto tokens = lexer.tokenize();
    
    my_assert(tokens.size() >= 8); // At least 7 tokens + EOF
    my_assert(tokens[0].type == caesar::TokenType::INTEGER);
    my_assert(tokens[0].value == "123");
    my_assert(tokens[1].type == caesar::TokenType::FLOAT);
    my_assert(tokens[1].value == "3.14");
    my_assert(tokens[2].type == caesar::TokenType::STRING);
    my_assert(tokens[2].value == "hello");
    my_assert(tokens[3].type == caesar::TokenType::PLUS);
    my_assert(tokens[4].type == caesar::TokenType::MINUS);
    my_assert(tokens[5].type == caesar::TokenType::MULTIPLY);
    my_assert(tokens[6].type == caesar::TokenType::DIVIDE);
    
    std::cout << "✓ Basic tokens test passed\n";
}

void test_keywords() {
    std::cout << "Testing keywords...\n";
    
    caesar::Lexer lexer("if else while for def class return");
    auto tokens = lexer.tokenize();
    
    assert(tokens[0].type == caesar::TokenType::IF);
    assert(tokens[1].type == caesar::TokenType::ELSE);
    assert(tokens[2].type == caesar::TokenType::WHILE);
    assert(tokens[3].type == caesar::TokenType::FOR);
    assert(tokens[4].type == caesar::TokenType::DEF);
    assert(tokens[5].type == caesar::TokenType::CLASS);
    assert(tokens[6].type == caesar::TokenType::RETURN);
    
    std::cout << "✓ Keywords test passed\n";
}

void test_identifiers() {
    std::cout << "Testing identifiers...\n";
    
    caesar::Lexer lexer("variable_name func123 _private");
    auto tokens = lexer.tokenize();
    
    assert(tokens[0].type == caesar::TokenType::IDENTIFIER);
    assert(tokens[0].value == "variable_name");
    assert(tokens[1].type == caesar::TokenType::IDENTIFIER);
    assert(tokens[1].value == "func123");
    assert(tokens[2].type == caesar::TokenType::IDENTIFIER);
    assert(tokens[2].value == "_private");
    
    std::cout << "✓ Identifiers test passed\n";
}

void test_operators() {
    std::cout << "Testing operators...\n";
    
    caesar::Lexer lexer("== != <= >= += -= *= /= **");
    auto tokens = lexer.tokenize();
    
    assert(tokens[0].type == caesar::TokenType::EQUAL);
    assert(tokens[1].type == caesar::TokenType::NOT_EQUAL);
    assert(tokens[2].type == caesar::TokenType::LESS_EQUAL);
    assert(tokens[3].type == caesar::TokenType::GREATER_EQUAL);
    assert(tokens[4].type == caesar::TokenType::PLUS_ASSIGN);
    assert(tokens[5].type == caesar::TokenType::MINUS_ASSIGN);
    assert(tokens[6].type == caesar::TokenType::MULT_ASSIGN);
    assert(tokens[7].type == caesar::TokenType::DIV_ASSIGN);
    assert(tokens[8].type == caesar::TokenType::POWER);
    
    std::cout << "✓ Operators test passed\n";
}

void test_string_literals() {
    std::cout << "Testing string literals...\n";
    
    caesar::Lexer lexer("\"hello world\" 'single quotes' \"with\\nescapes\"");
    auto tokens = lexer.tokenize();
    
    assert(tokens[0].type == caesar::TokenType::STRING);
    assert(tokens[0].value == "hello world");
    assert(tokens[1].type == caesar::TokenType::STRING);
    assert(tokens[1].value == "single quotes");
    assert(tokens[2].type == caesar::TokenType::STRING);
    assert(tokens[2].value == "with\nescapes");
    
    std::cout << "✓ String literals test passed\n";
}

void test_simple_program() {
    std::cout << "Testing simple program...\n";
    
    std::string program = R"(
def fibonacci(n):
    if n <= 1:
        return n
    else:
        return fibonacci(n-1) + fibonacci(n-2)
)";
    
    caesar::Lexer lexer(program);
    auto tokens = lexer.tokenize();
    
    // Just verify we can tokenize without errors
    bool found_def = false, found_if = false, found_return = false;
    for (const auto& token : tokens) {
        if (token.type == caesar::TokenType::DEF) found_def = true;
        if (token.type == caesar::TokenType::IF) found_if = true;
        if (token.type == caesar::TokenType::RETURN) found_return = true;
    }
    
    assert(found_def && found_if && found_return);
    
    std::cout << "✓ Simple program test passed\n";
}

int main() {
    std::cout << "Running Caesar lexer tests...\n\n";
    
    try {
        test_basic_tokens();
        test_keywords();
        test_identifiers();
        test_operators();
        test_string_literals();
        test_simple_program();
        
        std::cout << "\n✅ All tests passed!\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "\n❌ Test failed: " << e.what() << "\n";
        return 1;
    }
}
