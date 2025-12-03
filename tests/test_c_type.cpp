/**
 * @file test_c_type.cpp
 * @brief Comprehensive tests for type() built-in function in C code generation
 */

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>

bool compileAndTest(const std::string& caesar_code, const std::string& test_name) {
    std::string caesar_file = "/tmp/test_type_" + test_name + ".csr";
    std::string c_file = "/tmp/test_type_" + test_name + ".c";
    std::string exe_file = "/tmp/test_type_" + test_name;
    
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
    
    std::cout << "PASS: " << test_name << std::endl;
    return true;
}

bool test_type_of_integer() {
    return compileAndTest("x = 42\nt = type(x)\nprint(t)\n", "type_of_integer");
}

bool test_type_of_float() {
    return compileAndTest("x = 3.14\nt = type(x)\nprint(t)\n", "type_of_float");
}

bool test_type_of_string() {
    return compileAndTest("x = \"hello\"\nt = type(x)\nprint(t)\n", "type_of_string");
}

bool test_type_of_boolean_true() {
    return compileAndTest("x = True\nt = type(x)\nprint(t)\n", "type_of_boolean_true");
}

bool test_type_of_boolean_false() {
    return compileAndTest("x = False\nt = type(x)\nprint(t)\n", "type_of_boolean_false");
}

bool test_type_direct_literal() {
    return compileAndTest("print(type(999))\n", "type_direct_literal");
}

bool test_type_nested_in_print() {
    return compileAndTest("x = 42\nprint(\"Type is:\", type(x))\n", "type_nested_in_print");
}

bool test_multiple_type_calls() {
    return compileAndTest("x = 10\ny = 3.14\nt1 = type(x)\nt2 = type(y)\nprint(t1)\nprint(t2)\n", "multiple_type_calls");
}

bool test_type_in_comparison() {
    return compileAndTest("x = 42\nif type(x) == \"int\":\n    y = 1\n", "type_in_comparison");
}

bool test_type_in_while_loop() {
    return compileAndTest("i = 0\nwhile i < 2:\n    t = type(i)\n    i = i + 1\n", "type_in_while_loop");
}

bool test_type_of_zero() {
    return compileAndTest("x = 0\nprint(type(x))\n", "type_of_zero");
}

bool test_type_of_negative() {
    return compileAndTest("x = -42\nprint(type(x))\n", "type_of_negative");
}

bool test_type_with_reassignment() {
    return compileAndTest("x = 10\nt1 = type(x)\nx = 3.14\nt2 = type(x)\n", "type_with_reassignment");
}

bool test_type_of_empty_string() {
    return compileAndTest("s = \"\"\nprint(type(s))\n", "type_of_empty_string");
}

bool test_type_multiple_in_expression() {
    return compileAndTest("x = 42\ny = \"hello\"\nprint(type(x), type(y))\n", "type_multiple_in_expression");
}

int main() {
    int passed = 0;
    int total = 15;
    
    if (test_type_of_integer()) passed++;
    if (test_type_of_float()) passed++;
    if (test_type_of_string()) passed++;
    if (test_type_of_boolean_true()) passed++;
    if (test_type_of_boolean_false()) passed++;
    if (test_type_direct_literal()) passed++;
    if (test_type_nested_in_print()) passed++;
    if (test_multiple_type_calls()) passed++;
    if (test_type_in_comparison()) passed++;
    if (test_type_in_while_loop()) passed++;
    if (test_type_of_zero()) passed++;
    if (test_type_of_negative()) passed++;
    if (test_type_with_reassignment()) passed++;
    if (test_type_of_empty_string()) passed++;
    if (test_type_multiple_in_expression()) passed++;
    
    std::cout << "\n========================================" << std::endl;
    std::cout << "Test Results: " << passed << "/" << total << " passed" << std::endl;
    std::cout << "========================================" << std::endl;
    
    return (passed == total) ? 0 : 1;
}
