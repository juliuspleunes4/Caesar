#include "caesar/caesar.h"
#include "caesar/lexer.h"
#include "caesar/parser.h"
#include <iostream>

int main() {
    std::string source = R"(
def test_nested():
    if condition_0:
        if condition_1:
            return "deep"
        else:
            return "not_deep_1"
    else:
        return "not_deep_0"
)";
    
    std::cout << "Source code:\n" << source << "\n";
    std::cout << "=" << std::string(50, '=') << "\n";
    
    try {
        caesar::Lexer lexer(source);
        auto tokens = lexer.tokenize();
        
        std::cout << "Generated tokens:\n";
        for (size_t i = 0; i < tokens.size(); ++i) {
            std::cout << "[" << i << "] " 
                      << "type=" << static_cast<int>(tokens[i].type) 
                      << " value='" << tokens[i].value << "'"
                      << " line=" << tokens[i].position.line
                      << " col=" << tokens[i].position.column << "\n";
        }
        
        std::cout << "\n" << std::string(50, '=') << "\n";
        std::cout << "Attempting to parse...\n";
        
        caesar::Parser parser(std::move(tokens));
        auto program = parser.parse();
        
        if (program) {
            std::cout << "✓ Parsing succeeded!\n";
        } else {
            std::cout << "❌ Parsing failed!\n";
        }
        
    } catch (const std::exception& e) {
        std::cout << "❌ Exception: " << e.what() << "\n";
    }
    
    return 0;
}