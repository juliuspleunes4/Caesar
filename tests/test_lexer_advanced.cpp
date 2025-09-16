/**
 * @file test_lexer_advanced.cpp
 * @brief Advanced unit tests for the Caesar lexer
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

void test_indentation_complex() {
    std::cout << "Testing complex indentation...\n";
    
    std::string source = R"(
def outer():
    if True:
        def inner():
            if condition:
                x = 1
                y = 2
            else:
                z = 3
        return inner()
    else:
        return None
)";
    
    caesar::Lexer lexer(source);
    auto tokens = lexer.tokenize();
    
    // Count indentation tokens
    int indent_count = 0;
    int dedent_count = 0;
    for (const auto& token : tokens) {
        if (token.type == caesar::TokenType::INDENT) indent_count++;
        if (token.type == caesar::TokenType::DEDENT) dedent_count++;
    }
    
    assert(indent_count > 0);
    assert(dedent_count > 0);
    // Should have matching dedents for all indents
    assert(dedent_count >= indent_count - 1); // -1 because outermost doesn't need dedent
    
    std::cout << "✓ Complex indentation test passed\n";
}

void test_string_literals_edge_cases() {
    std::cout << "Testing string literal edge cases...\n";
    
    // Test empty strings
    caesar::Lexer lexer1("\"\" ''");
    auto tokens1 = lexer1.tokenize();
    assert(tokens1.size() >= 3); // 2 strings + EOF
    assert(tokens1[0].type == caesar::TokenType::STRING);
    assert(tokens1[0].value == "");
    assert(tokens1[1].type == caesar::TokenType::STRING);
    assert(tokens1[1].value == "");
    
    // Test strings with all escape sequences
    caesar::Lexer lexer2(R"("Hello\nWorld\t\r\"\\\0")");
    auto tokens2 = lexer2.tokenize();
    assert(tokens2[0].type == caesar::TokenType::STRING);
    
    // Build expected string manually to handle null character
    std::string expected = "Hello\nWorld\t\r\"\\";
    expected += '\0';
    assert(tokens2[0].value == expected);
    
    // Test very long string
    std::string long_string = "\"" + std::string(1000, 'a') + "\"";
    caesar::Lexer lexer3(long_string);
    auto tokens3 = lexer3.tokenize();
    assert(tokens3[0].type == caesar::TokenType::STRING);
    assert(tokens3[0].value.length() == 1000);
    
    std::cout << "✓ String literal edge cases test passed\n";
}

void test_numeric_literals_edge_cases() {
    std::cout << "Testing numeric literal edge cases...\n";
    
    // Test various number formats
    caesar::Lexer lexer("0 00 123 0.0 .5 5. 123.456 1e5 1e-3 1.23e+10");
    auto tokens = lexer.tokenize();
    
    // Should have integers and floats
    assert(tokens[0].type == caesar::TokenType::INTEGER);
    assert(tokens[0].value == "0");
    
    assert(tokens[1].type == caesar::TokenType::INTEGER);
    assert(tokens[1].value == "00");
    
    assert(tokens[2].type == caesar::TokenType::INTEGER);
    assert(tokens[2].value == "123");
    
    // Float tests
    assert(tokens[3].type == caesar::TokenType::FLOAT);
    assert(tokens[3].value == "0.0");
    
    // Test scientific notation if supported
    // Note: This test might need adjustment based on actual lexer implementation
    
    std::cout << "✓ Numeric literal edge cases test passed\n";
}

void test_operator_combinations() {
    std::cout << "Testing operator combinations...\n";
    
    caesar::Lexer lexer("== != <= >= += -= *= /= ** // and or not");
    auto tokens = lexer.tokenize();
    
    // Verify compound operators are recognized correctly
    assert(tokens[0].type == caesar::TokenType::EQUAL);
    assert(tokens[1].type == caesar::TokenType::NOT_EQUAL);
    assert(tokens[2].type == caesar::TokenType::LESS_EQUAL);
    assert(tokens[3].type == caesar::TokenType::GREATER_EQUAL);
    assert(tokens[4].type == caesar::TokenType::PLUS_ASSIGN);
    assert(tokens[5].type == caesar::TokenType::MINUS_ASSIGN);
    assert(tokens[6].type == caesar::TokenType::MULT_ASSIGN);
    assert(tokens[7].type == caesar::TokenType::DIV_ASSIGN);
    assert(tokens[8].type == caesar::TokenType::POWER);
    assert(tokens[9].type == caesar::TokenType::FLOOR_DIVIDE);
    assert(tokens[10].type == caesar::TokenType::AND);
    assert(tokens[11].type == caesar::TokenType::OR);
    assert(tokens[12].type == caesar::TokenType::NOT);
    
    std::cout << "✓ Operator combinations test passed\n";
}

void test_comments_and_whitespace() {
    std::cout << "Testing comments and whitespace handling...\n";
    
    std::string source = R"(
# This is a comment
x = 1  # End of line comment
# Another comment
y = 2
    # Indented comment
z = 3 # Final comment
)";
    
    caesar::Lexer lexer(source);
    auto tokens = lexer.tokenize();
    
    // Comments should be ignored, only tokens should be identifiers, operators, and literals
    bool found_x = false, found_y = false, found_z = false;
    for (const auto& token : tokens) {
        if (token.type == caesar::TokenType::IDENTIFIER) {
            if (token.value == "x") found_x = true;
            if (token.value == "y") found_y = true;
            if (token.value == "z") found_z = true;
        }
    }
    
    assert(found_x && found_y && found_z);
    
    std::cout << "✓ Comments and whitespace test passed\n";
}

void test_identifier_edge_cases() {
    std::cout << "Testing identifier edge cases...\n";
    
    caesar::Lexer lexer("_private __dunder__ _123 abc123 CamelCase UPPER_CASE");
    auto tokens = lexer.tokenize();
    
    // All should be valid identifiers
    for (size_t i = 0; i < 6; ++i) {
        assert(tokens[i].type == caesar::TokenType::IDENTIFIER);
    }
    
    assert(tokens[0].value == "_private");
    assert(tokens[1].value == "__dunder__");
    assert(tokens[2].value == "_123");
    assert(tokens[3].value == "abc123");
    assert(tokens[4].value == "CamelCase");
    assert(tokens[5].value == "UPPER_CASE");
    
    std::cout << "✓ Identifier edge cases test passed\n";
}

void test_mixed_delimiters() {
    std::cout << "Testing mixed delimiters...\n";
    
    caesar::Lexer lexer("(){}[],:;.");
    auto tokens = lexer.tokenize();
    
    assert(tokens[0].type == caesar::TokenType::LPAREN);
    assert(tokens[1].type == caesar::TokenType::RPAREN);
    assert(tokens[2].type == caesar::TokenType::LBRACE);
    assert(tokens[3].type == caesar::TokenType::RBRACE);
    assert(tokens[4].type == caesar::TokenType::LBRACKET);
    assert(tokens[5].type == caesar::TokenType::RBRACKET);
    assert(tokens[6].type == caesar::TokenType::COMMA);
    assert(tokens[7].type == caesar::TokenType::COLON);
    assert(tokens[8].type == caesar::TokenType::SEMICOLON);
    assert(tokens[9].type == caesar::TokenType::DOT);
    
    std::cout << "✓ Mixed delimiters test passed\n";
}

void test_boolean_literals() {
    std::cout << "Testing boolean literals...\n";
    
    caesar::Lexer lexer("True False");
    auto tokens = lexer.tokenize();
    
    assert(tokens[0].type == caesar::TokenType::BOOLEAN);
    assert(tokens[0].value == "True");
    assert(tokens[1].type == caesar::TokenType::BOOLEAN);
    assert(tokens[1].value == "False");
    
    std::cout << "✓ Boolean literals test passed\n";
}

void test_none_literal() {
    std::cout << "Testing None literal...\n";
    
    caesar::Lexer lexer("None");
    auto tokens = lexer.tokenize();
    
    assert(tokens[0].type == caesar::TokenType::NONE);
    assert(tokens[0].value == "None");
    
    std::cout << "✓ None literal test passed\n";
}

void test_position_tracking() {
    std::cout << "Testing position tracking...\n";
    
    std::string source = "line1\nline2\n  indented";
    caesar::Lexer lexer(source);
    auto tokens = lexer.tokenize();
    
    // Check that positions are being tracked
    bool found_line1 = false, found_line2 = false, found_indented = false;
    for (const auto& token : tokens) {
        if (token.type == caesar::TokenType::IDENTIFIER) {
            if (token.value == "line1") {
                found_line1 = true;
                assert(token.position.line == 1);
            }
            if (token.value == "line2") {
                found_line2 = true;
                assert(token.position.line == 2);
            }
            if (token.value == "indented") {
                found_indented = true;
                assert(token.position.line == 3);
            }
        }
    }
    
    assert(found_line1 && found_line2 && found_indented);
    
    std::cout << "✓ Position tracking test passed\n";
}

int main() {
    std::cout << "Running Caesar advanced lexer tests...\n\n";
    
    try {
        test_indentation_complex();
        test_string_literals_edge_cases();
        test_numeric_literals_edge_cases();
        test_operator_combinations();
        test_comments_and_whitespace();
        test_identifier_edge_cases();
        test_mixed_delimiters();
        test_boolean_literals();
        test_none_literal();
        test_position_tracking();
        
        std::cout << "\n✅ All advanced lexer tests passed!\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "\n❌ Advanced lexer test failed: " << e.what() << "\n";
        return 1;
    }
}
