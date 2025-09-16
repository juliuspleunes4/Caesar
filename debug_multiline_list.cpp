#include "caesar/caesar.h"
#include "caesar/lexer.h"
#include "caesar/parser.h"
#include <iostream>

int main() {
    // Test multiline list parsing like the stress test
    std::string source = R"(large_list = [
    1,
    2,
    3
])";
    
    std::cout << "=== MULTILINE LIST TEST ===\n";
    std::cout << "Source:\n" << source << "\n\n";
    
    try {
        caesar::Lexer lexer(source);
        auto tokens = lexer.tokenize();
        
        std::cout << "Tokens:\n";
        for (size_t i = 0; i < tokens.size(); i++) {
            std::cout << "[" << i << "] type=" << (int)tokens[i].type 
                      << " value='" << tokens[i].value 
                      << "' line=" << tokens[i].position.line 
                      << " col=" << tokens[i].position.column;
            
            // Add token type names
            switch(tokens[i].type) {
                case (caesar::TokenType)50: std::cout << " (NEWLINE)"; break;
                case (caesar::TokenType)51: std::cout << " (INDENT)"; break;
                case (caesar::TokenType)52: std::cout << " (DEDENT)"; break;
                case (caesar::TokenType)53: std::cout << " (EOF)"; break;
                case (caesar::TokenType)42: std::cout << " (LBRACKET)"; break;
                case (caesar::TokenType)43: std::cout << " (RBRACKET)"; break;
                case (caesar::TokenType)46: std::cout << " (COMMA)"; break;
                case (caesar::TokenType)0: std::cout << " (INTEGER)"; break;
                case (caesar::TokenType)4: std::cout << " (IDENTIFIER)"; break;
                case (caesar::TokenType)29: std::cout << " (ASSIGN)"; break;
            }
            std::cout << "\n";
        }
        
        std::cout << "\n=== Parsing ===\n";
        caesar::Parser parser(std::move(tokens));
        auto program = parser.parse();
        std::cout << "SUCCESS: Multiline list parsed successfully!\n";
        std::cout << "AST: " << program->toString() << "\n";
        
    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    
    return 0;
}