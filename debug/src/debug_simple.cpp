#include "include/caesar/caesar.h"
#include "include/caesar/lexer.h"
#include "include/caesar/parser.h"
#include <iostream>

int main() {
    // Simplest case - single level if/else
    std::string source = R"(
if condition:
    return "then"
else:
    return "else"
)";
    
    std::cout << "=== SIMPLE IF/ELSE TEST ===\n";
    std::cout << "Source:\n" << source << "\n\n";
    
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
        std::cout << "SUCCESS: Single level if/else parsed successfully!\n";
        
    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    
    return 0;
}
