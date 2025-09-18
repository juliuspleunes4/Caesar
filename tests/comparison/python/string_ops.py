#!/usr/bin/env python3
"""
String operations benchmark for Python
Usage: python string_ops.py <iterations>
"""

import sys

def string_concatenation(n):
    """String concatenation performance"""
    result = ""
    for i in range(n):
        result += str(i)
    return len(result)

def string_processing(n):
    """String processing operations"""
    text = "Hello World! " * 100
    count = 0
    
    for _ in range(n):
        # String operations
        upper_text = text.upper()
        lower_text = text.lower()
        length = len(upper_text) + len(lower_text)
        count += length
    
    return count

def main():
    if len(sys.argv) != 2:
        print("Usage: python string_ops.py <iterations>")
        sys.exit(1)
    
    try:
        n = int(sys.argv[1])
    except ValueError:
        print("Error: iterations must be an integer")
        sys.exit(1)
    
    if n < 0:
        print("Error: iterations must be non-negative")
        sys.exit(1)
    
    # Choose test based on scale
    if n <= 10000:
        result = string_concatenation(n)
    else:
        result = string_processing(n // 100)
    
    # Don't print result to avoid affecting timing

if __name__ == "__main__":
    main()