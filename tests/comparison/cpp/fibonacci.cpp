#include <iostream>
#include <chrono>
#include <cstdlib>

/**
 * Fibonacci benchmark for C++
 * Usage: ./fibonacci <n>
 */

long long fibonacci_recursive(int n) {
    if (n <= 1) {
        return n;
    }
    return fibonacci_recursive(n - 1) + fibonacci_recursive(n - 2);
}

long long fibonacci_iterative(int n) {
    // Iterative fibonacci implementation - standardized across all languages
    if (n <= 1) {
        return n;
    }
    
    long long a = 0, b = 1;
    int i = 2;
    while (i <= n) {
        long long temp = a + b;
        a = b;
        b = temp;
        i++;
    }
    return b;
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
    
    // Use recursive for smaller values, iterative for larger
    long long result;
    if (n <= 35) {
        result = fibonacci_recursive(n);
    } else {
        result = fibonacci_iterative(n);
    }
    
    // Don't print result to avoid affecting timing
    // std::cout << "fib(" << n << ") = " << result << std::endl;
    
    return 0;
}