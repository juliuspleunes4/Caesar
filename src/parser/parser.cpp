/**
 * @file parser.cpp
 * @brief Implementation of the Caesar parser
 * @author Julius Pleunes
 * @version 1.0.0
 */

#include "caesar/parser.h"
#include <algorithm>

namespace caesar {

Parser::Parser(std::vector<Token> tokens) : tokens_(std::move(tokens)), current_(0) {}

std::unique_ptr<Program> Parser::parse() {
    return program();
}

bool Parser::isAtEnd() const {
    return current_ >= tokens_.size() || peek().type == TokenType::EOF_TOKEN;
}

const Token& Parser::peek() const {
    if (current_ >= tokens_.size()) {
        static Token eof_token(TokenType::EOF_TOKEN, "", Position());
        return eof_token;
    }
    return tokens_[current_];
}

const Token& Parser::previous() const {
    if (current_ == 0) {
        static Token dummy(TokenType::UNKNOWN, "", Position());
        return dummy;
    }
    return tokens_[current_ - 1];
}

Token Parser::advance() {
    if (!isAtEnd()) current_++;
    return previous();
}

bool Parser::check(TokenType type) const {
    if (isAtEnd()) return false;
    return peek().type == type;
}

bool Parser::match(const std::vector<TokenType>& types) {
    for (TokenType type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

Token Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) return advance();
    error(message);
    return Token(TokenType::UNKNOWN, "", Position());
}

void Parser::skipNewlines() {
    while (match({TokenType::NEWLINE})) {
        // Skip newlines
    }
}

void Parser::error(const std::string& message) const {
    const Token& token = peek();
    throw ParserException(message + " at line " + std::to_string(token.position.line) + 
                         ", column " + std::to_string(token.position.column) + 
                         " (got '" + token.value + "')");
}

std::unique_ptr<Program> Parser::program() {
    std::vector<std::unique_ptr<Statement>> statements;
    
    skipNewlines();
    
    while (!isAtEnd()) {
        auto stmt = statement();
        if (stmt) {
            statements.push_back(std::move(stmt));
        }
        skipNewlines();
    }
    
    return std::make_unique<Program>(std::move(statements));
}

std::unique_ptr<Statement> Parser::statement() {
    if (match({TokenType::DEF})) {
        return functionDefinition();
    }
    
    if (match({TokenType::IF})) {
        return ifStatement();
    }
    
    if (match({TokenType::WHILE})) {
        return whileStatement();
    }
    
    if (match({TokenType::FOR})) {
        return forStatement();
    }
    
    if (match({TokenType::RETURN})) {
        return returnStatement();
    }
    
    return expressionStatement();
}

std::unique_ptr<FunctionDefinition> Parser::functionDefinition() {
    Token name_token = consume(TokenType::IDENTIFIER, "Expected function name");
    std::string name = name_token.value;
    
    consume(TokenType::LPAREN, "Expected '(' after function name");
    
    std::vector<std::string> parameters;
    if (!check(TokenType::RPAREN)) {
        do {
            Token param = consume(TokenType::IDENTIFIER, "Expected parameter name");
            parameters.push_back(param.value);
        } while (match({TokenType::COMMA}));
    }
    
    consume(TokenType::RPAREN, "Expected ')' after parameters");
    consume(TokenType::COLON, "Expected ':' after function signature");
    consume(TokenType::NEWLINE, "Expected newline after ':'");
    
    auto body = blockStatement();
    
    return std::make_unique<FunctionDefinition>(name, std::move(parameters), 
                                               std::move(body), name_token.position);
}

std::unique_ptr<IfStatement> Parser::ifStatement() {
    auto condition = expression();
    consume(TokenType::COLON, "Expected ':' after if condition");
    consume(TokenType::NEWLINE, "Expected newline after ':'");
    
    auto then_block = blockStatement();
    std::unique_ptr<Statement> else_block = nullptr;
    
    if (match({TokenType::ELSE})) {
        consume(TokenType::COLON, "Expected ':' after else");
        consume(TokenType::NEWLINE, "Expected newline after ':'");
        else_block = blockStatement();
    }
    
    return std::make_unique<IfStatement>(std::move(condition), std::move(then_block),
                                        std::move(else_block), previous().position);
}

std::unique_ptr<WhileStatement> Parser::whileStatement() {
    auto condition = expression();
    consume(TokenType::COLON, "Expected ':' after while condition");
    consume(TokenType::NEWLINE, "Expected newline after ':'");
    
    auto body = blockStatement();
    
    return std::make_unique<WhileStatement>(std::move(condition), std::move(body),
                                          previous().position);
}

std::unique_ptr<ForStatement> Parser::forStatement() {
    Token var_token = consume(TokenType::IDENTIFIER, "Expected variable name in for loop");
    consume(TokenType::IN, "Expected 'in' after for variable");
    
    auto iterable = expression();
    consume(TokenType::COLON, "Expected ':' after for expression");
    consume(TokenType::NEWLINE, "Expected newline after ':'");
    
    auto body = blockStatement();
    
    return std::make_unique<ForStatement>(var_token.value, std::move(iterable),
                                        std::move(body), var_token.position);
}

std::unique_ptr<ReturnStatement> Parser::returnStatement() {
    Position pos = previous().position;
    std::unique_ptr<Expression> value = nullptr;
    
    if (!check(TokenType::NEWLINE) && !isAtEnd()) {
        value = expression();
    }
    
    return std::make_unique<ReturnStatement>(std::move(value), pos);
}

std::unique_ptr<BlockStatement> Parser::blockStatement() {
    consume(TokenType::INDENT, "Expected indented block");
    
    std::vector<std::unique_ptr<Statement>> statements;
    
    while (!check(TokenType::DEDENT) && !isAtEnd()) {
        auto stmt = statement();
        if (stmt) {
            statements.push_back(std::move(stmt));
        }
        skipNewlines();
    }
    
    if (check(TokenType::DEDENT)) {
        consume(TokenType::DEDENT, "Expected dedent to close block");
    }
    
    return std::make_unique<BlockStatement>(std::move(statements), Position());
}

std::unique_ptr<ExpressionStatement> Parser::expressionStatement() {
    auto expr = expression();
    Position pos = expr ? expr->position : Position();
    return std::make_unique<ExpressionStatement>(std::move(expr), pos);
}

std::unique_ptr<Expression> Parser::expression() {
    return assignment();
}

std::unique_ptr<Expression> Parser::assignment() {
    auto expr = logicalOr();
    
    if (match({TokenType::ASSIGN})) {
        auto value = assignment();
        return std::make_unique<AssignmentExpression>(std::move(expr), std::move(value),
                                                     previous().position);
    }
    
    return expr;
}

std::unique_ptr<Expression> Parser::logicalOr() {
    auto expr = logicalAnd();
    
    while (match({TokenType::OR})) {
        TokenType operator_type = previous().type;
        auto right = logicalAnd();
        expr = std::make_unique<BinaryExpression>(std::move(expr), operator_type,
                                                 std::move(right), previous().position);
    }
    
    return expr;
}

std::unique_ptr<Expression> Parser::logicalAnd() {
    auto expr = equality();
    
    while (match({TokenType::AND})) {
        TokenType operator_type = previous().type;
        auto right = equality();
        expr = std::make_unique<BinaryExpression>(std::move(expr), operator_type,
                                                 std::move(right), previous().position);
    }
    
    return expr;
}

std::unique_ptr<Expression> Parser::equality() {
    auto expr = comparison();
    
    while (match({TokenType::NOT_EQUAL, TokenType::EQUAL})) {
        TokenType operator_type = previous().type;
        auto right = comparison();
        expr = std::make_unique<BinaryExpression>(std::move(expr), operator_type,
                                                 std::move(right), previous().position);
    }
    
    return expr;
}

std::unique_ptr<Expression> Parser::comparison() {
    auto expr = term();
    
    while (match({TokenType::GREATER, TokenType::GREATER_EQUAL,
                  TokenType::LESS, TokenType::LESS_EQUAL})) {
        TokenType operator_type = previous().type;
        auto right = term();
        expr = std::make_unique<BinaryExpression>(std::move(expr), operator_type,
                                                 std::move(right), previous().position);
    }
    
    return expr;
}

std::unique_ptr<Expression> Parser::term() {
    auto expr = factor();
    
    while (match({TokenType::MINUS, TokenType::PLUS})) {
        TokenType operator_type = previous().type;
        auto right = factor();
        expr = std::make_unique<BinaryExpression>(std::move(expr), operator_type,
                                                 std::move(right), previous().position);
    }
    
    return expr;
}

std::unique_ptr<Expression> Parser::factor() {
    auto expr = power();
    
    while (match({TokenType::DIVIDE, TokenType::MULTIPLY, TokenType::MODULO,
                  TokenType::FLOOR_DIVIDE})) {
        TokenType operator_type = previous().type;
        auto right = power();
        expr = std::make_unique<BinaryExpression>(std::move(expr), operator_type,
                                                 std::move(right), previous().position);
    }
    
    return expr;
}

std::unique_ptr<Expression> Parser::power() {
    auto expr = unary();
    
    if (match({TokenType::POWER})) {
        TokenType operator_type = previous().type;
        auto right = power(); // Right associative
        expr = std::make_unique<BinaryExpression>(std::move(expr), operator_type,
                                                 std::move(right), previous().position);
    }
    
    return expr;
}

std::unique_ptr<Expression> Parser::unary() {
    if (match({TokenType::NOT, TokenType::MINUS})) {
        TokenType operator_type = previous().type;
        auto right = unary();
        return std::make_unique<UnaryExpression>(operator_type, std::move(right),
                                               previous().position);
    }
    
    return call();
}

std::unique_ptr<Expression> Parser::call() {
    auto expr = primary();
    
    while (true) {
        if (match({TokenType::LPAREN})) {
            expr = finishCall(std::move(expr));
        } else {
            break;
        }
    }
    
    return expr;
}

std::unique_ptr<Expression> Parser::primary() {
    if (match({TokenType::BOOLEAN, TokenType::INTEGER, TokenType::FLOAT, TokenType::STRING})) {
        return std::make_unique<LiteralExpression>(previous());
    }
    
    if (match({TokenType::IDENTIFIER})) {
        return std::make_unique<IdentifierExpression>(previous().value, previous().position);
    }
    
    if (match({TokenType::LPAREN})) {
        auto expr = expression();
        consume(TokenType::RPAREN, "Expected ')' after expression");
        return expr;
    }
    
    error("Expected expression");
    return nullptr;
}

std::unique_ptr<Expression> Parser::finishCall(std::unique_ptr<Expression> callee) {
    std::vector<std::unique_ptr<Expression>> arguments;
    
    if (!check(TokenType::RPAREN)) {
        do {
            arguments.push_back(expression());
        } while (match({TokenType::COMMA}));
    }
    
    Token paren = consume(TokenType::RPAREN, "Expected ')' after arguments");
    return std::make_unique<CallExpression>(std::move(callee), std::move(arguments),
                                          paren.position);
}

} // namespace caesar