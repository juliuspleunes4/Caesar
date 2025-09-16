/**
 * @file token.cpp
 * @brief Implementation of token functionality
 * @author J.J.G. Pleunes
 * @version 1.0.0
 */

#include "caesar/token.h"
#include <unordered_set>

namespace caesar {

std::string Token::typeToString() const {
    switch (type) {
        case TokenType::INTEGER: return "INTEGER";
        case TokenType::FLOAT: return "FLOAT";
        case TokenType::STRING: return "STRING";
        case TokenType::BOOLEAN: return "BOOLEAN";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::IF: return "IF";
        case TokenType::ELIF: return "ELIF";
        case TokenType::ELSE: return "ELSE";
        case TokenType::WHILE: return "WHILE";
        case TokenType::FOR: return "FOR";
        case TokenType::IN: return "IN";
        case TokenType::DEF: return "DEF";
        case TokenType::CLASS: return "CLASS";
        case TokenType::RETURN: return "RETURN";
        case TokenType::BREAK: return "BREAK";
        case TokenType::CONTINUE: return "CONTINUE";
        case TokenType::PASS: return "PASS";
        case TokenType::AND: return "AND";
        case TokenType::OR: return "OR";
        case TokenType::NOT: return "NOT";
        case TokenType::IS: return "IS";
        case TokenType::NONE: return "NONE";
        case TokenType::PLUS: return "PLUS";
        case TokenType::MINUS: return "MINUS";
        case TokenType::MULTIPLY: return "MULTIPLY";
        case TokenType::DIVIDE: return "DIVIDE";
        case TokenType::FLOOR_DIVIDE: return "FLOOR_DIVIDE";
        case TokenType::MODULO: return "MODULO";
        case TokenType::POWER: return "POWER";
        case TokenType::ASSIGN: return "ASSIGN";
        case TokenType::PLUS_ASSIGN: return "PLUS_ASSIGN";
        case TokenType::MINUS_ASSIGN: return "MINUS_ASSIGN";
        case TokenType::MULT_ASSIGN: return "MULT_ASSIGN";
        case TokenType::DIV_ASSIGN: return "DIV_ASSIGN";
        case TokenType::EQUAL: return "EQUAL";
        case TokenType::NOT_EQUAL: return "NOT_EQUAL";
        case TokenType::LESS: return "LESS";
        case TokenType::LESS_EQUAL: return "LESS_EQUAL";
        case TokenType::GREATER: return "GREATER";
        case TokenType::GREATER_EQUAL: return "GREATER_EQUAL";
        case TokenType::LPAREN: return "LPAREN";
        case TokenType::RPAREN: return "RPAREN";
        case TokenType::LBRACKET: return "LBRACKET";
        case TokenType::RBRACKET: return "RBRACKET";
        case TokenType::LBRACE: return "LBRACE";
        case TokenType::RBRACE: return "RBRACE";
        case TokenType::COMMA: return "COMMA";
        case TokenType::COLON: return "COLON";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::DOT: return "DOT";
        case TokenType::NEWLINE: return "NEWLINE";
        case TokenType::INDENT: return "INDENT";
        case TokenType::DEDENT: return "DEDENT";
        case TokenType::EOF_TOKEN: return "EOF";
        case TokenType::UNKNOWN: return "UNKNOWN";
        default: return "UNKNOWN";
    }
}

bool Token::isKeyword() const {
    static const std::unordered_set<TokenType> keywords = {
        TokenType::IF, TokenType::ELIF, TokenType::ELSE, TokenType::WHILE,
        TokenType::FOR, TokenType::IN, TokenType::DEF, TokenType::CLASS,
        TokenType::RETURN, TokenType::BREAK, TokenType::CONTINUE, TokenType::PASS,
        TokenType::AND, TokenType::OR, TokenType::NOT, TokenType::IS,
        TokenType::NONE, TokenType::BOOLEAN
    };
    return keywords.find(type) != keywords.end();
}

bool Token::isOperator() const {
    static const std::unordered_set<TokenType> operators = {
        TokenType::PLUS, TokenType::MINUS, TokenType::MULTIPLY, TokenType::DIVIDE,
        TokenType::FLOOR_DIVIDE, TokenType::MODULO, TokenType::POWER,
        TokenType::ASSIGN, TokenType::PLUS_ASSIGN, TokenType::MINUS_ASSIGN,
        TokenType::MULT_ASSIGN, TokenType::DIV_ASSIGN,
        TokenType::EQUAL, TokenType::NOT_EQUAL, TokenType::LESS,
        TokenType::LESS_EQUAL, TokenType::GREATER, TokenType::GREATER_EQUAL
    };
    return operators.find(type) != operators.end();
}

std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << token.typeToString() << "('" << token.value << "') at " 
       << token.position.line << ":" << token.position.column;
    return os;
}

} // namespace caesar
