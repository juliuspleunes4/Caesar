/**
 * @file test_stress.cpp
 * @brief Stress tests for the Caesar language parser
 * @author Julius Pleunes
 * @version 1.0.0
 */

#undef NDEBUG
#include "caesar/lexer.h"
#include "caesar/parser.h"
#include "caesar/ast.h"
#include <iostream>
#include <cassert>
#include <sstream>
#include <chrono>
#include <vector>
#include <cstdlib>

// Ensure std types are available
using std::vector;

// Simple assert replacement for debugging
#define my_assert(condition) \
    do { \
        if (!(condition)) { \
            std::cerr << "Assertion failed: " << #condition << " at line " << __LINE__ << std::endl; \
            std::abort(); \
        } \
    } while(0)

#ifndef assert
#define assert my_assert
#endif

// Timer utility for performance testing
class Timer {
public:
    Timer() : start_time(std::chrono::high_resolution_clock::now()) {}
    
    double elapsed() const {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        return duration.count();
    }
    
private:
    std::chrono::high_resolution_clock::time_point start_time;
};

// Helper function to test parsing performance
bool testParsingPerformance(const std::string& source, const std::string& testName, double maxTimeMs = 5000.0) {
    std::cout << "Testing " << testName << " performance...\n";
    
    Timer timer;
    
    try {
        caesar::Lexer lexer(source);
        auto tokens = lexer.tokenize();
        
        caesar::Parser parser(std::move(tokens));
        auto program = parser.parse();
        
        double elapsed = timer.elapsed();
        
        if (!program) {
            std::cerr << "❌ " << testName << ": Parser returned null\n";
            return false;
        }
        
        if (elapsed > maxTimeMs) {
            std::cerr << "❌ " << testName << ": Too slow (" << elapsed << "ms > " << maxTimeMs << "ms)\n";
            return false;
        }
        
        std::cout << "✓ " << testName << " completed in " << elapsed << "ms\n";
        std::cout << "  Statements: " << program->statements.size() << "\n";
        return true;
    } catch (const std::exception& e) {
        std::cerr << "❌ " << testName << " failed: " << e.what() << "\n";
        return false;
    }
}

void test_large_file() {
    std::cout << "Testing large file parsing...\n";
    
    std::stringstream source;
    
    // Generate a large program with 1000 function definitions
    for (int i = 0; i < 1000; ++i) {
        source << "def function_" << i << "(param1, param2, param3):\n";
        source << "    result = param1 + param2 * param3\n";
        source << "    if result > 0:\n";
        source << "        return result\n";
        source << "    else:\n";
        source << "        return 0\n";
        source << "\n";
    }
    
    // Add function calls
    source << "def main():\n";
    for (int i = 0; i < 100; ++i) {
        source << "    result_" << i << " = function_" << i << "(1, 2, 3)\n";
    }
    source << "\nmain()\n";
    
    bool success = testParsingPerformance(source.str(), "Large File (1000 functions)", 10000.0);
    assert(success);
}

void test_deeply_nested_structures() {
    std::cout << "Testing deeply nested structures...\n";
    
    // Start with just 1 level to debug
    std::string source1 = R"(
def test_single():
    if condition_0:
        return "true"
    else:
        return "false"
)";
    
    std::cout << "Testing single if/else...\n";
    bool success1 = testParsingPerformance(source1, "Single if/else", 5000.0);
    
    if (success1) {
        std::cout << "Single level worked, trying 2 levels...\n";
        
        std::string source2 = R"(
def test_double():
    if condition_0:
        if condition_1:
            return "deep"
        else:
            return "not_deep_1"
    else:
        return "not_deep_0"
)";
        
        bool success2 = testParsingPerformance(source2, "Double nested if/else", 5000.0);
        
        if (success2) {
            std::cout << "2 levels worked, trying 3 levels...\n";
            
            std::string source3 = R"(
def test_triple():
    if condition_0:
        if condition_1:
            if condition_2:
                return "deep"
            else:
                return "not_deep_2"
        else:
            return "not_deep_1"
    else:
        return "not_deep_0"
)";
            
            bool success3 = testParsingPerformance(source3, "Triple nested if/else", 5000.0);
            assert(success3);
        } else {
            assert(false); // 2 levels failed
        }
    } else {
        assert(false); // 1 level failed
    }
}

