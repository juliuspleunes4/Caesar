/**
 * @file interpreter.h
 * @brief Caesar AST Interpreter for direct execution
 * @author J.J.G. Pleunes
 * @version 1.0.0
 */

#ifndef CAESAR_INTERPRETER_H
#define CAESAR_INTERPRETER_H

#include "caesar/ast.h"
#include <variant>
#include <functional>
#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <exception>

namespace caesar {

// Forward declarations
class Interpreter;
class Environment;

/**
 * @brief Value type for runtime values (simplified for now)
 */
using Value = std::variant<
    std::nullptr_t,              // None/null
    bool,                        // Boolean
    int64_t,                     // Integer
    double,                      // Float
    std::string                  // String (and other objects as strings for now)
>;

/**
 * @brief Runtime error class
 */
class RuntimeError : public std::exception {
public:
    std::string message;
    
    RuntimeError(const std::string& msg) : message(msg) {}
    
    const char* what() const noexcept override {
        return message.c_str();
    }
};

/**
 * @brief Control flow exceptions for break/continue/return
 */
class BreakException : public std::exception {
public:
    const char* what() const noexcept override { return "break"; }
};

class ContinueException : public std::exception {
public:
    const char* what() const noexcept override { return "continue"; }
};

class ReturnException : public std::exception {
public:
    Value value;
    
    ReturnException(const Value& val) : value(val) {}
    const char* what() const noexcept override { return "return"; }
};

/**
 * @brief Environment for variable scoping
 */
class Environment {
private:
    std::shared_ptr<Environment> parent;
    std::unordered_map<std::string, Value> variables;

public:
    Environment(std::shared_ptr<Environment> parent_env = nullptr)
        : parent(parent_env) {}

    void define(const std::string& name, const Value& value);
    Value get(const std::string& name);
    void assign(const std::string& name, const Value& value);
    bool exists(const std::string& name);
};

/**
 * @brief Callable function class (stub for now)
 */
class CallableFunction {
private:
    std::shared_ptr<FunctionDefinition> declaration;
    std::shared_ptr<Environment> closure;

public:
    CallableFunction(std::shared_ptr<FunctionDefinition> decl, std::shared_ptr<Environment> env)
        : declaration(decl), closure(env) {}

    Value call(Interpreter& interpreter, const std::vector<Value>& arguments);
};

/**
 * @brief Built-in function type
 */
using BuiltinFunction = std::function<Value(const std::vector<Value>&)>;

/**
 * @brief Main interpreter class
 */
class Interpreter : public ASTVisitor {
private:
    std::shared_ptr<Environment> environment;
    std::unordered_map<std::string, BuiltinFunction> builtins;
    
    Value last_value;

public:
    Interpreter();
    ~Interpreter() = default;

    /**
     * @brief Interpret a complete program
     */
    Value interpret(Program* program);

    /**
     * @brief Get current environment
     */
    std::shared_ptr<Environment> getCurrentEnvironment() const;

    /**
     * @brief Evaluate an expression
     */
    Value evaluate(Expression* expr);

    // Expression visitors (matching actual AST visitor interface)
    void visit(LiteralExpression& node) override;
    void visit(IdentifierExpression& node) override;
    void visit(BinaryExpression& node) override;
    void visit(UnaryExpression& node) override;
    void visit(CallExpression& node) override;
    void visit(MemberExpression& node) override;
    void visit(AssignmentExpression& node) override;
    void visit(ListExpression& node) override;
    void visit(DictExpression& node) override;

    // Statement visitors (matching actual AST visitor interface)
    void visit(ExpressionStatement& node) override;
    void visit(BlockStatement& node) override;
    void visit(IfStatement& node) override;
    void visit(WhileStatement& node) override;
    void visit(ForStatement& node) override;
    void visit(FunctionDefinition& node) override;
    void visit(ClassDefinition& node) override;
    void visit(ReturnStatement& node) override;
    void visit(BreakStatement& node) override;
    void visit(ContinueStatement& node) override;
    void visit(PassStatement& node) override;
    void visit(Program& node) override;

private:
    /**
     * @brief Initialize built-in functions
     */
    void initializeBuiltins();

    /**
     * @brief Convert value to string representation
     */
    std::string valueToString(const Value& value);

    /**
     * @brief Check if value is truthy
     */
    bool isTruthy(const Value& value);

    /**
     * @brief Check if two values are equal
     */
    bool isEqual(const Value& a, const Value& b);

    /**
     * @brief Convert token to value
     */
    Value tokenToValue(const Token& token);
};

} // namespace caesar

#endif // CAESAR_INTERPRETER_H