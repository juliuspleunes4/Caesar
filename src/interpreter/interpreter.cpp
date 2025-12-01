/**
 * @file interpreter.cpp
 * @brief Caesar AST Interpreter implementation
 * @author J.J.G. Pleunes
 * @version 1.0.0
 */

#include "caesar/interpreter.h"
#include "caesar/token.h"
#include <iostream>
#include <sstream>

namespace caesar {

// Environment implementation
void Environment::define(const std::string& name, const Value& value) {
    variables[name] = value;
}

Value Environment::get(const std::string& name) {
    auto it = variables.find(name);
    if (it != variables.end()) {
        return it->second;
    }
    
    if (parent) {
        return parent->get(name);
    }
    
    throw RuntimeError("Undefined variable '" + name + "'");
}

void Environment::assign(const std::string& name, const Value& value) {
    auto it = variables.find(name);
    if (it != variables.end()) {
        it->second = value;
        return;
    }
    
    if (parent) {
        parent->assign(name, value);
        return;
    }
    
    throw RuntimeError("Undefined variable '" + name + "'");
}

bool Environment::exists(const std::string& name) {
    return variables.find(name) != variables.end() || 
           (parent && parent->exists(name));
}

// CallableFunction implementation
Value CallableFunction::call(Interpreter& interpreter, const std::vector<Value>& arguments) {
    // Create new environment for function execution
    auto function_env = std::make_shared<Environment>(closure);
    
    // Bind parameters to arguments
    auto& params = declaration->parameters;
    for (size_t i = 0; i < params.size(); i++) {
        Value arg_value = nullptr;
        
        if (i < arguments.size()) {
            // Use provided argument
            arg_value = arguments[i];
        } else if (params[i].default_value) {
            // Use default value
            auto previous_env = interpreter.getCurrentEnvironment();
            // Set environment temporarily to evaluate default value in closure context
            interpreter.environment = closure;
            arg_value = interpreter.evaluate(params[i].default_value.get());
            interpreter.environment = previous_env;
        } else {
            throw RuntimeError("Missing argument for parameter '" + params[i].name + "'");
        }
        
        function_env->define(params[i].name, arg_value);
    }
    
    // Check for too many arguments
    if (arguments.size() > params.size()) {
        throw RuntimeError("Too many arguments: expected " + std::to_string(params.size()) + 
                          ", got " + std::to_string(arguments.size()));
    }
    
    // Execute function body in new environment
    auto previous_env = interpreter.getCurrentEnvironment();
    interpreter.environment = function_env;
    
    try {
        declaration->body->accept(interpreter);
        // Function completed without explicit return
        interpreter.environment = previous_env;
        return nullptr;
    } catch (const ReturnException& ret) {
        interpreter.environment = previous_env;
        return ret.value;
    } catch (...) {
        interpreter.environment = previous_env;
        throw;
    }
}

// Interpreter implementation
Interpreter::Interpreter() {
    environment = std::make_shared<Environment>();
    initializeBuiltins();
}

Value Interpreter::interpret(Program* program) {
    Value result = nullptr;
    
    try {
        program->accept(*this);
        result = last_value;
    } catch (const RuntimeError& e) {
        std::cerr << "Runtime Error: " << e.what() << std::endl;
        return nullptr;
    }
    
    return result;
}

std::shared_ptr<Environment> Interpreter::getCurrentEnvironment() const {
    return environment;
}

Value Interpreter::evaluate(Expression* expr) {
    expr->accept(*this);
    return last_value;
}

// Expression visitors
void Interpreter::visit(LiteralExpression& node) {
    last_value = tokenToValue(node.value);
}

void Interpreter::visit(IdentifierExpression& node) {
    // Check if it's a builtin function reference
    auto builtin_it = builtins.find(node.name);
    if (builtin_it != builtins.end()) {
        last_value = std::string("__builtin_" + node.name);
        return;
    }
    
    last_value = environment->get(node.name);
}

void Interpreter::visit(BinaryExpression& node) {
    Value left = evaluate(node.left.get());
    Value right = evaluate(node.right.get());
    
    // Handle arithmetic operations
    if (std::holds_alternative<int64_t>(left) && std::holds_alternative<int64_t>(right)) {
        int64_t l = std::get<int64_t>(left);
        int64_t r = std::get<int64_t>(right);
        
        switch (node.operator_type) {
            case TokenType::PLUS: last_value = l + r; return;
            case TokenType::MINUS: last_value = l - r; return;
            case TokenType::MULTIPLY: last_value = l * r; return;
            case TokenType::DIVIDE: 
                if (r == 0) throw RuntimeError("Division by zero");
                last_value = static_cast<double>(l) / static_cast<double>(r); 
                return;
            case TokenType::MODULO: 
                if (r == 0) throw RuntimeError("Modulo by zero");
                last_value = l % r; 
                return;
            case TokenType::EQUAL: last_value = l == r; return;
            case TokenType::NOT_EQUAL: last_value = l != r; return;
            case TokenType::LESS: last_value = l < r; return;
            case TokenType::LESS_EQUAL: last_value = l <= r; return;
            case TokenType::GREATER: last_value = l > r; return;
            case TokenType::GREATER_EQUAL: last_value = l >= r; return;
            default: break;
        }
    }
    
    // Handle floating-point operations
    if ((std::holds_alternative<double>(left) || std::holds_alternative<int64_t>(left)) &&
        (std::holds_alternative<double>(right) || std::holds_alternative<int64_t>(right))) {
        
        double l = std::holds_alternative<double>(left) ? std::get<double>(left) : static_cast<double>(std::get<int64_t>(left));
        double r = std::holds_alternative<double>(right) ? std::get<double>(right) : static_cast<double>(std::get<int64_t>(right));
        
        switch (node.operator_type) {
            case TokenType::PLUS: last_value = l + r; return;
            case TokenType::MINUS: last_value = l - r; return;
            case TokenType::MULTIPLY: last_value = l * r; return;
            case TokenType::DIVIDE: 
                if (r == 0.0) throw RuntimeError("Division by zero");
                last_value = l / r; 
                return;
            case TokenType::EQUAL: last_value = l == r; return;
            case TokenType::NOT_EQUAL: last_value = l != r; return;
            case TokenType::LESS: last_value = l < r; return;
            case TokenType::LESS_EQUAL: last_value = l <= r; return;
            case TokenType::GREATER: last_value = l > r; return;
            case TokenType::GREATER_EQUAL: last_value = l >= r; return;
            default: break;
        }
    }
    
    // Handle string concatenation and comparison
    if (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right)) {
        std::string l = std::get<std::string>(left);
        std::string r = std::get<std::string>(right);
        
        switch (node.operator_type) {
            case TokenType::PLUS: last_value = l + r; return;
            case TokenType::EQUAL: last_value = l == r; return;
            case TokenType::NOT_EQUAL: last_value = l != r; return;
            case TokenType::LESS: last_value = l < r; return;
            case TokenType::LESS_EQUAL: last_value = l <= r; return;
            case TokenType::GREATER: last_value = l > r; return;
            case TokenType::GREATER_EQUAL: last_value = l >= r; return;
            default: break;
        }
    }
    
