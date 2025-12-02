/**
 * @file caesar_runtime.h
 * @brief Caesar Runtime Library - C implementations of built-in functions
 * @author J.J.G. Pleunes
 * @version 1.5.1
 * 
 * This header provides the runtime support needed for Caesar programs
 * compiled to C. It includes implementations of built-in functions,
 * data structures, and memory management.
 */

#ifndef CAESAR_RUNTIME_H
#define CAESAR_RUNTIME_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// Basic I/O Functions
// ============================================================================

/**
 * Print an integer value followed by newline
 */
static inline void caesar_print_int(int64_t val) {
    printf("%lld\n", (long long)val);
}

/**
 * Print a string value followed by newline
 */
static inline void caesar_print_str(const char* val) {
    printf("%s\n", val);
}

/**
 * Print a floating point value followed by newline
 */
static inline void caesar_print_float(double val) {
    printf("%g\n", val);
}

/**
 * Print a double value (alias for caesar_print_float)
 */
static inline void caesar_print_double(double val) {
    printf("%g\n", val);
}

/**
 * Print a boolean value followed by newline
 */
static inline void caesar_print_bool(bool val) {
    printf("%s\n", val ? "true" : "false");
}

// ============================================================================
// Range Iterator (for for-loops)
// ============================================================================

/**
 * Range iterator structure for for-loops
 * Supports: for i in range(stop)
 *           for i in range(start, stop)
 *           for i in range(start, stop, step)
 */
typedef struct {
    int64_t current;
    int64_t stop;
    int64_t step;
    bool done;
} CaesarRange;

/**
 * Initialize a range iterator: range(stop)
 */
static inline CaesarRange caesar_range_init(int64_t stop) {
    CaesarRange r;
    r.current = 0;
    r.stop = stop;
    r.step = 1;
    r.done = (0 >= stop);
    return r;
}

/**
 * Initialize a range iterator: range(start, stop)
 */
static inline CaesarRange caesar_range_init2(int64_t start, int64_t stop) {
    CaesarRange r;
    r.current = start;
    r.stop = stop;
    r.step = (start < stop) ? 1 : -1;
    r.done = (start >= stop && r.step > 0) || (start <= stop && r.step < 0);
    return r;
}

/**
 * Initialize a range iterator: range(start, stop, step)
 */
static inline CaesarRange caesar_range_init3(int64_t start, int64_t stop, int64_t step) {
    CaesarRange r;
    r.current = start;
    r.stop = stop;
    r.step = step;
    if (step > 0) {
        r.done = (start >= stop);
    } else if (step < 0) {
        r.done = (start <= stop);
    } else {
        r.done = true;  // step of 0 is invalid
    }
    return r;
}

/**
 * Check if range iterator has more values
 */
static inline bool caesar_range_has_next(CaesarRange* r) {
    return !r->done;
}

/**
 * Get next value from range iterator and advance
 */
static inline int64_t caesar_range_next(CaesarRange* r) {
    int64_t value = r->current;
    r->current += r->step;
    
    // Check if we're done after this iteration
    if (r->step > 0) {
        r->done = (r->current >= r->stop);
    } else {
        r->done = (r->current <= r->stop);
    }
    
    return value;
}

// ============================================================================
// Type Conversion Functions
// ============================================================================

/**
 * Convert string to integer
 * Returns 0 if conversion fails
 */
static inline int64_t caesar_int_from_str(const char* str) {
    return (int64_t)atoll(str);
}

/**
 * Convert string to float
 * Returns 0.0 if conversion fails
 */
static inline double caesar_float_from_str(const char* str) {
    return atof(str);
}

/**
 * Convert integer to string (caller must free)
 */
static inline char* caesar_str_from_int(int64_t val) {
    char* buf = (char*)malloc(32);
    snprintf(buf, 32, "%lld", (long long)val);
    return buf;
}

/**
 * Convert float to string (caller must free)
 */
static inline char* caesar_str_from_float(double val) {
    char* buf = (char*)malloc(32);
    snprintf(buf, 32, "%g", val);
    return buf;
}

// ============================================================================
// String Functions
// ============================================================================

/**
 * Get length of a string
 */
static inline int64_t caesar_len_str(const char* str) {
    return (int64_t)strlen(str);
}

/**
 * Get character at index (returns 0 if out of bounds)
 */
static inline char caesar_str_at(const char* str, int64_t index) {
    int64_t len = (int64_t)strlen(str);
    if (index < 0) index += len;  // Support negative indexing
    if (index < 0 || index >= len) return 0;
    return str[index];
}

// ============================================================================
// Math Functions
// ============================================================================

/**
 * Absolute value of integer
 */
static inline int64_t caesar_abs_int(int64_t val) {
    return (val < 0) ? -val : val;
}

/**
 * Absolute value of float
 */
static inline double caesar_abs_float(double val) {
    return (val < 0.0) ? -val : val;
}

#ifdef __cplusplus
}
#endif

#endif // CAESAR_RUNTIME_H