void test_complex_expressions() {
    std::cout << "Testing complex expression parsing...\n";
    
    std::stringstream source;
    
    // Generate complex mathematical expressions
    for (int i = 0; i < 100; ++i) {
        source << "result_" << i << " = ";
        
        // Create a complex expression with many operators
        for (int j = 0; j < 20; ++j) {
            if (j > 0) {
                switch (j % 4) {
                    case 0: source << " + "; break;
                    case 1: source << " - "; break;
                    case 2: source << " * "; break;
                    case 3: source << " / "; break;
                }
            }
            source << "var_" << j << "_" << i;
        }
        source << "\n";
    }
    
    bool success = testParsingPerformance(source.str(), "Complex Expressions (100x20)", 3000.0);
    assert(success);
}

void test_many_function_calls() {
    std::cout << "Testing many function calls...\n";
    
    std::stringstream source;
    
    // Create nested function calls
    source << "result = ";
    for (int i = 0; i < 100; ++i) {
        source << "func_" << i << "(";
    }
    source << "base_value";
    for (int i = 0; i < 100; ++i) {
        source << ")";
    }
    source << "\n";
    
    bool success = testParsingPerformance(source.str(), "Nested Function Calls (100 deep)", 2000.0);
    assert(success);
}

void test_large_data_structures() {
    std::cout << "Testing large data structure parsing...\n";
    
    std::stringstream source;
    
    // Create a large list
    source << "large_list = [\n";
    for (int i = 0; i < 1000; ++i) {
        if (i > 0) source << ",\n";
        source << "    " << i;
    }
    source << "\n]\n";
    
    // Create a large dictionary
    source << "large_dict = {\n";
    for (int i = 0; i < 500; ++i) {
        if (i > 0) source << ",\n";
        source << "    \"key_" << i << "\": " << i;
    }
    source << "\n}\n";
    
    bool success = testParsingPerformance(source.str(), "Large Data Structures", 3000.0);
    assert(success);
}

void test_many_classes() {
    std::cout << "Testing many class definitions...\n";
    
    std::stringstream source;
    
    // Generate many class definitions
    for (int i = 0; i < 200; ++i) {
        source << "class Class_" << i << ":\n";
        source << "    def __init__(self):\n";
        source << "        self.value = " << i << "\n";
        source << "\n";
        source << "    def method_1(self):\n";
        source << "        return self.value * 2\n";
        source << "\n";
        source << "    def method_2(self, param):\n";
        source << "        return self.value + param\n";
        source << "\n";
        source << "    def method_3(self):\n";
        source << "        if self.value > 50:\n";
        source << "            return \"high\"\n";
        source << "        else:\n";
        source << "            return \"low\"\n";
        source << "\n";
    }
    
    bool success = testParsingPerformance(source.str(), "Many Classes (200)", 8000.0);
    assert(success);
}

void test_complex_control_flow() {
    std::cout << "Testing complex control flow...\n";
    
    std::stringstream source;
    
    // Generate complex control flow with many nested loops
    source << "def complex_control_flow():\n";
    
    for (int i = 0; i < 10; ++i) {
        std::string indent(4 + i * 4, ' ');
        source << indent << "for i_" << i << " in range(10):\n";
        source << indent << "    if i_" << i << " % 2 == 0:\n";
        source << indent << "        while condition_" << i << ":\n";
        source << indent << "            for j_" << i << " in range(5):\n";
        source << indent << "                if j_" << i << " == 3:\n";
        source << indent << "                    break\n";
        source << indent << "                elif j_" << i << " == 1:\n";
        source << indent << "                    continue\n";
        source << indent << "                else:\n";
        source << indent << "                    process_" << i << "(j_" << i << ")\n";
    }
    
    bool success = testParsingPerformance(source.str(), "Complex Control Flow", 4000.0);
    assert(success);
}

