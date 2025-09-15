/**
 * @file main.cpp
 * @brief Main entry point for the Caesar compiler
 * @author Julius Pleunes
 * @version 1.0.0
 */

#include "caesar/caesar.h"
#include "caesar/lexer.h"
#include <iostream>
#include <fstream>
#include <sstream>

void printUsage(const char* program_name) {
    std::cout << "Usage: " << program_name << " [options] <input_file>\n";
    std::cout << "Options:\n";
    std::cout << "  -h, --help     Show this help message\n";
    std::cout << "  -v, --version  Show version information\n";
    std::cout << "  -t, --tokens   Show tokenization output\n";
    std::cout << "  -o <output>    Specify output file\n";
}

void printVersion() {
    std::cout << "Caesar Programming Language v" << caesar::Version::STRING << "\n";
    std::cout << "Built with LLVM backend for high-performance compilation\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }
    
    bool show_tokens = false;
    std::string input_file;
    std::string output_file;
    
    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            printUsage(argv[0]);
            return 0;
        } else if (arg == "-v" || arg == "--version") {
            printVersion();
            return 0;
        } else if (arg == "-t" || arg == "--tokens") {
            show_tokens = true;
        } else if (arg == "-o" && i + 1 < argc) {
            output_file = argv[++i];
        } else if (arg[0] != '-') {
            input_file = arg;
        } else {
            std::cerr << "Unknown option: " << arg << "\n";
            printUsage(argv[0]);
            return 1;
        }
    }
    
    if (input_file.empty()) {
        std::cerr << "Error: No input file specified\n";
        printUsage(argv[0]);
        return 1;
    }
    
    try {
        // Read input file
        std::ifstream file(input_file);
        if (!file.is_open()) {
            std::cerr << "Error: Cannot open file '" << input_file << "'\n";
            return 1;
        }
        
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string source = buffer.str();
        file.close();
        
        // Tokenize
        caesar::Lexer lexer(source);
        auto tokens = lexer.tokenize();
        
        if (show_tokens) {
            std::cout << "Tokens:\n";
            for (const auto& token : tokens) {
                std::cout << "  " << token << "\n";
            }
            return 0;
        }
        
        // TODO: Add parser, IR generation, and compilation stages
        std::cout << "Successfully tokenized " << tokens.size() << " tokens from '" 
                  << input_file << "'\n";
        std::cout << "Note: Full compilation pipeline not yet implemented.\n";
        
    } catch (const caesar::CaesarException& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}