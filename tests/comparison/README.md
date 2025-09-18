# Performance Comparison Suite

This directory contains performance benchmarks comparing Caesar against Python and C++ across various algorithms and use cases.

## Directory Structure

```
comparison/
├── README.md                 # This file
├── run_benchmarks.ps1        # Main benchmark runner
├── benchmark_results.md      # Latest benchmark results
├── algorithms/               # Algorithm implementations
│   ├── fibonacci/            # Fibonacci sequence computation
│   ├── factorial/            # Factorial computation
│   ├── prime_check/          # Prime number checking
│   ├── sorting/              # Sorting algorithms
│   └── matrix_multiply/      # Matrix multiplication
├── python/                   # Python implementations
├── caesar/                   # Caesar implementations
└── cpp/                      # C++ implementations
```

## Benchmark Categories

### 1. Mathematical Algorithms
- **Fibonacci**: Recursive and iterative implementations
- **Factorial**: Recursive computation with large numbers
- **Prime Checking**: Efficiency in mathematical operations
- **Matrix Operations**: Numerical computation performance

### 2. Control Flow Performance
- **Loop Intensive**: Performance in tight loops
- **Conditional Logic**: Complex branching performance
- **Function Calls**: Call overhead and optimization

### 3. Data Processing
- **String Operations**: Text processing and manipulation
- **Numerical Computation**: Arithmetic operation speed
- **Type Conversions**: Dynamic typing overhead

## Running Benchmarks

```powershell
# Run all benchmarks
.\run_benchmarks.ps1

# Run specific category
.\run_benchmarks.ps1 -Category fibonacci

# Run with custom iterations
.\run_benchmarks.ps1 -Iterations 10000
```

## Performance Expectations

Based on Caesar's architecture, we expect:

- **Caesar vs Python**: 10-100x faster (compiled vs interpreted)
- **Caesar vs C++**: 1-5x slower (dynamic typing vs static typing)
- **Memory Usage**: Caesar < C++ < Python (typically)

## Benchmark Results Format

Each benchmark produces:
- **Execution Time**: Average time across multiple runs
- **Memory Usage**: Peak memory consumption
- **Relative Performance**: Comparison ratios
- **Standard Deviation**: Performance consistency

## Implementation Guidelines

### For Each Algorithm:
1. **Identical Logic**: Same algorithm across all languages
2. **Fair Comparison**: No language-specific optimizations
3. **Multiple Scales**: Test with different input sizes
4. **Statistical Validity**: Multiple runs with averaging

### Language-Specific Notes:
- **Python**: Use standard library, no NumPy for numerical tasks
- **Caesar**: Use only built-in functions and language features
- **C++**: Standard library only, no specialized optimizations

## Contributing Benchmarks

When adding new benchmarks:
1. Create algorithm folder under `algorithms/`
2. Implement in all three languages
3. Add to benchmark runner script
4. Document expected performance characteristics
5. Test for correctness before performance measurement

---

*Performance comparison suite for Caesar Programming Language*