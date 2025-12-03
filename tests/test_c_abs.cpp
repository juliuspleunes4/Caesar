/**
 * @file test_c_abs.cpp
 * @brief Comprehensive tests for abs() built-in function in C code generation
 */

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>

// Helper function to compile Caesar code to C and test
bool compileAndTest(const std::string& caesar_code, const std::string& test_name) {
    std::string caesar_file = "/tmp/test_abs_" + test_name + ".csr";
    std::string c_file = "/tmp/test_abs_" + test_name + ".c";
    std::string exe_file = "/tmp/test_abs_" + test_name;
    
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
        std::cerr << "Failed to compile C with GCC: " << test_name << std::endl;
        system(("rm -f " + caesar_file + " " + c_file + " " + exe_file).c_str());
        return false;
    }
    
    // Clean up
    system(("rm -f " + caesar_file + " " + c_file + " " + exe_file).c_str());
    return true;
}

bool test_positive_int() {
    return compileAndTest("x = 42\na = abs(x)\n", "positive_int");
}

bool test_negative_int() {
    return compileAndTest("x = -123\na = abs(x)\n", "negative_int");
}

bool test_zero() {
    return compileAndTest("x = 0\na = abs(x)\n", "zero");
}

bool test_positive_float() {
    return compileAndTest("f = 3.14\na = abs(f)\n", "positive_float");
}

bool test_negative_float() {
    return compileAndTest("f = -2.5\na = abs(f)\n", "negative_float");
}

bool test_zero_float() {
    return compileAndTest("f = 0.0\na = abs(f)\n", "zero_float");
}

bool test_bool_true() {
    return compileAndTest("b = True\na = abs(b)\n", "bool_true");
}

bool test_bool_false() {
    return compileAndTest("b = False\na = abs(b)\n", "bool_false");
}

bool test_multiple_calls() {
    return compileAndTest("x = -10\ny = 20\na1 = abs(x)\na2 = abs(y)\n", "multiple_calls");
}

bool test_direct_literal() {
    return compileAndTest("a = abs(-999)\n", "direct_literal");
}

bool test_in_arithmetic() {
    return compileAndTest("x = -5\nresult = abs(x) + 10\n", "in_arithmetic");
}

bool test_in_comparison() {
    return compileAndTest("x = -3\nif abs(x) > 2:\n    y = 1\n", "in_comparison");
}

bool test_in_while_loop() {
    return compileAndTest("i = 0\nx = -3\nwhile i < 2:\n    a = abs(x)\n    i = i + 1\n", "in_while_loop");
}

bool test_with_reassignment() {
    return compileAndTest("x = -10\na = abs(x)\nx = 20\nb = abs(x)\n", "with_reassignment");
}

bool test_large_negative() {
    return compileAndTest("x = -999999\na = abs(x)\n", "large_negative");
}

int main(int argc, char **argv) {
    int passed = 0;
    int total = 15;
    
    std::cout << "Running abs() built-in function tests..." << std::endl;
    
    if (test_positive_int()) { std::cout << "PASS: positive int\n"; passed++; } else { std::cout << "FAIL: positive int\n"; }
    if (test_negative_int()) { std::cout << "PASS: negative int\n"; passed++; } else { std::cout << "FAIL: negative int\n"; }
    if (test_zero()) { std::cout << "PASS: zero\n"; passed++; } else { std::cout << "FAIL: zero\n"; }
    if (test_positive_float()) { std::cout << "PASS: positive float\n"; passed++; } else { std::cout << "FAIL: positive float\n"; }
    if (test_negative_float()) { std::cout << "PASS: negative float\n"; passed++; } else { std::cout << "FAIL: negative float\n"; }
    if (test_zero_float()) { std::cout << "PASS: zero float\n"; passed++; } else { std::cout << "FAIL: zero float\n"; }
    if (test_bool_true()) { std::cout << "PASS: bool true\n"; passed++; } else { std::cout << "FAIL: bool true\n"; }
    if (test_bool_false()) { std::cout << "PASS: bool false\n"; passed++; } else { std::cout << "FAIL: bool false\n"; }
    if (test_multiple_calls()) { std::cout << "PASS: multiple calls\n"; passed++; } else { std::cout << "FAIL: multiple calls\n"; }
    if (test_direct_literal()) { std::cout << "PASS: direct literal\n"; passed++; } else { std::cout << "FAIL: direct literal\n"; }
    if (test_in_arithmetic()) { std::cout << "PASS: in arithmetic\n"; passed++; } else { std::cout << "FAIL: in arithmetic\n"; }
    if (test_in_comparison()) { std::cout << "PASS: in comparison\n"; passed++; } else { std::cout << "FAIL: in comparison\n"; }
    if (test_in_while_loop()) { std::cout << "PASS: in while loop\n"; passed++; } else { std::cout << "FAIL: in while loop\n"; }
    if (test_with_reassignment()) { std::cout << "PASS: with reassignment\n"; passed++; } else { std::cout << "FAIL: with reassignment\n"; }
    if (test_large_negative()) { std::cout << "PASS: large negative\n"; passed++; } else { std::cout << "FAIL: large negative\n"; }
    
    std::cout << "\n" << passed << "/" << total << " tests passed.\n";
    return (passed == total) ? 0 : 1;
}
