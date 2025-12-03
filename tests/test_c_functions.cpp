/**
 * @file test_c_functions.cpp
 * @brief Comprehensive tests for user-defined functions in C code generation
 */

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>

bool compileAndTest(const std::string& caesar_code, const std::string& test_name, const std::string& expected_output) {
    std::string caesar_file = "/tmp/test_func_" + test_name + ".csr";
    std::string c_file = "/tmp/test_func_" + test_name + ".c";
    std::string exe_file = "/tmp/test_func_" + test_name;
    
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
    
    // Execute and capture output
    std::string run_cmd = exe_file + " > /tmp/test_func_output_" + test_name + ".txt 2>&1";
    system(run_cmd.c_str());
    
    // Read output
    std::ifstream result_file("/tmp/test_func_output_" + test_name + ".txt");
    std::string actual_output((std::istreambuf_iterator<char>(result_file)),
                              std::istreambuf_iterator<char>());
    result_file.close();
    
    // Compare output
    bool passed = (actual_output == expected_output);
    
    // Clean up
    system(("rm -f " + caesar_file + " " + c_file + " " + exe_file + " /tmp/test_func_output_" + test_name + ".txt").c_str());
    
    if (passed) {
        std::cout << "PASS: " << test_name << std::endl;
    } else {
        std::cerr << "FAIL: " << test_name << std::endl;
        std::cerr << "  Expected: " << expected_output << std::endl;
        std::cerr << "  Got:      " << actual_output << std::endl;
    }
    
    return passed;
}

// Test 1: Simple function with two parameters
bool test_simple_add() {
    return compileAndTest(
        "def add(a, b):\n"
        "    return a + b\n"
        "\n"
        "result = add(10, 20)\n"
        "print(result)\n",
        "simple_add",
        "30\n"
    );
}

// Test 2: Function with one parameter
bool test_single_param() {
    return compileAndTest(
        "def double(x):\n"
        "    return x * 2\n"
        "\n"
        "result = double(15)\n"
        "print(result)\n",
        "single_param",
        "30\n"
    );
}

// Test 3: Function with no parameters
bool test_no_params() {
    return compileAndTest(
        "def get_value():\n"
        "    return 42\n"
        "\n"
        "result = get_value()\n"
        "print(result)\n",
        "no_params",
        "42\n"
    );
}

// Test 4: Function with local variables
bool test_local_vars() {
    return compileAndTest(
        "def compute(a, b):\n"
        "    sum = a + b\n"
        "    product = a * b\n"
        "    return sum + product\n"
        "\n"
        "result = compute(5, 3)\n"
        "print(result)\n",
        "local_vars",
        "23\n"  // (5+3) + (5*3) = 8 + 15 = 23
    );
}

// Test 5: Multiple function calls
bool test_multiple_calls() {
    return compileAndTest(
        "def add(a, b):\n"
        "    return a + b\n"
        "\n"
        "r1 = add(10, 5)\n"
        "r2 = add(20, 3)\n"
        "r3 = add(r1, r2)\n"
        "print(r3)\n",
        "multiple_calls",
        "38\n"  // (10+5) + (20+3) = 15 + 23 = 38
    );
}

// Test 6: Recursive function - factorial
bool test_recursive_factorial() {
    return compileAndTest(
        "def factorial(n):\n"
        "    if n <= 1:\n"
        "        return 1\n"
        "    return n * factorial(n - 1)\n"
        "\n"
        "result = factorial(5)\n"
        "print(result)\n",
        "recursive_factorial",
        "120\n"  // 5! = 120
    );
}

// Test 7: Function calling another function
bool test_function_calls_function() {
    return compileAndTest(
        "def add(a, b):\n"
        "    return a + b\n"
        "\n"
        "def add_three(x, y, z):\n"
        "    temp = add(x, y)\n"
        "    return add(temp, z)\n"
        "\n"
        "result = add_three(10, 20, 30)\n"
        "print(result)\n",
        "function_calls_function",
        "60\n"
    );
}

// Test 8: Function with subtraction
bool test_subtract() {
    return compileAndTest(
        "def subtract(a, b):\n"
        "    return a - b\n"
        "\n"
        "result = subtract(50, 20)\n"
        "print(result)\n",
        "subtract",
        "30\n"
    );
}

// Test 9: Function with division
bool test_divide() {
    return compileAndTest(
        "def divide(a, b):\n"
        "    return a / b\n"
        "\n"
        "result = divide(60, 2)\n"
        "print(result)\n",
        "divide",
        "30\n"
    );
}

// Test 10: Function with comparison
bool test_comparison() {
    return compileAndTest(
        "def is_greater(a, b):\n"
        "    if a > b:\n"
        "        return 1\n"
        "    return 0\n"
        "\n"
        "result = is_greater(10, 5)\n"
        "print(result)\n",
        "comparison",
        "1\n"
    );
}

