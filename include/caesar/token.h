/**
 * @file token.h
 * @brief Token definitions for the Caesar lexer
 * @author J.J.G. Pleunes
 * @version 1.0.0
 */

#ifndef CAESAR_TOKEN_H
#define CAESAR_TOKEN_H

#include <string>
#include <ostream>

namespace caesar {

/**
 * @brief Enumeration of all token types in Caesar
 */
enum class TokenType {
    // Literals
    INTEGER,        ///< Integer literal (42, 123)
    FLOAT,          ///< Float literal (3.14, 2.0)
    STRING,         ///< String literal ("hello", 'world')
    BOOLEAN,        ///< Boolean literal (True, False)
    
    // Identifiers and keywords
    IDENTIFIER,     ///< Variable/function names
    
    // Keywords
    IF,             ///< if keyword
    ELIF,           ///< elif keyword
    ELSE,           ///< else keyword
    WHILE,          ///< while keyword
    FOR,            ///< for keyword
    IN,             ///< in keyword
    DEF,            ///< def keyword (function definition)
    CLASS,          ///< class keyword
    RETURN,         ///< return keyword
    BREAK,          ///< break keyword
    CONTINUE,       ///< continue keyword
    PASS,           ///< pass keyword
    AND,            ///< and keyword
    OR,             ///< or keyword
    NOT,            ///< not keyword
    IS,             ///< is keyword
    NONE,           ///< None keyword
    
    // Operators
    PLUS,           ///< +
    MINUS,          ///< -
    MULTIPLY,       ///< *
    DIVIDE,         ///< /
    FLOOR_DIVIDE,   ///< //
    MODULO,         ///< %
    POWER,          ///< **
    
    // Assignment
    ASSIGN,         ///< =
    PLUS_ASSIGN,    ///< +=
    MINUS_ASSIGN,   ///< -=
    MULT_ASSIGN,    ///< *=
    DIV_ASSIGN,     ///< /=
    
    // Comparison
    EQUAL,          ///< ==
    NOT_EQUAL,      ///< !=
    LESS,           ///< <
    LESS_EQUAL,     ///< <=
    GREATER,        ///< >
    GREATER_EQUAL,  ///< >=
    
    // Delimiters
    LPAREN,         ///< (
    RPAREN,         ///< )
    LBRACKET,       ///< [
    RBRACKET,       ///< ]
    LBRACE,         ///< {
    RBRACE,         ///< }
    COMMA,          ///< ,
    COLON,          ///< :
    SEMICOLON,      ///< ;
    DOT,            ///< .
    
    // Special
    NEWLINE,        ///< Newline character
    INDENT,         ///< Indentation increase
    DEDENT,         ///< Indentation decrease
    EOF_TOKEN,      ///< End of file
    
    // Error
    UNKNOWN         ///< Unknown/invalid token
};

/**
 * @brief Position information for tokens
 */
struct Position {
    size_t line;      ///< Line number (1-based)
    size_t column;    ///< Column number (1-based)
    
    Position(size_t l = 1, size_t c = 1) : line(l), column(c) {}
};

/**
 * @brief Token structure containing type, value, and position
 */
struct Token {
    TokenType type;         ///< Type of the token
    std::string value;      ///< String representation of the token
    Position position;      ///< Position in source code
    
    /**
     * @brief Construct a token
     * @param t Token type
     * @param v Token value
     * @param pos Position in source
     */
    Token(TokenType t, const std::string& v, const Position& pos)
        : type(t), value(v), position(pos) {}
    
    /**
     * @brief Convert token type to string
     * @return String representation of token type
     */
    std::string typeToString() const;
    
    /**
     * @brief Check if token is a keyword
     * @return true if token is a keyword
     */
    bool isKeyword() const;
    
    /**
     * @brief Check if token is an operator
     * @return true if token is an operator
     */
    bool isOperator() const;
};

/**
 * @brief Stream operator for tokens
 * @param os Output stream
 * @param token Token to output
 * @return Reference to output stream
 */
std::ostream& operator<<(std::ostream& os, const Token& token);

} // namespace caesar

#endif // CAESAR_TOKEN_H
