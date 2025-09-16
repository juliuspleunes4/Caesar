#include "caesar/caesar.h"
#include "caesar/lexer.h"
#include "caesar/parser.h"
#include <iostream>

int main() {
    // Two level nesting - minimal case
    std::string source = R"(
if outer:
    if inner:
        return "nested"
    else:
        return "inner_else"
)";
    
    std::cout << "=== TWO LEVEL NESTING TEST ===\n";
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
            
            // Add token type names for clarity
            switch(tokens[i].type) {
                case (caesar::TokenType)50: std::cout << " (NEWLINE)"; break;
                case (caesar::TokenType)51: std::cout << " (INDENT)"; break;
                case (caesar::TokenType)52: std::cout << " (DEDENT)"; break;
                case (caesar::TokenType)53: std::cout << " (EOF)"; break;
                case (caesar::TokenType)5: std::cout << " (IF)"; break;
                case (caesar::TokenType)7: std::cout << " (ELSE)"; break;
                case (caesar::TokenType)47: std::cout << " (COLON)"; break;
                case (caesar::TokenType)13: std::cout << " (RETURN)"; break;
                case (caesar::TokenType)4: std::cout << " (IDENTIFIER)"; break;
                case (caesar::TokenType)2: std::cout << " (STRING)"; break;
            }
            std::cout << "\n";
        }
        
        std::cout << "\n=== Parsing ===\n";
        caesar::Parser parser(std::move(tokens));
        auto program = parser.parse();
        std::cout << "SUCCESS: Two level nesting parsed successfully!\n";
        
    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    
    return 0;
}