/**
 * @file test_c_int.cpp
 * @brief Comprehensive tests for int() built-in function in C code generation
 */

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <sstream>

// Helper function to compile Caesar to C and test the result
bool compileAndTest(const std::string& caesar_code, const std::string& test_name) {
    std::string caesar_file = "/tmp/test_int_" + test_name + ".csr";
    std::string c_file = "/tmp/test_int_" + test_name + ".c";
    std::string exe_file = "/tmp/test_int_" + test_name;
    
    // Write Caesar code to file
    std::ofstream out(caesar_file);
    out << caesar_code;
    out.close();
    
    // Compile to C
    std::string cmd = "../src/caesar --c " + caesar_file + " -o " + c_file + " 2>/dev/null";
    if (system(cmd.c_str()) != 0) {
        std::cerr << "Failed to compile " << test_name << " to C" << std::endl;
        system(("rm -f " + caesar_file + " " + c_file + " " + exe_file).c_str());
        return false;
    }
    
    // Compile C code with GCC
    cmd = "gcc -std=c11 " + c_file + " -o " + exe_file + " -lm 2>/dev/null";
    if (system(cmd.c_str()) != 0) {
        std::cerr << "Failed to compile " << test_name << " C code with GCC" << std::endl;
        system(("rm -f " + caesar_file + " " + c_file + " " + exe_file).c_str());
        return false;
    }
    
    // Clean up
    system(("rm -f " + caesar_file + " " + c_file + " " + exe_file).c_str());
    
    return true;
}

int main(int argc, char **argv) {
    int passed = 0;
    int total = 0;
    
    // Test 1: Convert integer (identity)
    total++;
    if (compileAndTest("x = 42\nn = int(x)\nprint(n)", "int_identity")) {
        std::cout << "✓ Test 1 passed: int() with integer (identity)" << std::endl;
        passed++;
    } else {
        std::cout << "✗ Test 1 failed: int() with integer (identity)" << std::endl;
    }
    
    // Test 2: Convert positive float (truncate)
    total++;
    if (compileAndTest("x = 3.14\nn = int(x)\nprint(n)", "float_positive")) {
        std::cout << "✓ Test 2 passed: int() with positive float" << std::endl;
        passed++;
    } else {
        std::cout << "✗ Test 2 failed: int() with positive float" << std::endl;
    }
    
    // Test 3: Convert negative float
    total++;
    if (compileAndTest("x = -2.9\nn = int(x)\nprint(n)", "float_negative")) {
        std::cout << "✓ Test 3 passed: int() with negative float" << std::endl;
        passed++;
    } else {
        std::cout << "✗ Test 3 failed: int() with negative float" << std::endl;
    }
    
    // Test 4: Convert boolean True
    total++;
    if (compileAndTest("x = True\nn = int(x)\nprint(n)", "bool_true")) {
        std::cout << "✓ Test 4 passed: int() with True" << std::endl;
        passed++;
    } else {
        std::cout << "✗ Test 4 failed: int() with True" << std::endl;
    }
    
    // Test 5: Convert boolean False
    total++;
    if (compileAndTest("x = False\nn = int(x)\nprint(n)", "bool_false")) {
        std::cout << "✓ Test 5 passed: int() with False" << std::endl;
        passed++;
    } else {
        std::cout << "✗ Test 5 failed: int() with False" << std::endl;
    }
    
    // Test 6: Convert string "42"
    total++;
    if (compileAndTest("s = \"42\"\nn = int(s)\nprint(n)", "string_positive")) {
        std::cout << "✓ Test 6 passed: int() with string \"42\"" << std::endl;
        passed++;
    } else {
        std::cout << "✗ Test 6 failed: int() with string \"42\"" << std::endl;
    }
    
    // Test 7: Convert string "-123"
    total++;
    if (compileAndTest("s = \"-123\"\nn = int(s)\nprint(n)", "string_negative")) {
        std::cout << "✓ Test 7 passed: int() with string \"-123\"" << std::endl;
        passed++;
    } else {
        std::cout << "✗ Test 7 failed: int() with string \"-123\"" << std::endl;
    }
    
    // Test 8: Convert string "0"
    total++;
    if (compileAndTest("s = \"0\"\nn = int(s)\nprint(n)", "string_zero")) {
        std::cout << "✓ Test 8 passed: int() with string \"0\"" << std::endl;
        passed++;
    } else {
        std::cout << "✗ Test 8 failed: int() with string \"0\"" << std::endl;
    }
    
    // Test 9: Convert zero float
    total++;
    if (compileAndTest("x = 0.0\nn = int(x)\nprint(n)", "float_zero")) {
        std::cout << "✓ Test 9 passed: int() with zero float" << std::endl;
        passed++;
    } else {
        std::cout << "✗ Test 9 failed: int() with zero float" << std::endl;
    }
    
    // Test 10: Multiple int() calls
    total++;
    if (compileAndTest("x = 3.5\ny = 7.8\na = int(x)\nb = int(y)\nprint(a)\nprint(b)", "multiple_calls")) {
        std::cout << "✓ Test 10 passed: Multiple int() calls" << std::endl;
        passed++;
    } else {
        std::cout << "✗ Test 10 failed: Multiple int() calls" << std::endl;
    }
    
    // Test 11: int() with direct literal
    total++;
    if (compileAndTest("n = int(99)\nprint(n)", "direct_literal")) {
        std::cout << "✓ Test 11 passed: int() with direct literal" << std::endl;
        passed++;
    } else {
        std::cout << "✗ Test 11 failed: int() with direct literal" << std::endl;
    }
    
    // Test 12: int() in arithmetic expression
    total++;
    if (compileAndTest("x = 3.7\nn = int(x) + 10\nprint(n)", "in_arithmetic")) {
        std::cout << "✓ Test 12 passed: int() in arithmetic expression" << std::endl;
        passed++;
    } else {
        std::cout << "✗ Test 12 failed: int() in arithmetic expression" << std::endl;
    }
    
    // Test 13: int() in comparison
    total++;
    if (compileAndTest("x = 3.9\nif int(x) > 2:\n    print(42)", "in_comparison")) {
        std::cout << "✓ Test 13 passed: int() in comparison" << std::endl;
        passed++;
    } else {
        std::cout << "✗ Test 13 failed: int() in comparison" << std::endl;
    }
    
    // Test 14: int() in while loop
    total++;
    if (compileAndTest("x = 3.5\ni = 0\nwhile i < int(x):\n    print(i)\n    i = i + 1", "in_while_loop")) {
        std::cout << "✓ Test 14 passed: int() in while loop" << std::endl;
        passed++;
    } else {
        std::cout << "✗ Test 14 failed: int() in while loop" << std::endl;
    }
    
    // Test 15: int() with reassignment
    total++;
    if (compileAndTest("x = 2.5\ny = int(x)\nx = 8.9\nz = int(x)\nprint(y)\nprint(z)", "with_reassignment")) {
        std::cout << "✓ Test 15 passed: int() with reassignment" << std::endl;
        passed++;
    } else {
        std::cout << "✗ Test 15 failed: int() with reassignment" << std::endl;
    }
    
    std::cout << "\n========================================" << std::endl;
    std::cout << "int() built-in tests: " << passed << "/" << total << " passed" << std::endl;
    std::cout << "========================================\n" << std::endl;
    
    return (passed == total) ? 0 : 1;
}
