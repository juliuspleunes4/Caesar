/**
 * @file main.cpp
 * @brief Main entry point for the Caesar compiler
 * @author J.J.G. Pleunes
 * @version 1.5.0
 */

// Define include directory path at compile time
#ifndef CAESAR_INCLUDE_PATH
#define CAESAR_INCLUDE_PATH "../include"
#endif

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
    std::cout << "Caesar Compiler v" << caesar::Version::STRING << "\n";
    std::cout << "Usage: " << program_name << " [options] <input_file>\n\n";
    std::cout << "Default behavior: Compiles source file to native executable\n\n";
    std::cout << "Options:\n";
    std::cout << "  -h, --help         Show this help message\n";
    std::cout << "  -v, --version      Show version information\n";
    std::cout << "  -o, --output FILE  Specify output executable name (default: input name)\n";
    std::cout << "  -i, --interpret    Run using interpreter instead of compiling\n";
    std::cout << "  -t, --tokens       Show tokenization output (debug)\n";
    std::cout << "  -p, --parse        Show parsing output / AST (debug)\n";
    std::cout << "  --ir               Show IR (Intermediate Representation, debug)\n";
    std::cout << "  --asm              Generate assembly code only (no linking)\n";
    std::cout << "  --c                Transpile to C code\n";
    std::cout << "  --obj              Generate object file only (no linking)\n\n";
    std::cout << "Examples:\n";
    std::cout << "  " << program_name << " program.csr                      # Compile to program.exe\n";
    std::cout << "  " << program_name << " program.csr -o myapp.exe         # Compile with custom name\n";
    std::cout << "  " << program_name << " -i program.csr                   # Run with interpreter\n";
    std::cout << "  " << program_name << " --asm program.csr -o output.asm  # Generate assembly\n\n";
    std::cout << "For interactive development, use: caesar_repl\n";
}

void printVersion() {
    std::cout << "Caesar Compiler v" << caesar::Version::STRING << "\n";
    std::cout << "Native compiler for the Caesar programming language\n";
    std::cout << "Python-like syntax with true C-speed performance\n";
    std::cout << "\nLanguage Features:\n";
    std::cout << "  [+] Native compilation to x86-64 executables\n";
    std::cout << "  [+] Functions with default parameters\n";
    std::cout << "  [+] Control flow (if/elif/else, while, for)\n";
    std::cout << "  [+] Loop control (break, continue)\n";
    std::cout << "  [+] Built-in functions (print, range, len, str, int, float, type, abs)\n";
    std::cout << "  [+] Recursive functions and complex expressions\n";
    std::cout << "  [+] Python-style indentation and syntax\n";
    std::cout << "  [+] Enhanced data structures (lists and dictionaries)\n";
    std::cout << "\nCompiler Features:\n";
    std::cout << "  [+] Multi-stage compilation (Lexer -> Parser -> IR -> Codegen)\n";
    std::cout << "  [+] Multiple backends (Native x86-64, C transpiler, Bytecode)\n";
    std::cout << "  [+] Built-in interpreter for rapid development\n";
    std::cout << "\nBuilt with modern C++17\n";
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
    bool generate_obj = false;
    bool interpret = false;
    bool compile_mode = false;  // Will be set to true if no other mode specified
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
        } else if (arg == "--obj") {
            generate_obj = true;
        } else if (arg == "-i" || arg == "--interpret") {
            interpret = true;
        } else if ((arg == "-o" || arg == "--output") && i + 1 < argc) {
            output_file = argv[++i];
        } else if (arg[0] != '-') {
            input_file = arg;
        } else {
            std::cerr << "Unknown option: " << arg << "\n";
            printUsage(argv[0]);
            return 1;
        }
    }
    
    // If no mode specified, default to compilation
    if (!show_tokens && !show_parse && !show_ir && !generate_asm && 
        !generate_c && !generate_obj && !interpret) {
        compile_mode = true;
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

        // Interpret mode
        if (interpret) {
            caesar::Interpreter interpreter;
            interpreter.interpret(program.get());
            return 0;
        }
        
        // Compile mode (default)
        if (compile_mode || generate_obj) {
            // Generate IR
            caesar::IRGenerator ir_gen;
            auto ir_blocks = ir_gen.generate(program.get());
            
            // Generate object file name if not specified
            if (output_file.empty()) {
                // Remove .csr extension and add .exe
                size_t dot_pos = input_file.find_last_of('.');
                if (dot_pos != std::string::npos) {
                    output_file = input_file.substr(0, dot_pos) + ".exe";
                } else {
                    output_file = input_file + ".exe";
                }
            }
            
            std::cout << "Compiling " << input_file << "...\n";
            
            // For now, transpile to C and compile with MinGW
            auto codegen = caesar::CodeGeneratorFactory::createCGenerator();
            std::string c_code = codegen->generate(ir_blocks);
            
            // Write C code to temporary file
            std::string temp_c_file = "caesar_temp.c";
            std::ofstream c_out(temp_c_file);
            c_out << c_code;
            c_out.close();
            
            // Compile with GCC (include runtime header path)
            std::string include_path = "-I\"" + std::string(CAESAR_INCLUDE_PATH) + "\"";
            std::string compile_cmd = "gcc " + temp_c_file + " " + include_path + " -o " + output_file + " -O2";
            int result = system(compile_cmd.c_str());
            
            // Clean up temp file
            remove(temp_c_file.c_str());
            
            if (result == 0) {
                std::cout << "[SUCCESS] Compiled to: " << output_file << "\n";
                std::cout << "Run with: .\\" << output_file << "\n";
            } else {
                std::cerr << "[ERROR] Compilation failed\n";
                return 1;
            }
            
            return 0;
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
