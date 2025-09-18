#include <iostream>
#include <string>
#include <cstdlib>
#include <algorithm>

/**
 * String operations benchmark for C++
 * Usage: ./string_ops <iterations>
 */

int string_concatenation(int n) {
    // String concatenation performance - standardized across all languages
    std::string result = "";
    for (int i = 0; i < n; ++i) {
        // Simple character concatenation (matching Caesar implementation)
        result += "x";
    }
    return result.length();
}

int string_processing(int n) {
    // String processing operations - standardized across all languages
    std::string text = "Hello World! This is a test string for benchmarking purposes.";
    int count = 0;
    
    for (int i = 0; i < n; ++i) {
        // Simple string length operation (matching Caesar implementation)
        int length = text.length();
        count += length;
    }
    
    return count;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <iterations>" << std::endl;
        return 1;
    }
    
    int n = std::atoi(argv[1]);
    if (n < 0) {
        std::cerr << "Error: iterations must be non-negative" << std::endl;
        return 1;
    }
    
    // Choose test based on scale
    int result;
    if (n <= 10000) {
        result = string_concatenation(n);
    } else {
        result = string_processing(n / 100);
    }
    
    // Don't print result to avoid affecting timing
    
    return 0;
}