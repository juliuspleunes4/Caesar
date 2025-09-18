#include <iostream>
#include <cstdlib>

/**
 * Prime checking benchmark for C++
 * Usage: ./prime_check <limit>
 */

bool is_prime(int n) {
    // Check if a number is prime - standardized across all languages
    if (n < 2) {
        return false;
    }
    if (n == 2) {
        return true;
    }
    if (n % 2 == 0) {
        return false;
    }
    
    int i = 3;
    while (i * i <= n) {
        if (n % i == 0) {
            return false;
        }
        i += 2;
    }
    return true;
}

int count_primes(int limit) {
    // Count prime numbers up to limit - standardized across all languages
    int count = 0;
    int i = 2;
    while (i <= limit) {
        if (is_prime(i)) {
            count++;
        }
        i++;
    }
    return count;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <limit>" << std::endl;
        return 1;
    }
    
    int limit = std::atoi(argv[1]);
    if (limit < 0) {
        std::cerr << "Error: limit must be non-negative" << std::endl;
        return 1;
    }
    
    int result = count_primes(limit);
    
    // Don't print result to avoid affecting timing
    
    return 0;
}