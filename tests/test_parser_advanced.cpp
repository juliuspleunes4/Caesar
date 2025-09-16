/**
 * @file test_parser_advanced.cpp
 * @brief Advanced unit tests for the Caesar parser
 * @author J.J.G. Pleunes
 * @version 1.0.0
 */

#undef NDEBUG
#include "caesar/lexer.h"
#include "caesar/parser.h"
#include "caesar/ast.h"
#include <iostream>
#include <cassert>
#include <vector>
#include <cstdlib>

// Ensure std types are available
using std::vector;

// Simple assert replacement for debugging
#define my_assert(condition) \
    do { \
        if (!(condition)) { \
            std::cerr << "Assertion failed: " << #condition << " at line " << __LINE__ << std::endl; \
            std::abort(); \
        } \
    } while(0)

#ifndef assert
#define assert my_assert
#endif

void test_operator_precedence() {
    std::cout << "Testing operator precedence...\n";
    
    // Test arithmetic precedence: 1 + 2 * 3 should be 1 + (2 * 3)
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
    
    // Right side should be multiplication
    auto right_binary = dynamic_cast<caesar::BinaryExpression*>(binary_expr->right.get());
    assert(right_binary != nullptr);
    assert(right_binary->operator_type == caesar::TokenType::MULTIPLY);
    
    std::cout << "✓ Operator precedence test passed\n";
}

void test_complex_expressions() {
    std::cout << "Testing complex expressions...\n";
    
    std::string source = "a and b or c and not d == e + f * g ** h";
    caesar::Lexer lexer(source);
    auto tokens = lexer.tokenize();
    
    caesar::Parser parser(std::move(tokens));
    auto program = parser.parse();
    
    assert(program != nullptr);
    assert(program->statements.size() == 1);
    
    auto expr_stmt = dynamic_cast<caesar::ExpressionStatement*>(program->statements[0].get());
    assert(expr_stmt != nullptr);
    
    // Should parse without errors and create proper AST structure
    std::string ast_str = program->toString();
    assert(!ast_str.empty());
    
    std::cout << "Complex expression AST: " << ast_str << "\n";
    std::cout << "✓ Complex expressions test passed\n";
}

void test_nested_function_calls() {
    std::cout << "Testing nested function calls...\n";
    
    std::string source = "func1(func2(a, b), func3(c, func4(d)))";
    caesar::Lexer lexer(source);
    auto tokens = lexer.tokenize();
    
    caesar::Parser parser(std::move(tokens));
    auto program = parser.parse();
    
    assert(program != nullptr);
    
    auto expr_stmt = dynamic_cast<caesar::ExpressionStatement*>(program->statements[0].get());
    assert(expr_stmt != nullptr);
    
    auto call_expr = dynamic_cast<caesar::CallExpression*>(expr_stmt->expression.get());
    assert(call_expr != nullptr);
    assert(call_expr->arguments.size() == 2);
    
    // First argument should be another function call
    auto first_arg = dynamic_cast<caesar::CallExpression*>(call_expr->arguments[0].get());
    assert(first_arg != nullptr);
    
    std::cout << "✓ Nested function calls test passed\n";
}

void test_deeply_nested_blocks() {
    std::cout << "Testing deeply nested blocks...\n";
    
    std::string source = R"(
def outer():
    if condition1:
        while loop_condition:
            if condition2:
                for item in items:
                    if condition3:
                        if condition4:
                            return item
                        else:
                            continue
                    else:
                        break
            else:
                pass
    else:
        return None
)";
    
    caesar::Lexer lexer(source);
    auto tokens = lexer.tokenize();
    
    caesar::Parser parser(std::move(tokens));
    auto program = parser.parse();
    
    assert(program != nullptr);
    assert(program->statements.size() == 1);
    
    auto func_def = dynamic_cast<caesar::FunctionDefinition*>(program->statements[0].get());
    assert(func_def != nullptr);
    assert(func_def->name == "outer");
    
    std::cout << "✓ Deeply nested blocks test passed\n";
}

