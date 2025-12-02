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
#include <sys/stat.h>

#ifdef _WIN32
#include <windows.h>
#define PATH_SEP "\\\\"
#else
#include <unistd.h>
#include <linux/limits.h>
#define PATH_SEP "/"
#endif

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

/**
 * @brief Check if a file exists
 */
inline bool fileExists(const std::string& path) {
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}

/**
 * @brief Get executable directory
 */
std::string getExecutableDir() {
#ifdef _WIN32
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);
    std::string fullPath(path);
    size_t pos = fullPath.find_last_of("\\\\/");
    return fullPath.substr(0, pos);
#else
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    std::string fullPath(result, (count > 0) ? count : 0);
    size_t pos = fullPath.find_last_of("/");
    return fullPath.substr(0, pos);
#endif
}

/**
 * @brief Embedded Caesar runtime header
 * This is embedded directly into generated C code when no external
 * header file is found. Allows the compiler to work standalone.
 */
const char* EMBEDDED_CAESAR_RUNTIME = R"(
/* Caesar Runtime Library - Embedded Version */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Basic I/O */
static inline void caesar_print_int(int64_t val) { printf("%lld\n", (long long)val); }
static inline void caesar_print_str(const char* val) { printf("%s\n", val); }
static inline void caesar_print_float(double val) { printf("%g\n", val); }
static inline void caesar_print_bool(bool val) { printf("%s\n", val ? "true" : "false"); }

/* Range Iterator */
typedef struct { int64_t current; int64_t stop; int64_t step; bool done; } CaesarRange;

static inline CaesarRange caesar_range_init(int64_t stop) {
    CaesarRange r; r.current = 0; r.stop = stop; r.step = 1;
    r.done = (0 >= stop); return r;
}

static inline CaesarRange caesar_range_init2(int64_t start, int64_t stop) {
    CaesarRange r; r.current = start; r.stop = stop;
    r.step = (start < stop) ? 1 : -1;
    r.done = (start >= stop && r.step > 0) || (start <= stop && r.step < 0);
    return r;
}

static inline CaesarRange caesar_range_init3(int64_t start, int64_t stop, int64_t step) {
    CaesarRange r; r.current = start; r.stop = stop; r.step = step;
    if (step == 0) { r.done = true; return r; }
    r.done = (step > 0 && start >= stop) || (step < 0 && start <= stop);
    return r;
}

static inline bool caesar_range_has_next(CaesarRange* r) {
    if (r->done) return false;
    if (r->step > 0) return r->current < r->stop;
    else return r->current > r->stop;
}

static inline int64_t caesar_range_next(CaesarRange* r) {
    int64_t val = r->current;
    r->current += r->step;
    return val;
}

/* String Functions */
static inline int64_t caesar_len_str(const char* str) { return (int64_t)strlen(str); }

/* Math Functions */
static inline int64_t caesar_abs_int(int64_t val) { return val < 0 ? -val : val; }
static inline double caesar_abs_float(double val) { return val < 0.0 ? -val : val; }

)";

/**
 * @brief Find Caesar include directory
 * Searches in order:
 * 1. <exe_dir>/../include (development build)
 * 2. <exe_dir>/include (npm/installed)
 * 3. Returns empty string if not found (will use embedded runtime)
 */
std::string findCaesarIncludeDir() {
    std::string exeDir = getExecutableDir();
    
    // Try development layout: bin/../include
    std::string devPath = exeDir + PATH_SEP + ".." + PATH_SEP + "include" + PATH_SEP + "caesar" + PATH_SEP + "caesar_runtime.h";
    if (fileExists(devPath)) {
        return exeDir + PATH_SEP + ".." + PATH_SEP + "include";
    }
    
    // Try npm/installed layout: bin/include  
    std::string npmPath = exeDir + PATH_SEP + "include" + PATH_SEP + "caesar" + PATH_SEP + "caesar_runtime.h";
    if (fileExists(npmPath)) {
        return exeDir + PATH_SEP + "include";
    }
    
    // Not found - will use embedded runtime (works standalone!)
    return "";
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
            
            // Find include directory (searches: dev build, npm, standalone)
            std::string include_dir = findCaesarIncludeDir();
            
            // If no include directory found, embed runtime directly in C code
            if (include_dir.empty()) {
                // Replace #include directive with embedded runtime
                size_t include_pos = c_code.find("#include \"caesar/caesar_runtime.h\"");
                if (include_pos != std::string::npos) {
                    // Remove the include line and replace with embedded runtime
                    size_t include_end = c_code.find('\n', include_pos) + 1;
                    c_code.erase(include_pos, include_end - include_pos);
                    c_code.insert(include_pos, EMBEDDED_CAESAR_RUNTIME);
                }
            }
            
            // Write C code to temporary file
            std::string temp_c_file = "caesar_temp.c";
            std::ofstream c_out(temp_c_file);
            c_out << c_code;
            c_out.close();
            
            // Compile with GCC
            std::string compile_cmd;
            if (!include_dir.empty()) {
                // Use include directory if found
                std::string include_path = "-I\"" + include_dir + "\"";
                compile_cmd = "gcc " + temp_c_file + " " + include_path + " -o " + output_file + " -O2";
            } else {
                // Embedded mode - no include path needed
                compile_cmd = "gcc " + temp_c_file + " -o " + output_file + " -O2";
            }
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
