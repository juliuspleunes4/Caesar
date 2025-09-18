#!/usr/bin/env python3
"""
Loop-intensive benchmark for Python
Usage: python loop_intensive.py <iterations>
"""

import sys

def nested_loops(n):
    """Nested loop computation - standardized across all languages"""
    total = 0
    i = 0
    while i < n:
        j = 0
        while j < 100:
            total += i * j
            j += 1
        i += 1
    return total

def simple_loop(n):
    """Simple loop with arithmetic - standardized across all languages"""
    total = 0
    i = 0
    while i < n:
        total += i * i
        i += 1
    return total

def main():
    if len(sys.argv) != 2:
        print("Usage: python loop_intensive.py <iterations>")
        sys.exit(1)
    
    try:
        n = int(sys.argv[1])
    except ValueError:
        print("Error: iterations must be an integer")
        sys.exit(1)
    
    if n < 0:
        print("Error: iterations must be non-negative")
        sys.exit(1)
    
    # Choose algorithm based on scale
    if n <= 100000:
        result = nested_loops(n // 100)  # Scale down for nested loops
    else:
        result = simple_loop(n)
    
    # Don't print result to avoid affecting timing

if __name__ == "__main__":
    main()