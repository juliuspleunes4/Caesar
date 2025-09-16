/**
 * @file ast.h
 * @brief Abstract Syntax Tree definitions for Caesar
 * @author J.J.G. Pleunes
 * @version 1.0.0
 */

#ifndef CAESAR_AST_H
#define CAESAR_AST_H

#include "caesar/token.h"
#include <memory>
#include <vector>
#include <string>

namespace caesar {

// Forward declarations
class ASTVisitor;
class ASTNode;
class Expression;
class Statement;

/**
 * @brief Base class for all AST nodes
 */
class ASTNode {
public:
    Position position;  ///< Position in source code
    
    ASTNode(const Position& pos = Position()) : position(pos) {}
    virtual ~ASTNode() = default;
    virtual void accept(ASTVisitor& visitor) = 0;
    virtual std::string toString() const = 0;
};

/**
 * @brief Base class for expressions
 */
class Expression : public ASTNode {
public:
    Expression(const Position& pos = Position()) : ASTNode(pos) {}
    virtual ~Expression() = default;
};

/**
 * @brief Base class for statements
 */
class Statement : public ASTNode {
public:
    Statement(const Position& pos = Position()) : ASTNode(pos) {}
    virtual ~Statement() = default;
};

/**
 * @brief Visitor pattern for AST traversal
 */
class ASTVisitor {
public:
    virtual ~ASTVisitor() = default;
    
    // Expression visitors
    virtual void visit(class LiteralExpression& node) = 0;
    virtual void visit(class IdentifierExpression& node) = 0;
    virtual void visit(class BinaryExpression& node) = 0;
    virtual void visit(class UnaryExpression& node) = 0;
    virtual void visit(class CallExpression& node) = 0;
    virtual void visit(class MemberExpression& node) = 0;
    virtual void visit(class AssignmentExpression& node) = 0;
    virtual void visit(class ListExpression& node) = 0;
    virtual void visit(class DictExpression& node) = 0;
    
    // Statement visitors
    virtual void visit(class ExpressionStatement& node) = 0;
    virtual void visit(class BlockStatement& node) = 0;
    virtual void visit(class IfStatement& node) = 0;
    virtual void visit(class WhileStatement& node) = 0;
    virtual void visit(class ForStatement& node) = 0;
    virtual void visit(class FunctionDefinition& node) = 0;
    virtual void visit(class ClassDefinition& node) = 0;
    virtual void visit(class ReturnStatement& node) = 0;
    virtual void visit(class BreakStatement& node) = 0;
    virtual void visit(class ContinueStatement& node) = 0;
    virtual void visit(class PassStatement& node) = 0;
    virtual void visit(class Program& node) = 0;
};

// Expression classes

/**
 * @brief Literal expression (numbers, strings, booleans)
 */
class LiteralExpression : public Expression {
public:
    Token value;
    
    explicit LiteralExpression(const Token& val) : value(val) {}
    
    void accept(ASTVisitor& visitor) override;
    std::string toString() const override;
};

/**
 * @brief Identifier expression (variable/function names)
 */
class IdentifierExpression : public Expression {
public:
    std::string name;
    Position position;
    
    IdentifierExpression(const std::string& n, const Position& pos) : name(n), position(pos) {}
    
    void accept(ASTVisitor& visitor) override;
    std::string toString() const override;
};

/**
 * @brief Binary expression (a + b, a == b, etc.)
 */
class BinaryExpression : public Expression {
public:
    std::unique_ptr<Expression> left;
    TokenType operator_type;
    std::unique_ptr<Expression> right;
    
    BinaryExpression(std::unique_ptr<Expression> l, TokenType op, std::unique_ptr<Expression> r, const Position& pos = Position())
        : Expression(pos), left(std::move(l)), operator_type(op), right(std::move(r)) {}
    
    void accept(ASTVisitor& visitor) override;
    std::string toString() const override;
};

/**
 * @brief Unary expression (-a, not b)
 */
class UnaryExpression : public Expression {
public:
    TokenType operator_type;
    std::unique_ptr<Expression> operand;
    
    UnaryExpression(TokenType op, std::unique_ptr<Expression> expr, const Position& pos = Position())
        : Expression(pos), operator_type(op), operand(std::move(expr)) {}
    
