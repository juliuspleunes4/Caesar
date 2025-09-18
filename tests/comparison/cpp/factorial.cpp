#include <iostream>
#include <cstdlib>

/**
 * Factorial benchmark for C++
 * Usage: ./factorial <n>
 */

long long factorial_recursive(int n) {
    if (n <= 1) {
        return 1;
    }
    return n * factorial_recursive(n - 1);
}

long long factorial_iterative(int n) {
    long long result = 1;
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <n>" << std::endl;
        return 1;
    }
    
    int n = std::atoi(argv[1]);
    if (n < 0) {
        std::cerr << "Error: n must be non-negative" << std::endl;
        return 1;
    }
    
    // Use recursive for reasonable stack depth
    long long result;
    if (n <= 20) {
        result = factorial_recursive(n);
    } else {
        result = factorial_iterative(n);
    }
    
    // Don't print result to avoid affecting timing
    
    return 0;
}