    // Handle logical operations
    if (node.operator_type == TokenType::AND) {
        last_value = isTruthy(left) && isTruthy(right);
        return;
    }
    if (node.operator_type == TokenType::OR) {
        last_value = isTruthy(left) || isTruthy(right);
        return;
    }
    
    throw RuntimeError("Unsupported binary operation");
}

void Interpreter::visit(UnaryExpression& node) {
    Value operand = evaluate(node.operand.get());
    
    if (std::holds_alternative<int64_t>(operand)) {
        last_value = -std::get<int64_t>(operand);
    } else {
        last_value = operand;
    }
}

void Interpreter::visit(CallExpression& node) {
    Value callee = evaluate(node.function.get());
    
    std::vector<Value> arguments;
    for (auto& arg : node.arguments) {
        arguments.push_back(evaluate(arg.get()));
    }
    
    // Check if it's a user-defined function
    if (std::holds_alternative<std::shared_ptr<CallableFunction>>(callee)) {
        auto function = std::get<std::shared_ptr<CallableFunction>>(callee);
        last_value = function->call(*this, arguments);
        return;
    }
    
    // Check if it's a builtin function
    if (std::holds_alternative<std::string>(callee)) {
        std::string builtin_name = std::get<std::string>(callee);
        if (builtin_name.substr(0, 10) == "__builtin_") {
            std::string func_name = builtin_name.substr(10);
            auto it = builtins.find(func_name);
            if (it != builtins.end()) {
                last_value = it->second(arguments);
                return;
            }
        }
    }
    
    throw RuntimeError("Object is not callable");
}

