/**
 * @file test_c_str.cpp
 * @brief Comprehensive tests for str() built-in function in C code generation
 * @author Caesar Compiler Team
 * @version 1.5.1
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <unistd.h>

const int TOTAL_TESTS = 15;

// Helper function to run caesar compiler and generate C code
bool compileAndTest(const std::string& caesarCode, const std::string& description) {
    std::cout << "Testing: " << description << std::endl;
    
    // Create unique temp file names
    std::string caesar_file = "/tmp/test_str_" + std::to_string(time(nullptr)) + 
                              "_" + std::to_string(getpid()) + 
                              "_" + std::to_string(rand()) + ".csr";
    std::string c_file = "/tmp/test_str_" + std::to_string(time(nullptr)) + 
                         "_" + std::to_string(getpid()) + 
                         "_" + std::to_string(rand()) + ".c";
    std::string exe_file = "/tmp/test_str_" + std::to_string(time(nullptr)) + 
                           "_" + std::to_string(getpid()) +
                           "_" + std::to_string(rand());
    
    // Write Caesar code to file
    std::ofstream caesar_out(caesar_file);
    if (!caesar_out) {
        std::cerr << "  ERROR: Could not create Caesar file" << std::endl;
        return false;
    }
    caesar_out << caesarCode;
    caesar_out.close();
    
    // Run caesar compiler to generate C code
    std::string cmd = "../src/caesar --c " + caesar_file + " -o " + c_file + " 2>&1";
    int result = system(cmd.c_str());
    
    if (result != 0) {
        std::cerr << "  ERROR: Caesar compilation failed" << std::endl;
        system(("rm -f " + caesar_file + " " + c_file + " " + exe_file).c_str());
        return false;
    }
    
    // Compile the generated C code with gcc
    cmd = "gcc -std=c11 -o " + exe_file + " " + c_file + " 2>&1";
    result = system(cmd.c_str());
    
    // Clean up
    system(("rm -f " + caesar_file + " " + c_file + " " + exe_file).c_str());
    
    if (result != 0) {
        std::cerr << "  ERROR: GCC compilation failed" << std::endl;
        return false;
    }
    
    std::cout << "  ✓ Test passed" << std::endl;
    return true;
}

int main(int argc, char **argv) {
    srand(time(nullptr) ^ getpid());
    
    std::cout << "========================================" << std::endl;
    std::cout << "C Code Generation - str() Built-in Tests" << std::endl;
    std::cout << "========================================" << std::endl;
    
    int passed = 0;
    int failed = 0;
    
    // Test 1: str() with integer
    if (compileAndTest(R"(
x = 42
s = str(x)
print(s)
)", "str() with integer")) {
        passed++;
    } else {
        failed++;
    }
    
    // Test 2: str() with negative integer
    if (compileAndTest(R"(
x = -123
s = str(x)
print(s)
)", "str() with negative integer")) {
        passed++;
    } else {
        failed++;
    }
    
    // Test 3: str() with zero
    if (compileAndTest(R"(
x = 0
s = str(x)
print(s)
)", "str() with zero")) {
        passed++;
    } else {
        failed++;
    }
    
    // Test 4: str() with float
    if (compileAndTest(R"(
x = 3.14
s = str(x)
print(s)
)", "str() with float")) {
        passed++;
    } else {
        failed++;
    }
    
    // Test 5: str() with negative float
    if (compileAndTest(R"(
x = -2.5
s = str(x)
print(s)
)", "str() with negative float")) {
        passed++;
    } else {
        failed++;
    }
    
    // Test 6: str() with boolean True
    if (compileAndTest(R"(
x = True
s = str(x)
print(s)
)", "str() with boolean True")) {
        passed++;
    } else {
        failed++;
    }
    
    // Test 7: str() with boolean False
    if (compileAndTest(R"(
x = False
s = str(x)
print(s)
)", "str() with boolean False")) {
        passed++;
    } else {
        failed++;
    }
    
    // Test 8: str() with string (identity)
    if (compileAndTest(R"(
x = "hello"
s = str(x)
print(s)
)", "str() with string")) {
        passed++;
    } else {
        failed++;
    }
    
    // Test 9: Multiple str() calls
    if (compileAndTest(R"(
a = 10
b = 20
s1 = str(a)
s2 = str(b)
print(s1)
print(s2)
)", "Multiple str() calls")) {
        passed++;
    } else {
        failed++;
    }
    
    // Test 10: str() with direct literal
    if (compileAndTest(R"(
s = str(999)
print(s)
)", "str() with direct integer literal")) {
        passed++;
    } else {
        failed++;
    }
    
    // Test 11: str() result used in print
    if (compileAndTest(R"(
x = 42
print("Value:", str(x))
)", "str() result used in print")) {
        passed++;
    } else {
        failed++;
    }
    
    // Test 12: str() with large integer
    if (compileAndTest(R"(
x = 999999
s = str(x)
print(s)
)", "str() with large integer")) {
        passed++;
    } else {
        failed++;
    }
    
    // Test 13: str() with very small float
    if (compileAndTest(R"(
x = 0.0001
s = str(x)
print(s)
)", "str() with very small float")) {
        passed++;
    } else {
        failed++;
    }
    
    // Test 14: str() in loop
    if (compileAndTest(R"(
i = 0
while i < 3:
    s = str(i)
    print(s)
    i = i + 1
)", "str() in while loop")) {
        passed++;
    } else {
        failed++;
    }
    
    // Test 15: str() with reassignment
    if (compileAndTest(R"(
x = 10
s = str(x)
print(s)
x = 20
s = str(x)
print(s)
)", "str() with reassignment")) {
        passed++;
    } else {
        failed++;
    }
    
    // Summary
    std::cout << "\n========================================" << std::endl;
    std::cout << "Test Summary" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Passed: " << passed << "/" << TOTAL_TESTS << std::endl;
    std::cout << "Failed: " << failed << "/" << TOTAL_TESTS << std::endl;
    
    if (passed != TOTAL_TESTS) {
        std::cerr << "\nWARNING: Expected " << TOTAL_TESTS << " tests but counted " << (passed + failed) << " tests!" << std::endl;
    }
    
    return (failed == 0) ? 0 : 1;
}