// Test 11: Function returning from if statement
bool test_early_return() {
    return compileAndTest(
        "def check_positive(x):\n"
        "    if x > 0:\n"
        "        return 1\n"
        "    return 0\n"
        "\n"
        "r1 = check_positive(10)\n"
        "r2 = check_positive(-5)\n"
        "print(r1)\n"
        "print(r2)\n",
        "early_return",
        "1\n0\n"
    );
}

// Test 12: Function with loop
bool test_function_with_loop() {
    return compileAndTest(
        "def sum_to_n(n):\n"
        "    total = 0\n"
        "    i = 1\n"
        "    while i <= n:\n"
        "        total = total + i\n"
        "        i = i + 1\n"
        "    return total\n"
        "\n"
        "result = sum_to_n(10)\n"
        "print(result)\n",
        "function_with_loop",
        "55\n"  // 1+2+3+...+10 = 55
    );
}

// Test 13: Function calling built-in
bool test_function_calls_builtin() {
    return compileAndTest(
        "def print_double(x):\n"
        "    result = x * 2\n"
        "    print(result)\n"
        "    return result\n"
        "\n"
        "val = print_double(15)\n"
        "print(val)\n",
        "function_calls_builtin",
        "30\n30\n"
    );
}

// Test 14: Three parameters
bool test_three_params() {
    return compileAndTest(
        "def sum_three(a, b, c):\n"
        "    return a + b + c\n"
        "\n"
        "result = sum_three(10, 20, 30)\n"
        "print(result)\n",
        "three_params",
        "60\n"
    );
}

// Test 15: Multiple functions
bool test_multiple_functions() {
    return compileAndTest(
        "def add(a, b):\n"
        "    return a + b\n"
        "\n"
        "def multiply(a, b):\n"
        "    return a * b\n"
        "\n"
        "def subtract(a, b):\n"
        "    return a - b\n"
        "\n"
        "r1 = add(10, 5)\n"
        "r2 = multiply(3, 4)\n"
        "r3 = subtract(20, 8)\n"
        "result = add(r1, add(r2, r3))\n"
        "print(result)\n",
        "multiple_functions",
        "39\n"  // 15 + (12 + 12) = 39
    );
}

// Test 16: Fibonacci (recursive)
bool test_fibonacci() {
    return compileAndTest(
        "def fib(n):\n"
        "    if n <= 1:\n"
        "        return n\n"
        "    return fib(n - 1) + fib(n - 2)\n"
        "\n"
        "result = fib(7)\n"
        "print(result)\n",
        "fibonacci",
        "13\n"  // fib(7) = 13
    );
}

// Test 17: Function with modulo
bool test_modulo() {
    return compileAndTest(
        "def remainder(a, b):\n"
        "    return a % b\n"
        "\n"
        "result = remainder(17, 5)\n"
        "print(result)\n",
        "modulo",
        "2\n"
    );
}

// Test 18: Nested function calls
bool test_nested_calls() {
    return compileAndTest(
        "def add(a, b):\n"
        "    return a + b\n"
        "\n"
        "def double(x):\n"
        "    return x * 2\n"
        "\n"
        "result = add(double(5), double(10))\n"
        "print(result)\n",
        "nested_calls",
        "30\n"  // double(5) + double(10) = 10 + 20 = 30
    );
}

// Test 19: Function with negative numbers
bool test_negative_numbers() {
    return compileAndTest(
        "def negate(x):\n"
        "    return -x\n"
        "\n"
        "result = negate(30)\n"
        "print(result)\n",
        "negative_numbers",
        "-30\n"
    );
}

// Test 20: Complex expression in function
bool test_complex_expression() {
    return compileAndTest(
        "def compute(a, b, c):\n"
        "    return a * b + c * 2 - a\n"
        "\n"
        "result = compute(5, 4, 10)\n"
        "print(result)\n",
        "complex_expression",
        "35\n"  // 5*4 + 10*2 - 5 = 20 + 20 - 5 = 35
    );
}

int main() {
    int passed = 0;
    int total = 20;
    
    std::cout << "Running function tests..." << std::endl;
    std::cout << "========================================" << std::endl;
    
    if (test_simple_add()) passed++;
    if (test_single_param()) passed++;
    if (test_no_params()) passed++;
    if (test_local_vars()) passed++;
    if (test_multiple_calls()) passed++;
    if (test_recursive_factorial()) passed++;
    if (test_function_calls_function()) passed++;
    if (test_subtract()) passed++;
    if (test_divide()) passed++;
    if (test_comparison()) passed++;
    if (test_early_return()) passed++;
    if (test_function_with_loop()) passed++;
    if (test_function_calls_builtin()) passed++;
    if (test_three_params()) passed++;
    if (test_multiple_functions()) passed++;
    if (test_fibonacci()) passed++;
    if (test_modulo()) passed++;
    if (test_nested_calls()) passed++;
    if (test_negative_numbers()) passed++;
    if (test_complex_expression()) passed++;
    
    std::cout << "========================================" << std::endl;
    std::cout << "Test Results: " << passed << "/" << total << " passed" << std::endl;
    std::cout << "========================================" << std::endl;
    
    return (passed == total) ? 0 : 1;
}