void Interpreter::visit(MemberExpression& node) {
    Value object_value = evaluate(node.object.get());
    
    // Handle list properties
    if (std::holds_alternative<std::shared_ptr<ValueList>>(object_value)) {
        auto list = std::get<std::shared_ptr<ValueList>>(object_value);
        
        if (node.member == "length" || node.member == "size") {
            last_value = static_cast<int64_t>(list->size());
            return;
        }
    }
    
    // Handle dict properties
    if (std::holds_alternative<std::shared_ptr<ValueDict>>(object_value)) {
        auto dict = std::get<std::shared_ptr<ValueDict>>(object_value);
        
        if (node.member == "length" || node.member == "size") {
            last_value = static_cast<int64_t>(dict->size());
            return;
        }
        // Try to get the value with the member name as key
        auto it = dict->find(node.member);
        if (it != dict->end()) {
            last_value = it->second;
            return;
        }
    }
    
    // Handle string properties
    if (std::holds_alternative<std::string>(object_value)) {
        std::string str = std::get<std::string>(object_value);
        
        if (node.member == "length" || node.member == "size") {
            last_value = static_cast<int64_t>(str.length());
            return;
        }
    }
    
    // If we reach here, member access is not supported for this type
    throw RuntimeError("Cannot access member '" + node.member + "' on this type");
}

void Interpreter::visit(AssignmentExpression& node) {
    Value value = evaluate(node.value.get());
    
    if (auto identifier = dynamic_cast<IdentifierExpression*>(node.target.get())) {
        environment->define(identifier->name, value);
        last_value = value;
    } else {
        throw RuntimeError("Invalid assignment target");
    }
}

void Interpreter::visit(ListExpression& node) {
    // Create a new list and evaluate all elements
    auto list = std::make_shared<ValueList>();
    
    for (const auto& element : node.elements) {
        element->accept(*this);
        list->push_back(last_value);
    }
    
    last_value = list;
}

void Interpreter::visit(DictExpression& node) {
    // Create a new dictionary and evaluate all key-value pairs
    auto dict = std::make_shared<ValueDict>();
    
    for (const auto& pair : node.pairs) {
        // Evaluate key
        pair.first->accept(*this);
        Value key = last_value;
        
        // Convert key to string (dictionaries use string keys)
        std::string key_str = valueToString(key);
        
        // Evaluate value  
        pair.second->accept(*this);
        Value value = last_value;
        
        // Store in dictionary
        (*dict)[key_str] = value;
    }
    
    last_value = dict;
}

// Statement visitors
void Interpreter::visit(ExpressionStatement& node) {
    evaluate(node.expression.get());
}

void Interpreter::visit(BlockStatement& node) {
    for (auto& stmt : node.statements) {
        stmt->accept(*this);
    }
}

void Interpreter::visit(IfStatement& node) {
    Value condition = evaluate(node.condition.get());
    
    if (isTruthy(condition)) {
        node.then_block->accept(*this);
    } else if (node.else_block) {
        node.else_block->accept(*this);
    }
}

