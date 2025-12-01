/**
 * @file repl.cpp
 * @brief Interactive REPL for the Caesar programming language
 * @author J.J.G. Pleunes
 * @version 1.5.0
 */

#include "caesar/caesar.h"
#include "caesar/lexer.h"
#include "caesar/parser.h"
#include "caesar/interpreter.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

// Ensure std types are available
using std::vector;
using std::string;

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
    std::cout << "  run      - Toggle execution mode (on by default)\n";
    std::cout << "\nEnter Caesar code to execute it.\n";
    std::cout << "For multi-line input, use 'caesar -i file.csr'\n";
}

std::string valueToString(const caesar::Value& value) {
    return std::visit([](const auto& v) -> std::string {
        using T = std::decay_t<decltype(v)>;
        if constexpr (std::is_same_v<T, std::nullptr_t>) {
            return "None";
        } else if constexpr (std::is_same_v<T, bool>) {
            return v ? "True" : "False";
        } else if constexpr (std::is_same_v<T, std::string>) {
            return "\"" + v + "\"";
        } else if constexpr (std::is_same_v<T, int64_t>) {
            return std::to_string(v);
        } else if constexpr (std::is_same_v<T, double>) {
            std::ostringstream oss;
            oss << v;
            return oss.str();
        } else if constexpr (std::is_same_v<T, std::shared_ptr<caesar::CallableFunction>>) {
            return "<function>";
        } else if constexpr (std::is_same_v<T, std::shared_ptr<caesar::ValueList>>) {
            std::string result = "[";
            bool first = true;
            for (const auto& item : *v) {
                if (!first) result += ", ";
                first = false;
                result += valueToString(item);
            }
            result += "]";
            return result;
        } else if constexpr (std::is_same_v<T, std::shared_ptr<caesar::ValueDict>>) {
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
            return "[unknown]";
        }
    }, value);
}

int main() {
    printWelcome();
    
    bool show_tokens = false;
    bool run_code = true;
    std::string line;
    caesar::Interpreter interpreter;
    
    while (true) {
        std::cout << ">>> ";
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
        } else if (line == "run") {
            run_code = !run_code;
            std::cout << "Execution " << (run_code ? "enabled" : "disabled") << "\n";
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
            }
            
            if (run_code) {
                // Parse and execute
                caesar::Parser parser(tokens);
                auto program = parser.parse();
                
                if (program) {
                    caesar::Value result = interpreter.interpret(program.get());
                    
                    // Only print result if it's not None (to mimic Python REPL behavior)
                    if (!std::holds_alternative<std::nullptr_t>(result)) {
                        std::cout << valueToString(result) << "\n";
                    }
                }
            }
            
        } catch (const caesar::RuntimeError& e) {
            std::cout << "Runtime Error: " << e.what() << "\n";
        } catch (const caesar::CaesarException& e) {
            std::cout << "Error: " << e.what() << "\n";
        } catch (const std::exception& e) {
            std::cout << "Unexpected error: " << e.what() << "\n";
        }
    }
    
    return 0;
}
