/**
 * @file ast.cpp
 * @brief Implementation of AST nodes
 * @author J.J.G. Pleunes
 * @version 1.0.0
 */

#include "caesar/ast.h"
#include <sstream>

namespace caesar {

// LiteralExpression
void LiteralExpression::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string LiteralExpression::toString() const {
    return "Literal(" + value.value + ")";
}

// IdentifierExpression
void IdentifierExpression::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string IdentifierExpression::toString() const {
    return "Identifier(" + name + ")";
}

// BinaryExpression
void BinaryExpression::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string BinaryExpression::toString() const {
    std::string op_str;
    switch (operator_type) {
        case TokenType::PLUS: op_str = "+"; break;
        case TokenType::MINUS: op_str = "-"; break;
        case TokenType::MULTIPLY: op_str = "*"; break;
        case TokenType::DIVIDE: op_str = "/"; break;
        case TokenType::EQUAL: op_str = "=="; break;
        case TokenType::NOT_EQUAL: op_str = "!="; break;
        case TokenType::LESS: op_str = "<"; break;
        case TokenType::LESS_EQUAL: op_str = "<="; break;
        case TokenType::GREATER: op_str = ">"; break;
        case TokenType::GREATER_EQUAL: op_str = ">="; break;
        case TokenType::AND: op_str = "and"; break;
        case TokenType::OR: op_str = "or"; break;
        default: op_str = "?"; break;
    }
    return "Binary(" + left->toString() + " " + op_str + " " + right->toString() + ")";
}

// UnaryExpression
void UnaryExpression::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string UnaryExpression::toString() const {
    std::string op_str;
    switch (operator_type) {
        case TokenType::MINUS: op_str = "-"; break;
        case TokenType::NOT: op_str = "not"; break;
        default: op_str = "?"; break;
    }
    return "Unary(" + op_str + " " + operand->toString() + ")";
}

// CallExpression
void CallExpression::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string CallExpression::toString() const {
    std::ostringstream oss;
    oss << "Call(" << function->toString() << "(";
    for (size_t i = 0; i < arguments.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << arguments[i]->toString();
    }
    oss << "))";
    return oss.str();
}

// MemberExpression
void MemberExpression::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string MemberExpression::toString() const {
    return "Member(" + object->toString() + "." + member + ")";
}

// AssignmentExpression
void AssignmentExpression::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string AssignmentExpression::toString() const {
    std::string op_str;
    switch (operator_type) {
        case TokenType::ASSIGN: op_str = " = "; break;
        case TokenType::PLUS_ASSIGN: op_str = " += "; break;
        case TokenType::MINUS_ASSIGN: op_str = " -= "; break;
        case TokenType::MULT_ASSIGN: op_str = " *= "; break;
        case TokenType::DIV_ASSIGN: op_str = " /= "; break;
        default: op_str = " = "; break;
    }
    return "Assignment(" + target->toString() + op_str + value->toString() + ")";
}

// ListExpression
void ListExpression::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string ListExpression::toString() const {
    std::ostringstream oss;
    oss << "List([";
    for (size_t i = 0; i < elements.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << elements[i]->toString();
    }
    oss << "])";
    return oss.str();
}

// DictExpression
void DictExpression::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string DictExpression::toString() const {
    std::ostringstream oss;
    oss << "Dict({";
    for (size_t i = 0; i < pairs.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << pairs[i].first->toString() << ": " << pairs[i].second->toString();
    }
    oss << "})";
    return oss.str();
}

// ExpressionStatement
void ExpressionStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string ExpressionStatement::toString() const {
    return "ExprStmt(" + expression->toString() + ")";
}

// BlockStatement
void BlockStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string BlockStatement::toString() const {
    std::ostringstream oss;
    oss << "Block(\n";
    for (const auto& stmt : statements) {
        oss << "  " << stmt->toString() << "\n";
    }
    oss << ")";
    return oss.str();
}

// IfStatement
void IfStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string IfStatement::toString() const {
    std::string result = "If(" + condition->toString() + " then " + then_block->toString();
    if (else_block) {
        result += " else " + else_block->toString();
    }
    result += ")";
    return result;
}

// WhileStatement
void WhileStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string WhileStatement::toString() const {
    return "While(" + condition->toString() + " " + body->toString() + ")";
}

// ForStatement
void ForStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string ForStatement::toString() const {
    return "For(" + variable + " in " + iterable->toString() + " " + body->toString() + ")";
}

// FunctionDefinition
void FunctionDefinition::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string FunctionDefinition::toString() const {
    std::ostringstream oss;
    oss << "Function(" << name << "(";
    for (size_t i = 0; i < parameters.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << parameters[i].name;
        if (parameters[i].default_value) {
            oss << "=" << parameters[i].default_value->toString();
        }
    }
    oss << ") " << body->toString() << ")";
    return oss.str();
}

// ClassDefinition
void ClassDefinition::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string ClassDefinition::toString() const {
    std::ostringstream oss;
    oss << "Class(" << name;
    if (!base_classes.empty()) {
        oss << "(";
        for (size_t i = 0; i < base_classes.size(); ++i) {
            if (i > 0) oss << ", ";
            oss << base_classes[i];
        }
        oss << ")";
    }
    oss << " " << body->toString() << ")";
    return oss.str();
}

// ReturnStatement
void ReturnStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string ReturnStatement::toString() const {
    if (value) {
        return "Return(" + value->toString() + ")";
    } else {
        return "Return()";
    }
}

// BreakStatement
void BreakStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string BreakStatement::toString() const {
    return "Break()";
}

// ContinueStatement
void ContinueStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string ContinueStatement::toString() const {
    return "Continue()";
}

// PassStatement
void PassStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string PassStatement::toString() const {
    return "Pass()";
}

// Program
void Program::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string Program::toString() const {
    std::ostringstream oss;
    oss << "Program(\n";
    for (const auto& stmt : statements) {
        oss << "  " << stmt->toString() << "\n";
    }
    oss << ")";
    return oss.str();
}

} // namespace caesar
