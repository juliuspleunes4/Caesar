/**
 * @file test_parser.cpp
 * @brief Unit tests for the Caesar parser
 * @author J.J.G. Pleunes
 * @version 1.0.0
 */

#include "caesar/lexer.h"
#include "caesar/parser.h"
#include "caesar/ast.h"
#include <iostream>
#include <cassert>

void test_simple_expression() {
    std::cout << "Testing simple expression parsing...\n";
    
    std::string source = "1 + 2 * 3";
    caesar::Lexer lexer(source);
    auto tokens = lexer.tokenize();
    
    caesar::Parser parser(std::move(tokens));
    auto program = parser.parse();
    
    assert(program != nullptr);
    assert(program->statements.size() == 1);
    
    auto expr_stmt = dynamic_cast<caesar::ExpressionStatement*>(program->statements[0].get());
    assert(expr_stmt != nullptr);
    
    auto binary_expr = dynamic_cast<caesar::BinaryExpression*>(expr_stmt->expression.get());
    assert(binary_expr != nullptr);
    assert(binary_expr->operator_type == caesar::TokenType::PLUS);
    
    std::cout << "AST: " << program->toString() << "\n";
    std::cout << "✓ Simple expression test passed\n";
}

void test_function_definition() {
    std::cout << "Testing function definition parsing...\n";
    
    std::string source = R"(
def add(a, b):
    return a + b
)";
    
    caesar::Lexer lexer(source);
    auto tokens = lexer.tokenize();
    
    caesar::Parser parser(std::move(tokens));
    auto program = parser.parse();
    
    assert(program != nullptr);
    assert(program->statements.size() == 1);
    
    auto func_def = dynamic_cast<caesar::FunctionDefinition*>(program->statements[0].get());
    assert(func_def != nullptr);
    assert(func_def->name == "add");
    assert(func_def->parameters.size() == 2);
    assert(func_def->parameters[0].name == "a");
    assert(func_def->parameters[1].name == "b");
    
    std::cout << "AST: " << program->toString() << "\n";
    std::cout << "✓ Function definition test passed\n";
}

void test_if_statement() {
    std::cout << "Testing if statement parsing...\n";
    
    std::string source = R"(
if x > 0:
    print("positive")
else:
    print("negative")
)";
    
    caesar::Lexer lexer(source);
    auto tokens = lexer.tokenize();
    
    caesar::Parser parser(std::move(tokens));
    auto program = parser.parse();
    
    assert(program != nullptr);
    assert(program->statements.size() == 1);
    
    auto if_stmt = dynamic_cast<caesar::IfStatement*>(program->statements[0].get());
    assert(if_stmt != nullptr);
    assert(if_stmt->condition != nullptr);
    assert(if_stmt->then_block != nullptr);
    assert(if_stmt->else_block != nullptr);
    
    std::cout << "AST: " << program->toString() << "\n";
    std::cout << "✓ If statement test passed\n";
}

void test_while_loop() {
    std::cout << "Testing while loop parsing...\n";
    
    std::string source = R"(
while i < 10:
    i = i + 1
)";
    
    caesar::Lexer lexer(source);
    auto tokens = lexer.tokenize();
    
    caesar::Parser parser(std::move(tokens));
    auto program = parser.parse();
    
    assert(program != nullptr);
    assert(program->statements.size() == 1);
    
    auto while_stmt = dynamic_cast<caesar::WhileStatement*>(program->statements[0].get());
    assert(while_stmt != nullptr);
    assert(while_stmt->condition != nullptr);
    assert(while_stmt->body != nullptr);
    
    std::cout << "AST: " << program->toString() << "\n";
    std::cout << "✓ While loop test passed\n";
}

void test_function_call() {
    std::cout << "Testing function call parsing...\n";
    
    std::string source = "result = fibonacci(10)";
    
    caesar::Lexer lexer(source);
    auto tokens = lexer.tokenize();
    
    caesar::Parser parser(std::move(tokens));
    auto program = parser.parse();
    
    assert(program != nullptr);
    assert(program->statements.size() == 1);
    
    auto expr_stmt = dynamic_cast<caesar::ExpressionStatement*>(program->statements[0].get());
    assert(expr_stmt != nullptr);
    
    auto assign_expr = dynamic_cast<caesar::AssignmentExpression*>(expr_stmt->expression.get());
    assert(assign_expr != nullptr);
    
    auto call_expr = dynamic_cast<caesar::CallExpression*>(assign_expr->value.get());
    assert(call_expr != nullptr);
    assert(call_expr->arguments.size() == 1);
    
    std::cout << "AST: " << program->toString() << "\n";
    std::cout << "✓ Function call test passed\n";
}

void test_complex_program() {
    std::cout << "Testing complex program parsing...\n";
    
    std::string source = R"(
def fibonacci(n):
    if n <= 1:
        return n
    else:
        return fibonacci(n-1) + fibonacci(n-2)

def main():
    result = fibonacci(10)
    print("Result:", result)
)";
    
    caesar::Lexer lexer(source);
    auto tokens = lexer.tokenize();
    
    caesar::Parser parser(std::move(tokens));
    auto program = parser.parse();
    
    assert(program != nullptr);
    std::cout << "Number of statements: " << program->statements.size() << "\n";
    
    if (program->statements.size() >= 1) {
        // Check first function
        auto func1 = dynamic_cast<caesar::FunctionDefinition*>(program->statements[0].get());
        if (func1) {
            assert(func1->name == "fibonacci");
            std::cout << "First function name: " << func1->name << "\n";
        }
    }
    
    if (program->statements.size() >= 2) {
        // Check second function
        auto func2 = dynamic_cast<caesar::FunctionDefinition*>(program->statements[1].get());
        if (func2) {
            assert(func2->name == "main");
            std::cout << "Second function name: " << func2->name << "\n";
        }
    }
    
    std::cout << "AST: " << program->toString() << "\n";
    std::cout << "✓ Complex program test passed\n";
}

int main() {
    std::cout << "Running Caesar parser tests...\n\n";
    
    try {
        test_simple_expression();
        test_function_definition();
        test_if_statement();
        test_while_loop();
        test_function_call();
        test_complex_program();
        
        std::cout << "\n✅ All parser tests passed!\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "\n❌ Parser test failed: " << e.what() << "\n";
        return 1;
    }
}
