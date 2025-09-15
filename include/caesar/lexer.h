/**
 * @file lexer.h
 * @brief Lexical analyzer for the Caesar programming language
 * @author Julius Pleunes
 * @version 1.0.0
 */

#ifndef CAESAR_LEXER_H
#define CAESAR_LEXER_H

#include "caesar/caesar.h"
#include "caesar/token.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <stack>

namespace caesar {

/**
 * @brief Lexical analyzer that converts source code into tokens
 * 
 * The lexer handles Python-like syntax including:
 * - Indentation-based block structure
 * - String literals with various quote styles
 * - Numeric literals (integers and floats)
 * - Keywords and identifiers
 * - Operators and delimiters
 */
class Lexer {
private:
    std::string source_;              ///< Source code being tokenized
    size_t current_;                  ///< Current position in source
    Position position_;               ///< Current line and column
    std::stack<size_t> indent_stack_; ///< Stack to track indentation levels
    bool at_line_start_;              ///< Whether we're at the start of a line
    
    /// Keywords mapping
    static const std::unordered_map<std::string, TokenType> keywords_;
    
public:
    /**
     * @brief Construct a lexer with source code
     * @param source Source code to tokenize
     */
    explicit Lexer(const std::string& source);
    
    /**
     * @brief Tokenize the entire source code
     * @return Vector of tokens
     * @throws LexerException on tokenization errors
     */
    std::vector<Token> tokenize();
    
    /**
     * @brief Get the next token from the source
     * @return Next token
     * @throws LexerException on tokenization errors
     */
    Token nextToken();
    
private:
    /**
     * @brief Check if we've reached the end of source
     * @return true if at end
     */
    bool isAtEnd() const;
    
    /**
     * @brief Get current character without advancing
     * @return Current character or '\0' if at end
     */
    char peek() const;
    
    /**
     * @brief Get next character without advancing
     * @return Next character or '\0' if at end
     */
    char peekNext() const;
    
    /**
     * @brief Advance to next character
     * @return Previous character
     */
    char advance();
    
    /**
     * @brief Skip whitespace (but not newlines)
     */
    void skipWhitespace();
    
    /**
     * @brief Skip comments starting with #
     */
    void skipComment();
    
    /**
     * @brief Handle indentation at start of line
     * @return Vector of INDENT/DEDENT tokens
     */
    std::vector<Token> handleIndentation();
    
    /**
     * @brief Tokenize a string literal
     * @param quote_char The quote character (' or ")
     * @return String token
     */
    Token tokenizeString(char quote_char);
    
    /**
     * @brief Tokenize a numeric literal
     * @return Integer or float token
     */
    Token tokenizeNumber();
    
    /**
     * @brief Tokenize an identifier or keyword
     * @return Identifier or keyword token
     */
    Token tokenizeIdentifier();
    
    /**
     * @brief Check if character is a digit
     * @param c Character to check
     * @return true if digit
     */
    bool isDigit(char c) const;
    
    /**
     * @brief Check if character is alphabetic
     * @param c Character to check
     * @return true if alphabetic
     */
    bool isAlpha(char c) const;
    
    /**
     * @brief Check if character is alphanumeric or underscore
     * @param c Character to check
     * @return true if alphanumeric or underscore
     */
    bool isAlphaNumeric(char c) const;
    
    /**
     * @brief Create a token at current position
     * @param type Token type
     * @param value Token value
     * @return Created token
     */
    Token makeToken(TokenType type, const std::string& value) const;
    
    /**
     * @brief Throw a lexer exception with position info
     * @param message Error message
     */
    void error(const std::string& message) const;
};

} // namespace caesar

#endif // CAESAR_LEXER_H