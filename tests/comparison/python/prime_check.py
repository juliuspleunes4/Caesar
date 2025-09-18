#!/usr/bin/env python3
"""
Prime checking benchmark for Python
Usage: python prime_check.py <limit>
"""

import sys

def is_prime(n):
    """Check if a number is prime"""
    if n < 2:
        return False
    if n == 2:
        return True
    if n % 2 == 0:
        return False
    
    i = 3
    while i * i <= n:
        if n % i == 0:
            return False
        i += 2
    return True

def count_primes(limit):
    """Count prime numbers up to limit - standardized across all languages"""
    count = 0
    i = 2
    while i <= limit:
        if is_prime(i):
            count += 1
        i += 1
    return count

def main():
    if len(sys.argv) != 2:
        print("Usage: python prime_check.py <limit>")
        sys.exit(1)
    
    try:
        limit = int(sys.argv[1])
    except ValueError:
        print("Error: limit must be an integer")
        sys.exit(1)
    
    if limit < 0:
        print("Error: limit must be non-negative")
        sys.exit(1)
    
    result = count_primes(limit)
    
    # Don't print result to avoid affecting timing

if __name__ == "__main__":
    main()