    void accept(ASTVisitor& visitor) override;
    std::string toString() const override;
};

/**
 * @brief Function call expression
 */
class CallExpression : public Expression {
public:
    std::unique_ptr<Expression> function;
    std::vector<std::unique_ptr<Expression>> arguments;
    
    CallExpression(std::unique_ptr<Expression> func, std::vector<std::unique_ptr<Expression>> args, const Position& pos = Position())
        : Expression(pos), function(std::move(func)), arguments(std::move(args)) {}
    
    void accept(ASTVisitor& visitor) override;
    std::string toString() const override;
};

/**
 * @brief Member access expression (obj.member)
 */
class MemberExpression : public Expression {
public:
    std::unique_ptr<Expression> object;
    std::string member;
    
    MemberExpression(std::unique_ptr<Expression> obj, const std::string& member_name, const Position& pos = Position())
        : Expression(pos), object(std::move(obj)), member(member_name) {}
    
    void accept(ASTVisitor& visitor) override;
    std::string toString() const override;
};

/**
 * @brief Assignment expression
 */
class AssignmentExpression : public Expression {
public:
    std::unique_ptr<Expression> target;
    std::unique_ptr<Expression> value;
    TokenType operator_type; // ASSIGN, PLUS_ASSIGN, etc.
    
    AssignmentExpression(std::unique_ptr<Expression> tgt, std::unique_ptr<Expression> val, TokenType op = TokenType::ASSIGN, const Position& pos = Position())
        : Expression(pos), target(std::move(tgt)), value(std::move(val)), operator_type(op) {}
    
    void accept(ASTVisitor& visitor) override;
    std::string toString() const override;
};

/**
 * @brief List expression [item1, item2, ...]
 */
class ListExpression : public Expression {
public:
    std::vector<std::unique_ptr<Expression>> elements;
    
    ListExpression(std::vector<std::unique_ptr<Expression>> elems, const Position& pos = Position())
        : Expression(pos), elements(std::move(elems)) {}
    
    void accept(ASTVisitor& visitor) override;
    std::string toString() const override;
};

/**
 * @brief Dictionary expression {key1: value1, key2: value2, ...}
 */
class DictExpression : public Expression {
public:
    std::vector<std::pair<std::unique_ptr<Expression>, std::unique_ptr<Expression>>> pairs;
    
    DictExpression(std::vector<std::pair<std::unique_ptr<Expression>, std::unique_ptr<Expression>>> p, const Position& pos = Position())
        : Expression(pos), pairs(std::move(p)) {}
    
    void accept(ASTVisitor& visitor) override;
    std::string toString() const override;
};

// Statement classes

/**
 * @brief Expression statement (expression used as statement)
 */
class ExpressionStatement : public Statement {
public:
    std::unique_ptr<Expression> expression;
    
    ExpressionStatement(std::unique_ptr<Expression> expr, const Position& pos = Position())
        : Statement(pos), expression(std::move(expr)) {}
    
    void accept(ASTVisitor& visitor) override;
    std::string toString() const override;
};

/**
 * @brief Block statement (list of statements)
 */
class BlockStatement : public Statement {
public:
    std::vector<std::unique_ptr<Statement>> statements;
    
    BlockStatement(std::vector<std::unique_ptr<Statement>> stmts, const Position& pos = Position())
        : Statement(pos), statements(std::move(stmts)) {}
    
    void accept(ASTVisitor& visitor) override;
    std::string toString() const override;
};

/**
 * @brief If statement
 */
class IfStatement : public Statement {
public:
    std::unique_ptr<Expression> condition;
    std::unique_ptr<Statement> then_block;
    std::unique_ptr<Statement> else_block; // nullable
    
    IfStatement(std::unique_ptr<Expression> cond, std::unique_ptr<Statement> then_stmt, 
                std::unique_ptr<Statement> else_stmt, const Position& pos = Position())
        : Statement(pos), condition(std::move(cond)), then_block(std::move(then_stmt)), else_block(std::move(else_stmt)) {}
    
    void accept(ASTVisitor& visitor) override;
    std::string toString() const override;
};

/**
 * @brief While statement
 */
class WhileStatement : public Statement {
public:
    std::unique_ptr<Expression> condition;
    std::unique_ptr<Statement> body;
    
