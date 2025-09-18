#!/usr/bin/env python3
"""
Fibonacci benchmark for Python
Usage: python fibonacci.py <n>
"""

import sys
import time

def fibonacci_recursive(n):
    """Recursive fibonacci implementation"""
    if n <= 1:
        return n
    return fibonacci_recursive(n - 1) + fibonacci_recursive(n - 2)

def fibonacci_iterative(n):
    """Iterative fibonacci implementation - standardized across all languages"""
    if n <= 1:
        return n
    
    a = 0
    b = 1
    i = 2
    while i <= n:
        temp = a + b
        a = b
        b = temp
        i += 1
    return b

def main():
    if len(sys.argv) != 2:
        print("Usage: python fibonacci.py <n>")
        sys.exit(1)
    
    try:
        n = int(sys.argv[1])
    except ValueError:
        print("Error: n must be an integer")
        sys.exit(1)
    
    if n < 0:
        print("Error: n must be non-negative")
        sys.exit(1)
    
    # Use recursive for smaller values, iterative for larger
    if n <= 35:
        result = fibonacci_recursive(n)
    else:
        result = fibonacci_iterative(n)
    
    # Don't print result to avoid affecting timing
    # print(f"fib({n}) = {result}")

if __name__ == "__main__":
    main()