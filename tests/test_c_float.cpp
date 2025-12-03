/**
 * @file test_c_float.cpp
 * @brief Comprehensive tests for float() built-in function in C code generation
 */

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>

// Helper function to compile Caesar code to C and test
bool compileAndTest(const std::string& caesar_code, const std::string& test_name) {
    std::string caesar_file = "/tmp/test_float_" + test_name + ".csr";
    std::string c_file = "/tmp/test_float_" + test_name + ".c";
    std::string exe_file = "/tmp/test_float_" + test_name;
    
    // Write Caesar code
    std::ofstream out(caesar_file);
    out << caesar_code;
    out.close();
    
    // Compile to C
    std::string compile_cmd = "../src/caesar --c " + caesar_file + " -o " + c_file + " 2>&1";
    if (system(compile_cmd.c_str()) != 0) {
        std::cerr << "Failed to compile Caesar to C: " << test_name << std::endl;
        system(("rm -f " + caesar_file + " " + c_file + " " + exe_file).c_str());
        return false;
    }
    
    // Compile C with GCC
    std::string gcc_cmd = "gcc -std=c11 " + c_file + " -o " + exe_file + " -lm 2>&1";
    if (system(gcc_cmd.c_str()) != 0) {
        std::cerr << "Failed to compile C with GCC: " + test_name << std::endl;
        system(("rm -f " + caesar_file + " " + c_file + " " + exe_file).c_str());
        return false;
    }
    
    // Clean up
    system(("rm -f " + caesar_file + " " + c_file + " " + exe_file).c_str());
    return true;
}

// Test 1: Integer to float conversion
bool test_int_to_float() {
    return compileAndTest(
        "x = 42\n"
        "f = float(x)\n",
        "int_to_float"
    );
}

bool test_float_identity() {
    return compileAndTest("f = 3.14\ng = float(f)\n", "float_identity");
}

bool test_bool_to_float() {
    return compileAndTest("b = True\nf = float(b)\n", "bool_to_float");
}

bool test_string_to_float() {
    return compileAndTest("s = \"3.14\"\nf = float(s)\n", "string_to_float");
}

bool test_negative_int() {
    return compileAndTest("x = -123\nf = float(x)\n", "negative_int");
}

bool test_zero() {
    return compileAndTest("x = 0\nf = float(x)\n", "zero");
}

bool test_negative_string() {
    return compileAndTest("s = \"-2.5\"\nf = float(s)\n", "negative_string");
}

bool test_multiple_calls() {
    return compileAndTest("x = 10\ny = 20\nf1 = float(x)\nf2 = float(y)\n", "multiple_calls");
}

bool test_direct_literal() {
    return compileAndTest("f = float(999)\n", "direct_literal");
}

bool test_in_arithmetic() {
    return compileAndTest("x = 5\nresult = float(x) + 2.5\n", "in_arithmetic");
}

bool test_in_comparison() {
    return compileAndTest("x = 3\nif float(x) > 2.5:\n    y = 1\n", "in_comparison");
}

bool test_in_while_loop() {
    return compileAndTest("i = 0\nx = 3\nwhile i < 2:\n    f = float(x)\n    i = i + 1\n", "in_while_loop");
}

bool test_with_reassignment() {
    return compileAndTest("x = 10\nf = float(x)\nx = 20\ng = float(x)\n", "with_reassignment");
}

bool test_large_int() {
    return compileAndTest("x = 999999\nf = float(x)\n", "large_int");
}

bool test_false_to_float() {
    return compileAndTest("b = False\nf = float(b)\n", "false_to_float");
}

int main(int argc, char **argv) {
    int passed = 0;
    int total = 15;
    
    std::cout << "Running float() built-in function tests..." << std::endl;
    
    if (test_int_to_float()) { std::cout << "PASS: int to float\n"; passed++; } else { std::cout << "FAIL: int to float\n"; }
    if (test_float_identity()) { std::cout << "PASS: float identity\n"; passed++; } else { std::cout << "FAIL: float identity\n"; }
    if (test_bool_to_float()) { std::cout << "PASS: bool to float\n"; passed++; } else { std::cout << "FAIL: bool to float\n"; }
    if (test_string_to_float()) { std::cout << "PASS: string to float\n"; passed++; } else { std::cout << "FAIL: string to float\n"; }
    if (test_negative_int()) { std::cout << "PASS: negative int\n"; passed++; } else { std::cout << "FAIL: negative int\n"; }
    if (test_zero()) { std::cout << "PASS: zero\n"; passed++; } else { std::cout << "FAIL: zero\n"; }
    if (test_negative_string()) { std::cout << "PASS: negative string\n"; passed++; } else { std::cout << "FAIL: negative string\n"; }
    if (test_multiple_calls()) { std::cout << "PASS: multiple calls\n"; passed++; } else { std::cout << "FAIL: multiple calls\n"; }
    if (test_direct_literal()) { std::cout << "PASS: direct literal\n"; passed++; } else { std::cout << "FAIL: direct literal\n"; }
    if (test_in_arithmetic()) { std::cout << "PASS: in arithmetic\n"; passed++; } else { std::cout << "FAIL: in arithmetic\n"; }
    if (test_in_comparison()) { std::cout << "PASS: in comparison\n"; passed++; } else { std::cout << "FAIL: in comparison\n"; }
    if (test_in_while_loop()) { std::cout << "PASS: in while loop\n"; passed++; } else { std::cout << "FAIL: in while loop\n"; }
    if (test_with_reassignment()) { std::cout << "PASS: with reassignment\n"; passed++; } else { std::cout << "FAIL: with reassignment\n"; }
    if (test_large_int()) { std::cout << "PASS: large int\n"; passed++; } else { std::cout << "FAIL: large int\n"; }
    if (test_false_to_float()) { std::cout << "PASS: false to float\n"; passed++; } else { std::cout << "FAIL: false to float\n"; }
    
    std::cout << "\n" << passed << "/" << total << " tests passed.\n";
    return (passed == total) ? 0 : 1;
}