void Interpreter::visit(WhileStatement& node) {
    try {
        while (isTruthy(evaluate(node.condition.get()))) {
            try {
                node.body->accept(*this);
            } catch (const ContinueException&) {
                // Continue to next iteration
                continue;
            } catch (const BreakException&) {
                // Break out of loop
                break;
            }
        }
    } catch (const ReturnException&) {
        // Propagate return up the call stack
        throw;
    }
}

void Interpreter::visit(ForStatement& node) {
    // Evaluate the iterable expression
    Value iterable_value = evaluate(node.iterable.get());
    
    // Handle range() function calls for for-loops
    if (std::holds_alternative<std::string>(iterable_value)) {
        std::string str_val = std::get<std::string>(iterable_value);
        if (str_val.find("__range_") == 0) {
            // Parse range parameters from string like "__range_0_10_1"
            size_t first_underscore = str_val.find('_', 8);  // After "__range_"
            size_t second_underscore = str_val.find('_', first_underscore + 1);
            
            if (first_underscore != std::string::npos && second_underscore != std::string::npos) {
                int start = std::stoi(str_val.substr(8, first_underscore - 8));
                int end = std::stoi(str_val.substr(first_underscore + 1, second_underscore - first_underscore - 1));
                int step = std::stoi(str_val.substr(second_underscore + 1));
                
                try {
                    for (int i = start; i < end; i += step) {
                        environment->define(node.variable, static_cast<int64_t>(i));
                        try {
                            node.body->accept(*this);
                        } catch (const ContinueException&) {
                            continue;
                        } catch (const BreakException&) {
                            break;
                        }
                    }
                } catch (const ReturnException&) {
                    throw; // Propagate return
                }
            }
        } else {
            // Iterate over string characters
            try {
                for (size_t i = 0; i < str_val.size(); ++i) {
                    environment->define(node.variable, std::string(1, str_val[i]));
                    try {
                        node.body->accept(*this);
                    } catch (const ContinueException&) {
                        continue;
                    } catch (const BreakException&) {
                        break;
                    }
                }
            } catch (const ReturnException&) {
                throw; // Propagate return
            }
        }
    }
    // Handle list iteration
    else if (std::holds_alternative<std::shared_ptr<ValueList>>(iterable_value)) {
        auto list = std::get<std::shared_ptr<ValueList>>(iterable_value);
        try {
            for (const auto& item : *list) {
                environment->define(node.variable, item);
                try {
                    node.body->accept(*this);
                } catch (const ContinueException&) {
                    continue;
                } catch (const BreakException&) {
                    break;
                }
            }
        } catch (const ReturnException&) {
            throw; // Propagate return
        }
    }
    // Handle dictionary iteration (iterates over keys)
    else if (std::holds_alternative<std::shared_ptr<ValueDict>>(iterable_value)) {
        auto dict = std::get<std::shared_ptr<ValueDict>>(iterable_value);
        try {
            for (const auto& pair : *dict) {
                environment->define(node.variable, pair.first);
                try {
                    node.body->accept(*this);
                } catch (const ContinueException&) {
                    continue;
                } catch (const BreakException&) {
                    break;
                }
            }
        } catch (const ReturnException&) {
            throw; // Propagate return
        }
    }
}

void Interpreter::visit(FunctionDefinition& node) {
    // Create a callable function object with current environment as closure
    auto function = std::make_shared<CallableFunction>(
        std::shared_ptr<FunctionDefinition>(&node, [](FunctionDefinition*){}), // Non-owning shared_ptr
        environment
    );
    
    // Define the function in current environment
    environment->define(node.name, function);
}

void Interpreter::visit(ClassDefinition& node) {
    // Partial class support: stores class name but instantiation not yet implemented
    environment->define(node.name, std::string("__class_" + node.name));
    
    // Process class body to define any nested functions/classes
    if (node.body) {
        node.body->accept(*this);
    }
}

