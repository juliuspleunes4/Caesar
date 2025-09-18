#!/usr/bin/env python3
"""
Factorial benchmark for Python
Usage: python factorial.py <n>
"""

import sys

def factorial_recursive(n):
    """Recursive factorial implementation"""
    if n <= 1:
        return 1
    return n * factorial_recursive(n - 1)

def factorial_iterative(n):
    """Iterative factorial implementation - standardized across all languages"""
    result = 1
    i = 2
    while i <= n:
        result *= i
        i += 1
    return result

def main():
    if len(sys.argv) != 2:
        print("Usage: python factorial.py <n>")
        sys.exit(1)
    
    try:
        n = int(sys.argv[1])
    except ValueError:
        print("Error: n must be an integer")
        sys.exit(1)
    
    if n < 0:
        print("Error: n must be non-negative")
        sys.exit(1)
    
    # Use recursive for reasonable stack depth - standardized across all languages
    if n <= 20:
        result = factorial_recursive(n)
    else:
        result = factorial_iterative(n)
    
    # Don't print result to avoid affecting timing

if __name__ == "__main__":
    main()