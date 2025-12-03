/**
 * @file test_c_len.cpp
 * @brief Comprehensive tests for len() built-in function in C code generation
 * @author Caesar Compiler Team
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#ifdef _WIN32
#include <windows.h>
#include <process.h>
#define getpid _getpid
#define popen _popen
#define pclose _pclose
#else
#include <unistd.h>
#include <sys/wait.h>
#endif

const int TOTAL_TESTS = 15;

// Helper function to compile and run Caesar code
bool compileAndRunCaesar(const std::string& code, std::string& output) {
    // Generate unique temp filenames with multiple entropy sources
    srand(time(NULL) ^ getpid() ^ rand());
#ifdef _WIN32
    const char* temp_dir = std::getenv("TEMP");
    if (!temp_dir) temp_dir = ".";
    std::string temp_prefix = std::string(temp_dir) + "\\test_len_" + std::to_string(getpid()) + "_" + std::to_string(rand());
    std::string temp_file = temp_prefix + ".csr";
    std::string c_file = temp_prefix + ".c";
    std::string exe_file = temp_prefix + ".exe";
#else
    std::string temp_file = "/tmp/test_len_" + std::to_string(getpid()) + "_" + 
                           std::to_string(rand()) + ".csr";
    std::string c_file = "/tmp/test_len_" + std::to_string(getpid()) + "_" + 
                        std::to_string(rand()) + ".c";
    std::string exe_file = "/tmp/test_len_" + std::to_string(getpid()) + "_" + 
                          std::to_string(rand()) + ".exe";
#endif
    
    // Write Caesar code to temp file
    std::ofstream ofs(temp_file);
    ofs << code;
    ofs.close();
    
    // Compile Caesar to C
    std::string cmd = "../src/caesar --c " + temp_file + " -o " + c_file + " 2>&1";
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        remove(temp_file.c_str());
        return false;
    }
    pclose(pipe);
    
    // Check if C file was generated
    std::ifstream c_check(c_file);
    if (!c_check.good()) {
        remove(temp_file.c_str());
        return false;
    }
    c_check.close();
    
    // Compile C code with GCC
#ifdef _WIN32
    cmd = "C:\\msys64\\mingw64\\bin\\gcc.exe -o " + exe_file + " " + c_file + " 2>&1";
#else
    cmd = "gcc -o " + exe_file + " " + c_file + " 2>&1";
#endif
    pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        remove(temp_file.c_str());
        remove(c_file.c_str());
        return false;
    }
    
    char buffer[128];
    std::string gcc_output;
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        gcc_output += buffer;
    }
    int status = pclose(pipe);
    
    if (status != 0) {
        // GCC compilation failed
        remove(temp_file.c_str());
        remove(c_file.c_str());
        return false;
    }
    
    // Run the executable
    pipe = popen(exe_file.c_str(), "r");
    if (!pipe) {
        remove(temp_file.c_str());
        remove(c_file.c_str());
        remove(exe_file.c_str());
        return false;
    }
    
    output.clear();
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        output += buffer;
    }
    pclose(pipe);
    
    // Clean up
    remove(temp_file.c_str());
    remove(c_file.c_str());
    remove(exe_file.c_str());
    
    return true;
}

bool test_simple_string() {
    std::string code = R"(
s = "hello"
n = len(s)
print(n)
)";
    std::string output;
    if (!compileAndRunCaesar(code, output)) return false;
    return output == "5\n";
}

bool test_empty_string() {
    std::string code = R"(
s = ""
n = len(s)
print(n)
)";
    std::string output;
    if (!compileAndRunCaesar(code, output)) return false;
    return output == "0\n";
}

bool test_string_with_spaces() {
    std::string code = R"(
s = "hello world"
n = len(s)
print(n)
)";
    std::string output;
    if (!compileAndRunCaesar(code, output)) return false;
    return output == "11\n";
}

bool test_long_string() {
    std::string code = R"(
s = "The quick brown fox jumps over the lazy dog"
n = len(s)
print(n)
)";
    std::string output;
    if (!compileAndRunCaesar(code, output)) return false;
    return output == "43\n";
}

bool test_direct_string_literal() {
    std::string code = R"(
n = len("test")
print(n)
)";
    std::string output;
    if (!compileAndRunCaesar(code, output)) return false;
    return output == "4\n";
}

bool test_multiple_len_calls() {
    std::string code = R"(
a = "cat"
b = "dogs"
n1 = len(a)
n2 = len(b)
print(n1)
print(n2)
)";
    std::string output;
    if (!compileAndRunCaesar(code, output)) return false;
    return output == "3\n4\n";
}

bool test_len_in_arithmetic() {
    std::string code = R"(
s = "hello"
n = len(s) + 10
print(n)
)";
    std::string output;
    if (!compileAndRunCaesar(code, output)) return false;
    return output == "15\n";
}

bool test_len_in_comparison() {
    std::string code = R"(
s = "test"
if len(s) > 3:
    print("long")
else:
    print("short")
)";
    std::string output;
    if (!compileAndRunCaesar(code, output)) return false;
    return output == "long\n";
}

bool test_len_in_loop() {
    std::string code = R"(
s = "abc"
i = 0
while i < len(s):
    i = i + 1
print(i)
)";
    std::string output;
    if (!compileAndRunCaesar(code, output)) return false;
    return output == "3\n";
}

bool test_single_character() {
    std::string code = R"(
s = "x"
n = len(s)
print(n)
)";
    std::string output;
    if (!compileAndRunCaesar(code, output)) return false;
    return output == "1\n";
}

bool test_string_with_numbers() {
    std::string code = R"(
s = "12345"
n = len(s)
print(n)
)";
    std::string output;
    if (!compileAndRunCaesar(code, output)) return false;
    return output == "5\n";
}

bool test_len_result_reused() {
    std::string code = R"(
s = "hello"
n = len(s)
x = n + n
print(x)
)";
    std::string output;
    if (!compileAndRunCaesar(code, output)) return false;
    return output == "10\n";
}

bool test_string_with_punctuation() {
    std::string code = R"(
s = "Hello, World!"
n = len(s)
print(n)
)";
    std::string output;
    if (!compileAndRunCaesar(code, output)) return false;
    return output == "13\n";
}

bool test_nested_expression() {
    std::string code = R"(
s1 = "ab"
s2 = "cde"
n = len(s1) + len(s2)
print(n)
)";
    std::string output;
    if (!compileAndRunCaesar(code, output)) return false;
    return output == "5\n";
}

bool test_len_with_reassignment() {
    std::string code = R"(
s = "short"
n = len(s)
s = "longer string"
m = len(s)
print(n)
print(m)
)";
    std::string output;
    if (!compileAndRunCaesar(code, output)) return false;
    return output == "5\n13\n";
}

int main(int argc, char **argv) {
    int passed = 0;
    int total = TOTAL_TESTS;
    
    std::cout << "Running C len() built-in tests...\n";
    
    // Run all tests
    if (test_simple_string()) { std::cout << "  [PASS] Simple string\n"; passed++; } else { std::cout << "  [FAIL] Simple string\n"; }
    if (test_empty_string()) { std::cout << "  [PASS] Empty string\n"; passed++; } else { std::cout << "  [FAIL] Empty string\n"; }
    if (test_string_with_spaces()) { std::cout << "  [PASS] String with spaces\n"; passed++; } else { std::cout << "  [FAIL] String with spaces\n"; }
    if (test_long_string()) { std::cout << "  [PASS] Long string\n"; passed++; } else { std::cout << "  [FAIL] Long string\n"; }
    if (test_direct_string_literal()) { std::cout << "  [PASS] Direct string literal\n"; passed++; } else { std::cout << "  [FAIL] Direct string literal\n"; }
    if (test_multiple_len_calls()) { std::cout << "  [PASS] Multiple len calls\n"; passed++; } else { std::cout << "  [FAIL] Multiple len calls\n"; }
    if (test_len_in_arithmetic()) { std::cout << "  [PASS] Len in arithmetic\n"; passed++; } else { std::cout << "  [FAIL] Len in arithmetic\n"; }
    if (test_len_in_comparison()) { std::cout << "  [PASS] Len in comparison\n"; passed++; } else { std::cout << "  [FAIL] Len in comparison\n"; }
    if (test_len_in_loop()) { std::cout << "  [PASS] Len in loop\n"; passed++; } else { std::cout << "  [FAIL] Len in loop\n"; }
    if (test_single_character()) { std::cout << "  [PASS] Single character\n"; passed++; } else { std::cout << "  [FAIL] Single character\n"; }
    if (test_string_with_numbers()) { std::cout << "  [PASS] String with numbers\n"; passed++; } else { std::cout << "  [FAIL] String with numbers\n"; }
    if (test_len_result_reused()) { std::cout << "  [PASS] Len result reused\n"; passed++; } else { std::cout << "  [FAIL] Len result reused\n"; }
    if (test_string_with_punctuation()) { std::cout << "  [PASS] String with punctuation\n"; passed++; } else { std::cout << "  [FAIL] String with punctuation\n"; }
    if (test_nested_expression()) { std::cout << "  [PASS] Nested expression\n"; passed++; } else { std::cout << "  [FAIL] Nested expression\n"; }
    if (test_len_with_reassignment()) { std::cout << "  [PASS] Len with reassignment\n"; passed++; } else { std::cout << "  [FAIL] Len with reassignment\n"; }
    
    std::cout << "\nTest Results: " << passed << "/" << total << " tests passed\n";
    
    if (passed != total) {
        std::cout << "Expected " << total << " tests to pass, but only " << passed << " passed!\n";
        return 1;
    }
    
    return 0;
}