void Interpreter::visit(ReturnStatement& node) {
    Value return_value = nullptr;
    if (node.value) {
        return_value = evaluate(node.value.get());
    }
    throw ReturnException(return_value);
}

void Interpreter::visit(BreakStatement& node) {
    (void)node;
    throw BreakException();
}

void Interpreter::visit(ContinueStatement& node) {
    (void)node;
    throw ContinueException();
}

void Interpreter::visit(PassStatement& node) {
    (void)node;
    // Pass statement does nothing
}

void Interpreter::visit(Program& node) {
    for (auto& stmt : node.statements) {
        stmt->accept(*this);
    }
}

// Helper functions
void Interpreter::initializeBuiltins() {
    builtins["print"] = [this](const std::vector<Value>& args) -> Value {
        for (size_t i = 0; i < args.size(); ++i) {
            if (i > 0) std::cout << " ";
            std::cout << valueToString(args[i]);
        }
        std::cout << std::endl;
        return nullptr;
    };

    builtins["range"] = [](const std::vector<Value>& args) -> Value {
        if (args.empty() || args.size() > 3) {
            return nullptr; // Invalid range call
        }
        
        int64_t start = 0, end = 0, step = 1;
        
        if (args.size() == 1) {
            // range(n) -> 0 to n-1
            if (std::holds_alternative<int64_t>(args[0])) {
                end = std::get<int64_t>(args[0]);
            }
        } else if (args.size() == 2) {
            // range(start, end)
            if (std::holds_alternative<int64_t>(args[0]) && std::holds_alternative<int64_t>(args[1])) {
                start = std::get<int64_t>(args[0]);
                end = std::get<int64_t>(args[1]);
            }
        } else if (args.size() == 3) {
            // range(start, end, step)
            if (std::holds_alternative<int64_t>(args[0]) && 
                std::holds_alternative<int64_t>(args[1]) && 
                std::holds_alternative<int64_t>(args[2])) {
                start = std::get<int64_t>(args[0]);
                end = std::get<int64_t>(args[1]);
                step = std::get<int64_t>(args[2]);
            }
        }
        
        // Return a special string that ForStatement can recognize
        return std::string("__range_" + std::to_string(start) + "_" + std::to_string(end) + "_" + std::to_string(step));
    };

    builtins["len"] = [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) {
            throw RuntimeError("len() takes exactly one argument");
        }
        
        if (std::holds_alternative<std::string>(args[0])) {
            return static_cast<int64_t>(std::get<std::string>(args[0]).length());
        } else if (std::holds_alternative<std::shared_ptr<ValueList>>(args[0])) {
            auto list = std::get<std::shared_ptr<ValueList>>(args[0]);
            return static_cast<int64_t>(list->size());
        } else if (std::holds_alternative<std::shared_ptr<ValueDict>>(args[0])) {
            auto dict = std::get<std::shared_ptr<ValueDict>>(args[0]);
            return static_cast<int64_t>(dict->size());
        }
        
        throw RuntimeError("object has no len()");
    };

    builtins["str"] = [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) {
            throw RuntimeError("str() takes exactly one argument");
        }
        
        return std::visit([](const auto& v) -> std::string {
            using T = std::decay_t<decltype(v)>;
            if constexpr (std::is_same_v<T, std::nullptr_t>) {
                return "None";
            } else if constexpr (std::is_same_v<T, bool>) {
                return v ? "True" : "False";
            } else if constexpr (std::is_same_v<T, std::string>) {
                return v;
            } else if constexpr (std::is_same_v<T, int64_t>) {
                return std::to_string(v);
            } else if constexpr (std::is_same_v<T, double>) {
                return std::to_string(v);
            } else if constexpr (std::is_same_v<T, std::shared_ptr<CallableFunction>>) {
                return "<function " + v->getDeclaration()->name + ">";
            } else {
                return "[object]";
            }
        }, args[0]);
    };

    builtins["int"] = [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) {
            throw RuntimeError("int() takes exactly one argument");
        }
        
        if (std::holds_alternative<int64_t>(args[0])) {
            return std::get<int64_t>(args[0]);
        } else if (std::holds_alternative<double>(args[0])) {
            return static_cast<int64_t>(std::get<double>(args[0]));
        } else if (std::holds_alternative<std::string>(args[0])) {
            std::string str_val = std::get<std::string>(args[0]);
            
            // Handle boolean string literals
            if (str_val == "True") return static_cast<int64_t>(1);
            if (str_val == "False") return static_cast<int64_t>(0);
            
            try {
                return static_cast<int64_t>(std::stoll(str_val));
            } catch (...) {
                throw RuntimeError("invalid literal for int(): '" + str_val + "'");
            }
        } else if (std::holds_alternative<bool>(args[0])) {
            return static_cast<int64_t>(std::get<bool>(args[0]) ? 1 : 0);
        }
        
        throw RuntimeError("int() argument must be a string, a bytes-like object or a number");
    };

    builtins["float"] = [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) {
            throw RuntimeError("float() takes exactly one argument");
        }
        
        if (std::holds_alternative<double>(args[0])) {
            return std::get<double>(args[0]);
        } else if (std::holds_alternative<int64_t>(args[0])) {
            return static_cast<double>(std::get<int64_t>(args[0]));
        } else if (std::holds_alternative<std::string>(args[0])) {
            std::string str_val = std::get<std::string>(args[0]);
            
            // Handle boolean string literals
            if (str_val == "True") return 1.0;
            if (str_val == "False") return 0.0;
            
            try {
                return std::stod(str_val);
            } catch (...) {
                throw RuntimeError("could not convert string to float: '" + str_val + "'");
            }
        } else if (std::holds_alternative<bool>(args[0])) {
            return static_cast<double>(std::get<bool>(args[0]) ? 1.0 : 0.0);
        }
        
        throw RuntimeError("float() argument must be a string or a number");
    };

    builtins["type"] = [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) {
            throw RuntimeError("type() takes exactly one argument");
        }
        
        return std::visit([](const auto& v) -> std::string {
            using T = std::decay_t<decltype(v)>;
            if constexpr (std::is_same_v<T, std::nullptr_t>) {
                return "<class 'NoneType'>";
            } else if constexpr (std::is_same_v<T, bool>) {
                return "<class 'bool'>";
            } else if constexpr (std::is_same_v<T, std::string>) {
                return "<class 'str'>";
            } else if constexpr (std::is_same_v<T, int64_t>) {
                return "<class 'int'>";
            } else if constexpr (std::is_same_v<T, double>) {
                return "<class 'float'>";
            } else if constexpr (std::is_same_v<T, std::shared_ptr<CallableFunction>>) {
                return "<class 'function'>";
            } else if constexpr (std::is_same_v<T, std::shared_ptr<ValueList>>) {
                return "<class 'list'>";
            } else if constexpr (std::is_same_v<T, std::shared_ptr<ValueDict>>) {
                return "<class 'dict'>";
            } else {
                return "<class 'object'>";
            }
        }, args[0]);
    };

    builtins["abs"] = [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) {
            throw RuntimeError("abs() takes exactly one argument");
        }
        
        if (std::holds_alternative<int64_t>(args[0])) {
            int64_t val = std::get<int64_t>(args[0]);
            return val < 0 ? -val : val;
        } else if (std::holds_alternative<double>(args[0])) {
            double val = std::get<double>(args[0]);
            return val < 0.0 ? -val : val;
        }
        
        throw RuntimeError("bad operand type for abs()");
    };
    
    // Initialize special variables
    environment->define("__name__", std::string("__main__"));
}

