/**
 * @file parser.h
 * @brief Parser for the Caesar programming language
 * @author J.J.G. Pleunes
 * @version 1.0.0
 */

#ifndef CAESAR_PARSER_H
#define CAESAR_PARSER_H

#include "caesar/caesar.h"
#include "caesar/token.h"
#include "caesar/ast.h"
#include <vector>
#include <memory>

namespace caesar {

/**
 * @brief Recursive descent parser for Caesar language
 * 
 * The parser converts a sequence of tokens into an Abstract Syntax Tree (AST).
 * It implements a recursive descent parser with operator precedence for expressions.
 */
class Parser {
private:
    std::vector<Token> tokens_;    ///< Tokens to parse
    size_t current_;               ///< Current position in tokens

public:
    /**
     * @brief Construct parser with tokens
     * @param tokens Vector of tokens to parse
     */
    explicit Parser(std::vector<Token> tokens);
    
    /**
     * @brief Parse tokens into AST
     * @return Root Program node
     * @throws ParserException on parsing errors
     */
    std::unique_ptr<Program> parse();

private:
    // Utility methods
    
    /**
     * @brief Check if we've reached end of tokens
     * @return true if at end
     */
    bool isAtEnd() const;
    
    /**
     * @brief Get current token without advancing
     * @return Current token
     */
    const Token& peek() const;
    
    /**
     * @brief Get previous token
     * @return Previous token
     */
    const Token& previous() const;
    
    /**
     * @brief Advance to next token
     * @return Previous token
     */
    Token advance();
    
    /**
     * @brief Check if current token matches type
     * @param type Token type to check
     * @return true if matches
     */
    bool check(TokenType type) const;
    
    /**
     * @brief Check if current token matches any of the types
     * @param types Vector of token types
     * @return true if match found (and advances)
     */
    bool match(const std::vector<TokenType>& types);
    
    /**
     * @brief Consume token of expected type
     * @param type Expected token type
     * @param message Error message if not found
     * @return Consumed token
     * @throws ParserException if wrong type
     */
    Token consume(TokenType type, const std::string& message);
    
    /**
     * @brief Skip newline tokens
     */
    void skipNewlines();
    
    /**
     * @brief Throw parser exception
     * @param message Error message
     * @throws ParserException
     */
    void error(const std::string& message) const;
    
    // Grammar rules (in order of precedence)
    
    /**
     * @brief Parse entire program
     * @return Program AST node
     */
    std::unique_ptr<Program> program();
    
    /**
     * @brief Parse statement
     * @return Statement AST node
     */
    std::unique_ptr<Statement> statement();
    
    /**
     * @brief Parse function definition
     * @return FunctionDefinition AST node
     */
    std::unique_ptr<FunctionDefinition> functionDefinition();
    
    /**
     * @brief Parse class definition
     * @return ClassDefinition AST node
     */
    std::unique_ptr<ClassDefinition> classDefinition();
    
    /**
     * @brief Parse if statement
     * @return IfStatement AST node
     */
    std::unique_ptr<IfStatement> ifStatement();
    
    /**
     * @brief Parse while statement
     * @return WhileStatement AST node
     */
    std::unique_ptr<WhileStatement> whileStatement();
    
    /**
     * @brief Parse for statement
     * @return ForStatement AST node
     */
    std::unique_ptr<ForStatement> forStatement();
    
    /**
     * @brief Parse return statement
     * @return ReturnStatement AST node
     */
    std::unique_ptr<ReturnStatement> returnStatement();
    
    /**
     * @brief Parse break statement
     * @return BreakStatement AST node
     */
    std::unique_ptr<BreakStatement> breakStatement();
    
    /**
     * @brief Parse continue statement
     * @return ContinueStatement AST node
     */
    std::unique_ptr<ContinueStatement> continueStatement();
    
    /**
     * @brief Parse pass statement
     * @return PassStatement AST node
     */
    std::unique_ptr<PassStatement> passStatement();
    
    /**
     * @brief Parse block statement
     * @return BlockStatement AST node
     */
    std::unique_ptr<BlockStatement> blockStatement();
    
    /**
     * @brief Parse expression statement
     * @return ExpressionStatement AST node
     */
    std::unique_ptr<ExpressionStatement> expressionStatement();
    
    // Expression parsing (in precedence order)
    
    /**
     * @brief Parse expression
     * @return Expression AST node
     */
    std::unique_ptr<Expression> expression();
    
    /**
     * @brief Parse assignment expression
     * @return Expression AST node
     */
    std::unique_ptr<Expression> assignment();
    
    /**
     * @brief Parse logical or expression
     * @return Expression AST node
     */
    std::unique_ptr<Expression> logicalOr();
    
    /**
     * @brief Parse logical and expression
     * @return Expression AST node
     */
    std::unique_ptr<Expression> logicalAnd();
    
    /**
     * @brief Parse equality expression
     * @return Expression AST node
     */
    std::unique_ptr<Expression> equality();
    
    /**
     * @brief Parse comparison expression
     * @return Expression AST node
     */
    std::unique_ptr<Expression> comparison();
    
    /**
     * @brief Parse term expression (+ -)
     * @return Expression AST node
     */
    std::unique_ptr<Expression> term();
    
    /**
     * @brief Parse factor expression (* /)
     * @return Expression AST node
     */
    std::unique_ptr<Expression> factor();
    
    /**
     * @brief Parse power expression (**)
     * @return Expression AST node
     */
    std::unique_ptr<Expression> power();
    
    /**
     * @brief Parse unary expression
     * @return Expression AST node
     */
    std::unique_ptr<Expression> unary();
    
    /**
     * @brief Parse call expression
     * @return Expression AST node
     */
    std::unique_ptr<Expression> call();
    
    /**
     * @brief Parse primary expression
     * @return Expression AST node
     */
    std::unique_ptr<Expression> primary();
    
    /**
     * @brief Parse list literal [item1, item2, ...]
     * @return ListExpression AST node
     */
    std::unique_ptr<ListExpression> listLiteral();
    
    /**
     * @brief Parse dictionary literal {key1: value1, key2: value2, ...}
     * @return DictExpression AST node
     */
    std::unique_ptr<DictExpression> dictLiteral();
    
    /**
     * @brief Finish parsing function call
     * @param callee Function being called
     * @return CallExpression AST node
     */
    std::unique_ptr<Expression> finishCall(std::unique_ptr<Expression> callee);
};

} // namespace caesar

#endif // CAESAR_PARSER_H
