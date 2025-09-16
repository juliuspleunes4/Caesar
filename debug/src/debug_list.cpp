#include "include/caesar/caesar.h"
#include "include/caesar/lexer.h"
#include "include/caesar/parser.h"
#include <iostream>

int main() {
    // Test simple list parsing
    std::string source = R"(test_list = [1, 2, 3])";
    
    std::cout << "=== SIMPLE LIST TEST ===\n";
    std::cout << "Source: " << source << "\n\n";
    
    try {
        caesar::Lexer lexer(source);
        auto tokens = lexer.tokenize();
        
        std::cout << "Tokens:\n";
        for (size_t i = 0; i < tokens.size(); i++) {
            std::cout << "[" << i << "] type=" << (int)tokens[i].type 
                      << " value='" << tokens[i].value 
                      << "' line=" << tokens[i].position.line 
                      << " col=" << tokens[i].position.column << "\n";
        }
        
        std::cout << "\n=== Parsing ===\n";
        caesar::Parser parser(std::move(tokens));
        auto program = parser.parse();
        std::cout << "SUCCESS: Simple list parsed successfully!\n";
        std::cout << "AST: " << program->toString() << "\n";
        
    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    
    return 0;
}
