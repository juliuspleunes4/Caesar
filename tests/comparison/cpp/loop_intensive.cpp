#include <iostream>
#include <cstdlib>

/**
 * Loop-intensive benchmark for C++
 * Usage: ./loop_intensive <iterations>
 */

long long nested_loops(int n) {
    long long total = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < 100; ++j) {
            total += i * j;
        }
    }
    return total;
}

long long simple_loop(int n) {
    long long total = 0;
    for (int i = 0; i < n; ++i) {
        total += i * i;
    }
    return total;
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
    
    // Choose algorithm based on scale
    long long result;
    if (n <= 100000) {
        result = nested_loops(n / 100);  // Scale down for nested loops
    } else {
        result = simple_loop(n);
    }
    
    // Don't print result to avoid affecting timing
    
    return 0;
}