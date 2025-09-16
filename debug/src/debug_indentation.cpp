#include "include/caesar/caesar.h"
#include "include/caesar/lexer.h"
#include <iostream>

int main() {
    // Test the exact indentation pattern from the failing case
    std::string source = R"(
if outer:
    if inner:
        return "nested"
    else:
        return "inner_else"
else:
    return "outer_else"
)";
    
    std::cout << "=== DETAILED INDENTATION ANALYSIS ===\n";
    std::cout << "Source with visible indentation:\n";
    
    // Print source with indentation markers
    std::istringstream iss(source);
    std::string line;
    int lineNum = 1;
    while (std::getline(iss, line)) {
        std::cout << "Line " << lineNum << ": ";
        for (size_t i = 0; i < line.length(); i++) {
            if (line[i] == ' ') {
                std::cout << "·";  // dot for space
            } else if (line[i] == '\t') {
                std::cout << "→";  // arrow for tab
            } else {
                std::cout << line[i];
            }
        }
        std::cout << " [" << line.length() << " chars]\n";
        lineNum++;
    }
    
    std::cout << "\n=== TOKENIZATION ===\n";
    
    try {
        caesar::Lexer lexer(source);
        auto tokens = lexer.tokenize();
        
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
                case (caesar::TokenType)5: std::cout << " (IF)"; break;
                case (caesar::TokenType)7: std::cout << " (ELSE)"; break;
                case (caesar::TokenType)47: std::cout << " (COLON)"; break;
                case (caesar::TokenType)13: std::cout << " (RETURN)"; break;
                case (caesar::TokenType)4: std::cout << " (IDENTIFIER)"; break;
                case (caesar::TokenType)2: std::cout << " (STRING)"; break;
            }
            std::cout << "\n";
        }
        
    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    
    return 0;
}