    WhileStatement(std::unique_ptr<Expression> cond, std::unique_ptr<Statement> body_stmt, const Position& pos = Position())
        : Statement(pos), condition(std::move(cond)), body(std::move(body_stmt)) {}
    
    void accept(ASTVisitor& visitor) override;
    std::string toString() const override;
};

/**
 * @brief For statement
 */
class ForStatement : public Statement {
public:
    std::string variable;
    std::unique_ptr<Expression> iterable;
    std::unique_ptr<Statement> body;
    
    ForStatement(const std::string& var, std::unique_ptr<Expression> iter, std::unique_ptr<Statement> body_stmt, const Position& pos = Position())
        : Statement(pos), variable(var), iterable(std::move(iter)), body(std::move(body_stmt)) {}
    
    void accept(ASTVisitor& visitor) override;
    std::string toString() const override;
};

/**
 * @brief Function parameter with optional default value
 */
struct Parameter {
    std::string name;
    std::unique_ptr<Expression> default_value; // nullptr if no default
    
    Parameter(const std::string& param_name, std::unique_ptr<Expression> default_val = nullptr)
        : name(param_name), default_value(std::move(default_val)) {}
    
    // Move constructor
    Parameter(Parameter&& other) noexcept 
        : name(std::move(other.name)), default_value(std::move(other.default_value)) {}
    
    // Move assignment
    Parameter& operator=(Parameter&& other) noexcept {
        name = std::move(other.name);
        default_value = std::move(other.default_value);
        return *this;
    }
    
    // Delete copy operations since we have unique_ptr
    Parameter(const Parameter&) = delete;
    Parameter& operator=(const Parameter&) = delete;
};

/**
 * @brief Function definition
 */
class FunctionDefinition : public Statement {
public:
    std::string name;
    std::vector<Parameter> parameters;
    std::unique_ptr<Statement> body;
    
    FunctionDefinition(const std::string& func_name, std::vector<Parameter> params, std::unique_ptr<Statement> body_stmt, const Position& pos = Position())
        : Statement(pos), name(func_name), parameters(std::move(params)), body(std::move(body_stmt)) {}
    
    void accept(ASTVisitor& visitor) override;
    std::string toString() const override;
};

/**
 * @brief Class definition
 */
class ClassDefinition : public Statement {
public:
    std::string name;
    std::vector<std::string> base_classes; // For inheritance (optional)
    std::unique_ptr<Statement> body;
    
    ClassDefinition(const std::string& class_name, std::vector<std::string> bases, std::unique_ptr<Statement> body_stmt, const Position& pos = Position())
        : Statement(pos), name(class_name), base_classes(std::move(bases)), body(std::move(body_stmt)) {}
    
    void accept(ASTVisitor& visitor) override;
    std::string toString() const override;
};

/**
 * @brief Return statement
 */
class ReturnStatement : public Statement {
public:
    std::unique_ptr<Expression> value; // nullable
    
    ReturnStatement(std::unique_ptr<Expression> val, const Position& pos = Position())
        : Statement(pos), value(std::move(val)) {}
    
    void accept(ASTVisitor& visitor) override;
    std::string toString() const override;
};

/**
 * @brief Break statement
 */
class BreakStatement : public Statement {
public:
    BreakStatement(const Position& pos = Position()) : Statement(pos) {}
    
    void accept(ASTVisitor& visitor) override;
    std::string toString() const override;
};

/**
 * @brief Continue statement
 */
class ContinueStatement : public Statement {
public:
    ContinueStatement(const Position& pos = Position()) : Statement(pos) {}
    
    void accept(ASTVisitor& visitor) override;
    std::string toString() const override;
};

/**
 * @brief Pass statement (placeholder statement)
 */
class PassStatement : public Statement {
public:
    PassStatement(const Position& pos = Position()) : Statement(pos) {}
    
    void accept(ASTVisitor& visitor) override;
    std::string toString() const override;
};

/**
 * @brief Program (top-level container)
 */
class Program : public ASTNode {
public:
    std::vector<std::unique_ptr<Statement>> statements;
    
    explicit Program(std::vector<std::unique_ptr<Statement>> stmts)
        : statements(std::move(stmts)) {}
    
    void accept(ASTVisitor& visitor) override;
    std::string toString() const override;
};

} // namespace caesar

#endif // CAESAR_AST_H