std::string Interpreter::valueToString(const Value& value) {
    return std::visit([this](const auto& v) -> std::string {
        using T = std::decay_t<decltype(v)>;
        if constexpr (std::is_same_v<T, std::nullptr_t>) {
            return "None";
        } else if constexpr (std::is_same_v<T, bool>) {
            return v ? "True" : "False";
        } else if constexpr (std::is_same_v<T, std::string>) {
            return v;
        } else if constexpr (std::is_same_v<T, int64_t>) {
            return std::to_string(v);
        } else if constexpr (std::is_same_v<T, double>) {
            return std::to_string(v);
        } else if constexpr (std::is_same_v<T, std::shared_ptr<CallableFunction>>) {
            return "<function " + v->getDeclaration()->name + ">";
        } else if constexpr (std::is_same_v<T, std::shared_ptr<ValueList>>) {
            std::string result = "[";
            for (size_t i = 0; i < v->size(); ++i) {
                if (i > 0) result += ", ";
                result += valueToString((*v)[i]);
            }
            result += "]";
            return result;
        } else if constexpr (std::is_same_v<T, std::shared_ptr<ValueDict>>) {
            std::string result = "{";
            bool first = true;
            for (const auto& pair : *v) {
                if (!first) result += ", ";
                first = false;
                result += "\"" + pair.first + "\": " + valueToString(pair.second);
            }
            result += "}";
            return result;
        } else {
            return "[object]";
        }
    }, value);
}

