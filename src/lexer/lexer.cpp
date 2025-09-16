/**
 * @file lexer.cpp
 * @brief Implementation of the Caesar lexer
 * @author Julius Pleunes
 * @version 1.0.0
 */

#include "caesar/lexer.h"
#include <cctype>
#include <vector>
#include <cstddef>
#include <algorithm>
#include <iostream>

// Ensure size_t is available
using std::size_t;
using std::vector;

namespace caesar {

// Keywords mapping
const std::unordered_map<std::string, TokenType> Lexer::keywords_ = {
    {"if", TokenType::IF},
    {"elif", TokenType::ELIF},
    {"else", TokenType::ELSE},
    {"while", TokenType::WHILE},
    {"for", TokenType::FOR},
    {"in", TokenType::IN},
    {"def", TokenType::DEF},
    {"class", TokenType::CLASS},
    {"return", TokenType::RETURN},
    {"break", TokenType::BREAK},
    {"continue", TokenType::CONTINUE},
    {"pass", TokenType::PASS},
    {"and", TokenType::AND},
    {"or", TokenType::OR},
    {"not", TokenType::NOT},
    {"is", TokenType::IS},
    {"None", TokenType::NONE},
    {"True", TokenType::BOOLEAN},
    {"False", TokenType::BOOLEAN}
};

Lexer::Lexer(const std::string& source) 
    : source_(source), current_(0), position_(1, 1), at_line_start_(true) {
    indent_stack_.push(0); // Base indentation level
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    
    while (!isAtEnd()) {
        Token token = nextToken();
        tokens.push_back(token);
        
        if (token.type == TokenType::EOF_TOKEN) {
            break;
        }
    }
    
    // Add any remaining DEDENT tokens
    while (indent_stack_.size() > 1) {
        indent_stack_.pop();
        tokens.push_back(makeToken(TokenType::DEDENT, ""));
    }
    
    // Ensure EOF token is last
    if (tokens.empty() || tokens.back().type != TokenType::EOF_TOKEN) {
        tokens.push_back(makeToken(TokenType::EOF_TOKEN, ""));
    }
    
    return tokens;
}

Token Lexer::nextToken() {
    if (isAtEnd()) {
        return makeToken(TokenType::EOF_TOKEN, "");
    }
    
    // Handle indentation at start of line
    if (at_line_start_) {
        at_line_start_ = false;
        auto indent_tokens = handleIndentation();
        if (!indent_tokens.empty()) {
            // Return first indentation token, store others for later
            return indent_tokens[0];
        }
    }
    
    skipWhitespace();
    
    if (isAtEnd()) {
        return makeToken(TokenType::EOF_TOKEN, "");
    }
    
    char c = advance();
    
    // Handle newlines
    if (c == '\n') {
        at_line_start_ = true;
        return makeToken(TokenType::NEWLINE, "\n");
    }
    
    // Handle comments
    if (c == '#') {
        skipComment();
        return nextToken(); // Get next token after comment
    }
    
    // String literals
    if (c == '"' || c == '\'') {
        return tokenizeString(c);
    }
    
    // Numeric literals
    if (isDigit(c)) {
        current_--; // Back up to re-read the digit
        return tokenizeNumber();
    }
    
    // Identifiers and keywords
    if (isAlpha(c)) {
        current_--; // Back up to re-read the character
        return tokenizeIdentifier();
    }
    
    // Two-character operators
    char next = peek();
    switch (c) {
        case '+':
            if (next == '=') { advance(); return makeToken(TokenType::PLUS_ASSIGN, "+="); }
            return makeToken(TokenType::PLUS, "+");
        case '-':
            if (next == '=') { advance(); return makeToken(TokenType::MINUS_ASSIGN, "-="); }
            return makeToken(TokenType::MINUS, "-");
        case '*':
            if (next == '*') { advance(); return makeToken(TokenType::POWER, "**"); }
            if (next == '=') { advance(); return makeToken(TokenType::MULT_ASSIGN, "*="); }
            return makeToken(TokenType::MULTIPLY, "*");
        case '/':
            if (next == '/') { advance(); return makeToken(TokenType::FLOOR_DIVIDE, "//"); }
            if (next == '=') { advance(); return makeToken(TokenType::DIV_ASSIGN, "/="); }
            return makeToken(TokenType::DIVIDE, "/");
        case '=':
            if (next == '=') { advance(); return makeToken(TokenType::EQUAL, "=="); }
            return makeToken(TokenType::ASSIGN, "=");
        case '!':
            if (next == '=') { advance(); return makeToken(TokenType::NOT_EQUAL, "!="); }
            break;
        case '<':
            if (next == '=') { advance(); return makeToken(TokenType::LESS_EQUAL, "<="); }
            return makeToken(TokenType::LESS, "<");
        case '>':
            if (next == '=') { advance(); return makeToken(TokenType::GREATER_EQUAL, ">="); }
            return makeToken(TokenType::GREATER, ">");
    }
    
    // Single-character tokens
    switch (c) {
        case '%': return makeToken(TokenType::MODULO, "%");
        case '(': return makeToken(TokenType::LPAREN, "(");
        case ')': return makeToken(TokenType::RPAREN, ")");
        case '[': return makeToken(TokenType::LBRACKET, "[");
        case ']': return makeToken(TokenType::RBRACKET, "]");
        case '{': return makeToken(TokenType::LBRACE, "{");
        case '}': return makeToken(TokenType::RBRACE, "}");
        case ',': return makeToken(TokenType::COMMA, ",");
        case ':': return makeToken(TokenType::COLON, ":");
        case ';': return makeToken(TokenType::SEMICOLON, ";");
        case '.': return makeToken(TokenType::DOT, ".");
    }
    
    std::string char_str;
    char_str += c;
    error("Unexpected character: " + char_str);
    return makeToken(TokenType::UNKNOWN, char_str);
}

bool Lexer::isAtEnd() const {
    return current_ >= source_.length();
}

char Lexer::peek() const {
    if (isAtEnd()) return '\0';
    return source_[current_];
}

char Lexer::peekNext() const {
    if (current_ + 1 >= source_.length()) return '\0';
    return source_[current_ + 1];
}

char Lexer::advance() {
    if (isAtEnd()) return '\0';
    
    char c = source_[current_++];
    if (c == '\n') {
        position_.line++;
        position_.column = 1;
    } else {
        position_.column++;
    }
    return c;
}

void Lexer::skipWhitespace() {
    while (!isAtEnd() && std::isspace(static_cast<unsigned char>(peek())) && peek() != '\n') {
        advance();
    }
}

void Lexer::skipComment() {
    while (!isAtEnd() && peek() != '\n') {
        advance();
    }
}

std::vector<Token> Lexer::handleIndentation() {
    std::vector<Token> tokens;
    size_t indent_level = 0;
    
    // Count leading spaces/tabs
    while (!isAtEnd() && (peek() == ' ' || peek() == '\t')) {
        if (peek() == ' ') {
            indent_level++;
        } else { // tab
            indent_level += 8; // Treat tab as 8 spaces
        }
        advance();
    }
    
    // Skip empty lines and comments
    if (isAtEnd() || peek() == '\n' || peek() == '#') {
        return tokens;
    }
    
    size_t current_indent = indent_stack_.top();
    
    if (indent_level > current_indent) {
        // Increased indentation
        indent_stack_.push(indent_level);
        tokens.push_back(makeToken(TokenType::INDENT, ""));
    } else if (indent_level < current_indent) {
        // Decreased indentation
        while (!indent_stack_.empty() && indent_stack_.top() > indent_level) {
            indent_stack_.pop();
            tokens.push_back(makeToken(TokenType::DEDENT, ""));
        }
        
        if (indent_stack_.empty() || indent_stack_.top() != indent_level) {
            error("Indentation error: mismatched indentation level");
        }
    }
    
    return tokens;
}

Token Lexer::tokenizeString(char quote_char) {
    std::string value;
    
    while (!isAtEnd() && peek() != quote_char) {
        if (peek() == '\\') {
            advance(); // Skip backslash
            if (isAtEnd()) {
                error("Unterminated string literal");
            }
            
            char escaped = advance();
            switch (escaped) {
                case 'n': value += '\n'; break;
                case 't': value += '\t'; break;
                case 'r': value += '\r'; break;
                case '\\': value += '\\'; break;
                case '\'': value += '\''; break;
                case '"': value += '"'; break;
                case '0': value += '\0'; break;
                default:
                    value += escaped;
                    break;
            }
        } else {
            value += advance();
        }
    }
    
    if (isAtEnd()) {
        error("Unterminated string literal");
    }
    
    advance(); // Consume closing quote
    return makeToken(TokenType::STRING, value);
}

Token Lexer::tokenizeNumber() {
    std::string value;
    bool is_float = false;
    
    while (!isAtEnd() && isDigit(peek())) {
        value += advance();
    }
    
    // Check for decimal point
    if (!isAtEnd() && peek() == '.' && isDigit(peekNext())) {
        is_float = true;
        value += advance(); // Consume '.'
        
        while (!isAtEnd() && isDigit(peek())) {
            value += advance();
        }
    }
    
    TokenType type = is_float ? TokenType::FLOAT : TokenType::INTEGER;
    return makeToken(type, value);
}

Token Lexer::tokenizeIdentifier() {
    std::string value;
    
    while (!isAtEnd() && isAlphaNumeric(peek())) {
        value += advance();
    }
    
    // Check if it's a keyword
    auto it = keywords_.find(value);
    if (it != keywords_.end()) {
        return makeToken(it->second, value);
    }
    
    return makeToken(TokenType::IDENTIFIER, value);
}

bool Lexer::isDigit(char c) const {
    return c >= '0' && c <= '9';
}

bool Lexer::isAlpha(char c) const {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Lexer::isAlphaNumeric(char c) const {
    return isAlpha(c) || isDigit(c);
}

Token Lexer::makeToken(TokenType type, const std::string& value) const {
    return Token(type, value, position_);
}

void Lexer::error(const std::string& message) const {
    throw LexerException(message + " at line " + std::to_string(position_.line) + 
                        ", column " + std::to_string(position_.column));
}

} // namespace caesar