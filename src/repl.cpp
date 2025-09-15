/**
 * @file repl.cpp
 * @brief Interactive REPL for the Caesar programming language
 * @author Julius Pleunes
 * @version 1.0.0
 */

#include "caesar/caesar.h"
#include "caesar/lexer.h"
#include <iostream>
#include <string>

void printWelcome() {
    std::cout << "Caesar Programming Language REPL v" << caesar::Version::STRING << "\n";
    std::cout << "Type 'exit' or 'quit' to exit, 'help' for help\n\n";
}

void printHelp() {
    std::cout << "Caesar REPL Commands:\n";
    std::cout << "  help     - Show this help message\n";
    std::cout << "  exit     - Exit the REPL\n";
    std::cout << "  quit     - Exit the REPL\n";
    std::cout << "  tokens   - Toggle token display mode\n";
    std::cout << "\nEnter Caesar code to see tokenization (compilation coming soon!)\n";
}

int main() {
    printWelcome();
    
    bool show_tokens = true;
    std::string line;
    
    while (true) {
        std::cout << "caesar> ";
        if (!std::getline(std::cin, line)) {
            break; // EOF
        }
        
        // Trim whitespace
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);
        
        if (line.empty()) {
            continue;
        }
        
        // Handle commands
        if (line == "exit" || line == "quit") {
            std::cout << "Goodbye!\n";
            break;
        } else if (line == "help") {
            printHelp();
            continue;
        } else if (line == "tokens") {
            show_tokens = !show_tokens;
            std::cout << "Token display " << (show_tokens ? "enabled" : "disabled") << "\n";
            continue;
        }
        
        try {
            // Tokenize the input
            caesar::Lexer lexer(line);
            auto tokens = lexer.tokenize();
            
            if (show_tokens) {
                std::cout << "Tokens:\n";
                for (const auto& token : tokens) {
                    if (token.type != caesar::TokenType::EOF_TOKEN) {
                        std::cout << "  " << token << "\n";
                    }
                }
            } else {
                std::cout << "Tokenized " << tokens.size() - 1 << " tokens\n"; // -1 for EOF
            }
            
            // TODO: Add parsing, IR generation, and execution
            
        } catch (const caesar::CaesarException& e) {
            std::cout << "Error: " << e.what() << "\n";
        } catch (const std::exception& e) {
            std::cout << "Unexpected error: " << e.what() << "\n";
        }
    }
    
    return 0;
}