bool Interpreter::isTruthy(const Value& value) {
    return std::visit([](const auto& v) -> bool {
        using T = std::decay_t<decltype(v)>;
        if constexpr (std::is_same_v<T, std::nullptr_t>) {
            return false;
        } else if constexpr (std::is_same_v<T, bool>) {
            return v;
        } else if constexpr (std::is_same_v<T, int64_t>) {
            return v != 0;
        } else if constexpr (std::is_same_v<T, double>) {
            return v != 0.0;
        } else if constexpr (std::is_same_v<T, std::string>) {
            return !v.empty();
        } else if constexpr (std::is_same_v<T, std::shared_ptr<CallableFunction>>) {
            return true; // Functions are always truthy
        } else if constexpr (std::is_same_v<T, std::shared_ptr<ValueList>>) {
            return !v->empty(); // Empty lists are falsy
        } else if constexpr (std::is_same_v<T, std::shared_ptr<ValueDict>>) {
            return !v->empty(); // Empty dictionaries are falsy
        } else {
            return true;
        }
    }, value);
}

bool Interpreter::isEqual(const Value& a, const Value& b) {
    if (a.index() != b.index()) return false;
    
    return std::visit([](const auto& left, const auto& right) -> bool {
        using T = std::decay_t<decltype(left)>;
        using U = std::decay_t<decltype(right)>;
        if constexpr (std::is_same_v<T, U>) {
            if constexpr (std::is_same_v<T, std::shared_ptr<ValueList>>) {
                // Compare list contents
                if (left->size() != right->size()) return false;
                for (size_t i = 0; i < left->size(); ++i) {
                    // Note: This is a simplified comparison
                    // In a full implementation, we might need recursive equality checking
                    if ((*left)[i].index() != (*right)[i].index()) return false;
                }
                return true;
            } else if constexpr (std::is_same_v<T, std::shared_ptr<ValueDict>>) {
                // Compare dictionary contents
                if (left->size() != right->size()) return false;
                for (const auto& pair : *left) {
                    auto it = right->find(pair.first);
                    if (it == right->end()) return false;
                    // Note: This is a simplified comparison
                    if (pair.second.index() != it->second.index()) return false;
                }
                return true;
            } else {
                return left == right;
            }
        } else {
            return false;
        }
    }, a, b);
}

Value Interpreter::tokenToValue(const Token& token) {
    switch (token.type) {
        case TokenType::INTEGER:
            return static_cast<int64_t>(std::stoll(token.value));
        case TokenType::FLOAT:
            return std::stod(token.value);
        case TokenType::STRING:
            return token.value;
        default:
            return std::string(token.value);
    }
}

} // namespace caesar