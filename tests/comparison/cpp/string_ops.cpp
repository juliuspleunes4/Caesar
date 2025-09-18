#include <iostream>
#include <string>
#include <cstdlib>
#include <algorithm>

/**
 * String operations benchmark for C++
 * Usage: ./string_ops <iterations>
 */

int string_concatenation(int n) {
    std::string result = "";
    for (int i = 0; i < n; ++i) {
        result += std::to_string(i);
    }
    return result.length();
}

int string_processing(int n) {
    std::string text = "Hello World! ";
    // Replicate text 100 times
    std::string large_text = "";
    for (int i = 0; i < 100; ++i) {
        large_text += text;
    }
    
    int count = 0;
    for (int i = 0; i < n; ++i) {
        // String operations
        std::string upper_text = large_text;
        std::transform(upper_text.begin(), upper_text.end(), upper_text.begin(), ::toupper);
        
        std::string lower_text = large_text;
        std::transform(lower_text.begin(), lower_text.end(), lower_text.begin(), ::tolower);
        
        count += upper_text.length() + lower_text.length();
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