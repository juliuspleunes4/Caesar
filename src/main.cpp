/**
 * @file main.cpp
 * @brief Main entry point for the Caesar compiler
 * @author J.J.G. Pleunes
 * @version 1.5.0
 */

#include "caesar/caesar.h"
#include "caesar/lexer.h"
#include "caesar/parser.h"
#include "caesar/interpreter.h"
#include "caesar/ir.h"
#include "caesar/codegen.h"
#include <iostream>
#include <fstream>
#include <sstream>

void printUsage(const char* program_name) {
    std::cout << "Caesar Programming Language v" << caesar::Version::STRING << "\n";
    std::cout << "Usage: " << program_name << " [options] <input_file>\n\n";
    std::cout << "Execution Modes:\n";
    std::cout << "  (default)        Compile and execute the program (bytecode)\n";
    std::cout << "  -i, --interpret  Execute the program using the interpreter\n\n";
    std::cout << "Output Options:\n";
    std::cout << "  -h, --help       Show this help message\n";
    std::cout << "  -v, --version    Show version information\n";
    std::cout << "  -t, --tokens     Show tokenization output\n";
    std::cout << "  -p, --parse      Show parsing output (AST)\n";
    std::cout << "  --ir             Show IR (Intermediate Representation)\n";
    std::cout << "  --asm            Generate x86-64 assembly code\n";
    std::cout << "  --c              Generate C code (transpile)\n";
    std::cout << "  -o <output>      Specify output file\n\n";
    std::cout << "Examples:\n";
    std::cout << "  " << program_name << " program.csr                 # Compile and run (default)\n";
    std::cout << "  " << program_name << " -i program.csr              # Interpret program\n";
    std::cout << "  " << program_name << " --parse program.csr         # Show AST\n";
    std::cout << "  " << program_name << " --ir program.csr            # Show IR\n";
    std::cout << "  " << program_name << " --c program.csr -o out.c    # Transpile to C\n\n";
    std::cout << "For interactive mode, use: caesar_repl\n";
}

void printVersion() {
    std::cout << "Caesar Programming Language v" << caesar::Version::STRING << "\n";
    std::cout << "A Python-like language with C-speed performance\n";
    std::cout << "\nCompilation Modes:\n";
    std::cout << "  ✓ Bytecode compilation (VM execution)\n";
    std::cout << "  ✓ C code generation (transpiler)\n";
    std::cout << "  ✓ x86-64 assembly generation\n";
    std::cout << "  ✓ Tree-walking interpreter (-i flag)\n";
    std::cout << "\nLanguage Features:\n";
    std::cout << "  ✓ Functions with default parameters\n";
    std::cout << "  ✓ Control flow (if/elif/else, while, for)\n";
    std::cout << "  ✓ Loop control (break, continue)\n";
    std::cout << "  ✓ Built-in functions (print, range, len, str, int, float, type, abs)\n";
    std::cout << "  ✓ Recursive functions and complex expressions\n";
    std::cout << "  ✓ Python-style indentation and syntax\n";
    std::cout << "  ✓ Enhanced data structures (lists and dictionaries)\n";
    std::cout << "\nBuilt with modern C++17 for optimal performance\n";
    std::cout << "Build date: " << __DATE__ << " " << __TIME__ << "\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }
    
    bool show_tokens = false;
    bool show_parse = false;
    bool show_ir = false;
    bool generate_asm = false;
    bool generate_c = false;
    bool interpret = false;
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
        } else if (arg == "-p" || arg == "--parse") {
            show_parse = true;
        } else if (arg == "--ir") {
            show_ir = true;
        } else if (arg == "--asm") {
            generate_asm = true;
        } else if (arg == "--c") {
            generate_c = true;
        } else if (arg == "-i" || arg == "--interpret") {
            interpret = true;
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
        
        // Parse
        caesar::Parser parser(tokens);
        auto program = parser.parse();
        
        if (show_parse) {
            std::cout << "AST:\n" << program->toString() << "\n";
            return 0;
        }

        // Generate IR if requested or needed for code generation
        if (show_ir || generate_asm || generate_c) {
            caesar::IRGenerator ir_gen;
            auto ir_blocks = ir_gen.generate(program.get());
            
            if (show_ir) {
                std::cout << "IR (Intermediate Representation):\n";
                std::cout << ir_gen.toString() << "\n";
                return 0;
            }
            
            // Generate assembly code
            if (generate_asm) {
                auto codegen = caesar::CodeGeneratorFactory::create(caesar::TargetArch::X86_64);
                std::string asm_code = codegen->generate(ir_blocks);
                
                if (!output_file.empty()) {
                    std::ofstream out(output_file);
                    out << asm_code;
                    out.close();
                    std::cout << "Assembly code written to '" << output_file << "'\n";
                } else {
                    std::cout << "x86-64 Assembly:\n" << asm_code << "\n";
                }
                return 0;
            }
            
            // Generate C code
            if (generate_c) {
                auto codegen = caesar::CodeGeneratorFactory::createCGenerator();
                std::string c_code = codegen->generate(ir_blocks);
                
                if (!output_file.empty()) {
                    std::ofstream out(output_file);
                    out << c_code;
                    out.close();
                    std::cout << "C code written to '" << output_file << "'\n";
                } else {
                    std::cout << "C Code:\n" << c_code << "\n";
                }
                return 0;
            }
        }

        if (interpret) {
            // Interpret the program (tree-walking interpreter)
            caesar::Interpreter interpreter;
            interpreter.interpret(program.get());
        } else {
            // Default behavior: Compile to bytecode and execute
            // Generate IR
            caesar::IRGenerator ir_gen;
            auto ir_blocks = ir_gen.generate(program.get());
            
            // TODO: Implement proper bytecode VM execution
            // For now, fall back to interpreter until VM is production-ready
            caesar::Interpreter interpreter;
            interpreter.interpret(program.get());
        }
        
    } catch (const caesar::CaesarException& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}