void test_multiple_function_definitions() {
    std::cout << "Testing multiple function definitions...\n";
    
    std::string source = R"(
def func1(a):
    return a + 1

def func2(b, c):
    return func1(b) + c

def func3():
    pass
)";
    
    caesar::Lexer lexer(source);
    auto tokens = lexer.tokenize();
    
    caesar::Parser parser(std::move(tokens));
    auto program = parser.parse();
    
    assert(program != nullptr);
    assert(program->statements.size() == 3);
    
    for (int i = 0; i < 3; ++i) {
        auto func_def = dynamic_cast<caesar::FunctionDefinition*>(program->statements[i].get());
        assert(func_def != nullptr);
    }
    
    auto func1 = dynamic_cast<caesar::FunctionDefinition*>(program->statements[0].get());
    assert(func1->name == "func1");
    assert(func1->parameters.size() == 1);
    
    auto func2 = dynamic_cast<caesar::FunctionDefinition*>(program->statements[1].get());
    assert(func2->name == "func2");
    assert(func2->parameters.size() == 2);
    
    auto func3 = dynamic_cast<caesar::FunctionDefinition*>(program->statements[2].get());
    assert(func3->name == "func3");
    assert(func3->parameters.size() == 0);
    
    std::cout << "✓ Multiple function definitions test passed\n";
}

void test_complex_control_flow() {
    std::cout << "Testing complex control flow...\n";
    
    std::string source = R"(
if x > 0:
    if y > 0:
        result = "positive"
    elif y == 0:
        result = "zero"
    else:
        result = "negative y"
elif x == 0:
    result = "zero x"
else:
    result = "negative x"
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
    
    std::cout << "✓ Complex control flow test passed\n";
}

void test_assignment_expressions() {
    std::cout << "Testing assignment expressions...\n";
    
    std::string source = R"(
x = 1
y = x + 2
z = func(a, b)
result = x + y * z
)";
    
    caesar::Lexer lexer(source);
    auto tokens = lexer.tokenize();
    
    caesar::Parser parser(std::move(tokens));
    auto program = parser.parse();
    
    assert(program != nullptr);
    assert(program->statements.size() == 4);
    
    for (int i = 0; i < 4; ++i) {
        auto expr_stmt = dynamic_cast<caesar::ExpressionStatement*>(program->statements[i].get());
        assert(expr_stmt != nullptr);
        
        auto assign_expr = dynamic_cast<caesar::AssignmentExpression*>(expr_stmt->expression.get());
        assert(assign_expr != nullptr);
    }
    
    std::cout << "✓ Assignment expressions test passed\n";
}

void test_for_loops_advanced() {
    std::cout << "Testing advanced for loops...\n";
    
    std::string source = R"(
for item in collection:
    for subitem in item:
        if condition(subitem):
            process(subitem)
        else:
            skip(subitem)
)";
    
    caesar::Lexer lexer(source);
    auto tokens = lexer.tokenize();
    
    caesar::Parser parser(std::move(tokens));
    auto program = parser.parse();
    
    assert(program != nullptr);
    assert(program->statements.size() == 1);
    
    auto for_stmt = dynamic_cast<caesar::ForStatement*>(program->statements[0].get());
    assert(for_stmt != nullptr);
    assert(for_stmt->variable == "item");
    
    std::cout << "✓ Advanced for loops test passed\n";
}

void test_while_loops_advanced() {
    std::cout << "Testing advanced while loops...\n";
    
    std::string source = R"(
while condition1:
    while condition2:
        if break_condition:
            break
        if continue_condition:
            continue
        process()
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
    
    std::cout << "✓ Advanced while loops test passed\n";
}

void test_return_statements_complex() {
    std::cout << "Testing complex return statements...\n";
    
    std::string source = R"(
def complex_return():
    if condition:
        return func1(a) + func2(b)
    else:
        return None

def simple_return():
    return

def expression_return():
    return a and b or c
)";
    
    caesar::Lexer lexer(source);
    auto tokens = lexer.tokenize();
    
    caesar::Parser parser(std::move(tokens));
    auto program = parser.parse();
    
    assert(program != nullptr);
    assert(program->statements.size() == 3);
    
    // All should be function definitions
    for (int i = 0; i < 3; ++i) {
        auto func_def = dynamic_cast<caesar::FunctionDefinition*>(program->statements[i].get());
        assert(func_def != nullptr);
    }
    
    std::cout << "✓ Complex return statements test passed\n";
}

int main() {
    std::cout << "Running Caesar advanced parser tests...\n\n";
    
    try {
        test_operator_precedence();
        test_complex_expressions();
        test_nested_function_calls();
        test_deeply_nested_blocks();
        test_multiple_function_definitions();
        test_complex_control_flow();
        test_assignment_expressions();
        test_for_loops_advanced();
        test_while_loops_advanced();
        test_return_statements_complex();
        
        std::cout << "\n✅ All advanced parser tests passed!\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "\n❌ Advanced parser test failed: " << e.what() << "\n";
        return 1;
    }
}
