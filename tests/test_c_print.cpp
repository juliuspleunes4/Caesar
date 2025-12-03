#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <sys/wait.h>

const int TOTAL_TESTS = 15;

bool runTest(const std::string& caesar_code, const std::string& test_name, const std::string& expected_output) {
    // Generate unique temp filenames using multiple entropy sources
    std::string temp_prefix = "/tmp/caesar_print_test_" +
                            std::to_string(time(nullptr)) + "_" +
                            std::to_string(getpid()) + "_" +
                            std::to_string(rand());
    
    std::string csr_file = temp_prefix + ".csr";
    std::string c_file = temp_prefix + ".c";
    std::string exec_file = temp_prefix + "_exec";
    
    // Write Caesar code to file
    std::ofstream csr_out(csr_file);
    csr_out << caesar_code;
    csr_out.close();
    
    // Generate C code  
    std::string caesar_path = "../src/caesar";  // From build/tests to build/src
    std::string cmd = caesar_path + " --c " + csr_file + " -o " + c_file + " 2>&1";
    int result = system(cmd.c_str());
    if (result != 0) {
        std::cerr << "✗ " << test_name << ": Failed to generate C code\n";
        remove(csr_file.c_str());
        return false;
    }
    
    // Compile C code
    cmd = "gcc -I/home/runner/work/Caesar/Caesar " + c_file + " -o " + exec_file + " 2>&1";
    result = system(cmd.c_str());
    if (result != 0) {
        std::cerr << "✗ " << test_name << ": Failed to compile C code\n";
        remove(csr_file.c_str());
        remove(c_file.c_str());
        return false;
    }
    
    // Execute and capture output
    cmd = exec_file + " 2>&1";
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        std::cerr << "✗ " << test_name << ": Failed to execute\n";
        remove(csr_file.c_str());
        remove(c_file.c_str());
        remove(exec_file.c_str());
        return false;
    }
    
    std::string output;
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        output += buffer;
    }
    pclose(pipe);
    
    // Remove trailing newline from output for comparison
    if (!output.empty() && output.back() == '\n') {
        output.pop_back();
    }
    
    // Clean up temp files
    remove(csr_file.c_str());
    remove(c_file.c_str());
    remove(exec_file.c_str());
    
    // Check output
    if (output == expected_output) {
        std::cout << "✓ " << test_name << "\n";
        return true;
    } else {
        std::cerr << "✗ " << test_name << ": Expected '" << expected_output 
                  << "' but got '" << output << "'\n";
        return false;
    }
}

int main() {
    srand(time(nullptr) ^ getpid());
    int passed = 0;
    
    std::cout << "Running Caesar print() built-in function tests...\n\n";
    
    // Test 1: Print single integer
    if (runTest("print(42)", "Print single integer", "42")) passed++;
    
    // Test 2: Print single float
    if (runTest("print(3.14)", "Print single float", "3.14")) passed++;
    
    // Test 3: Print single string
    if (runTest("print(\"Hello\")", "Print single string", "Hello")) passed++;
    
    // Test 4: Print single boolean (True)
    if (runTest("print(True)", "Print boolean True", "True")) passed++;
    
    // Test 5: Print single boolean (False)
    if (runTest("print(False)", "Print boolean False", "False")) passed++;
    
    // Test 6: Print multiple arguments (2 args)
    if (runTest("print(\"Value:\", 42)", "Print 2 arguments", "Value: 42")) passed++;
    
    // Test 7: Print multiple arguments (3 args mixed types)
    if (runTest("print(\"Answer:\", 42, \"is correct\")", "Print 3 mixed arguments", 
                "Answer: 42 is correct")) passed++;
    
    // Test 8: Print multiple arguments (5 args)
    if (runTest("print(1, 2, 3, 4, 5)", "Print 5 integers", "1 2 3 4 5")) passed++;
    
    // Test 9: Print with variable
    if (runTest("x = 100\nprint(x)", "Print variable", "100")) passed++;
    
    // Test 10: Print in for loop
    if (runTest("for i in range(3):\n    print(i)", "Print in for loop", "0\n1\n2")) passed++;
    
    // Test 11: Print in while loop
    if (runTest("i = 0\nwhile i < 3:\n    print(i)\n    i = i + 1", 
                "Print in while loop", "0\n1\n2")) passed++;
    
    // Test 12: Print mixed types
    if (runTest("print(10, 3.14, \"text\", True)", "Print mixed types", "10 3.14 text True")) passed++;
    
    // Test 13: Print with arithmetic expression
    if (runTest("print(5 + 3)", "Print arithmetic expression", "8")) passed++;
    
    // Test 14: Print negative numbers
    if (runTest("print(-42, -3.14)", "Print negative numbers", "-42 -3.14")) passed++;
    
    // Test 15: Print zero
    if (runTest("print(0, 0.0)", "Print zeros", "0 0")) passed++;
    
    std::cout << "\n" << passed << "/" << TOTAL_TESTS << " tests passed\n";
    
    if (passed == TOTAL_TESTS) {
        std::cout << "All print() tests passed!\n";
        return 0;
    } else {
        std::cerr << (TOTAL_TESTS - passed) << " test(s) failed\n";
        return 1;
    }
}