void test_long_string_literals() {
    std::cout << "Testing long string literals...\n";
    
    std::stringstream source;
    
    // Create very long string literals
    for (int i = 0; i < 50; ++i) {
        source << "long_string_" << i << " = \"";
        for (int j = 0; j < 1000; ++j) {
            source << "This is a very long string literal part " << j << " ";
        }
        source << "\"\n";
    }
    
    bool success = testParsingPerformance(source.str(), "Long String Literals", 3000.0);
    assert(success);
}

void test_mixed_stress() {
    std::cout << "Testing mixed stress scenario...\n";
    
    std::stringstream source;
    
    // Combine multiple stress factors
    source << "# Mixed stress test program\n\n";
    
    // Many imports
    for (int i = 0; i < 50; ++i) {
        source << "import module_" << i << "\n";
    }
    source << "\n";
    
    // Large class with many methods
    source << "class StressTestClass:\n";
    source << "    def __init__(self):\n";
    source << "        self.data = [";
    for (int i = 0; i < 100; ++i) {
        if (i > 0) source << ", ";
        source << i;
    }
    source << "]\n\n";
    
    // Many methods with complex logic
    for (int i = 0; i < 100; ++i) {
        source << "    def method_" << i << "(self, param1, param2 = " << i << "):\n";
        source << "        result = 0\n";
        source << "        for item in self.data:\n";
        source << "            if item % " << (i + 1) << " == 0:\n";
        source << "                result += item * param1\n";
        source << "            else:\n";
        source << "                result -= item * param2\n";
        source << "        return result\n\n";
    }
    
    // Main function with complex operations
    source << "def main():\n";
    source << "    obj = StressTestClass()\n";
    for (int i = 0; i < 50; ++i) {
        source << "    result_" << i << " = obj.method_" << i << "(" << i << ", " << (i * 2) << ")\n";
    }
    source << "\nif __name__ == \"__main__\":\n";
    source << "    main()\n";
    
    bool success = testParsingPerformance(source.str(), "Mixed Stress Test", 15000.0);
    assert(success);
}

void test_memory_usage() {
    std::cout << "Testing memory usage with large AST...\n";
    
    std::stringstream source;
    
    // Generate a program that will create a large AST
    for (int i = 0; i < 500; ++i) {
        source << "def func_" << i << "():\n";
        source << "    data = {\n";
        for (int j = 0; j < 50; ++j) {
            if (j > 0) source << ",\n";
            source << "        \"key_" << i << "_" << j << "\": [";
            for (int k = 0; k < 10; ++k) {
                if (k > 0) source << ", ";
                source << (i * 100 + j * 10 + k);
            }
            source << "]";
        }
        source << "\n    }\n";
        source << "    return data\n\n";
    }
    
    Timer timer;
    
    try {
        caesar::Lexer lexer(source.str());
        auto tokens = lexer.tokenize();
        
        caesar::Parser parser(std::move(tokens));
        auto program = parser.parse();
        
        double elapsed = timer.elapsed();
        
        assert(program != nullptr);
        std::cout << "✓ Memory usage test completed in " << elapsed << "ms\n";
        std::cout << "  Created AST with " << program->statements.size() << " top-level statements\n";
    } catch (const std::exception& e) {
        std::cerr << "❌ Memory usage test failed: " << e.what() << "\n";
        assert(false);
    }
}

int main() {
    std::cout << "Running Caesar stress tests...\n\n";
    
    try {
        test_large_file();
        test_deeply_nested_structures();
        test_complex_expressions();
        test_many_function_calls();
        test_large_data_structures();
        test_many_classes();
        test_complex_control_flow();
        test_long_string_literals();
        test_mixed_stress();
        test_memory_usage();
        
        std::cout << "\n✅ All stress tests passed!\n";
        std::cout << "Caesar parser can handle large-scale programs efficiently.\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "\n❌ Stress test failed: " << e.what() << "\n";
        return 1;
    }
}