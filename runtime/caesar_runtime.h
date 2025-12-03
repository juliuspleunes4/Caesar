// Caesar Runtime Library for C Code Generation
// This header provides runtime support for compiled Caesar code

#ifndef CAESAR_RUNTIME_H
#define CAESAR_RUNTIME_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

// Value type enumeration for Caesar types
typedef enum {
    CAESAR_INT,
    CAESAR_FLOAT,
    CAESAR_STRING,
    CAESAR_BOOL,
    CAESAR_NONE
} CaesarType;

// Generic value type for Caesar
typedef struct {
    CaesarType type;
    union {
        int64_t i;
        double f;
        const char* s;
        bool b;
    } data;
} CaesarValue;

// Built-in function: print
// Prints all arguments separated by spaces, followed by newline
void caesar_print(int argc, CaesarValue* args) {
    for (int i = 0; i < argc; i++) {
        if (i > 0) {
            printf(" ");
        }
        
        switch (args[i].type) {
            case CAESAR_INT:
                printf("%ld", (long)args[i].data.i);
                break;
            case CAESAR_FLOAT:
                printf("%g", args[i].data.f);
                break;
            case CAESAR_STRING:
                printf("%s", args[i].data.s);
                break;
            case CAESAR_BOOL:
                printf("%s", args[i].data.b ? "True" : "False");
                break;
            case CAESAR_NONE:
                printf("None");
                break;
        }
    }
    printf("\n");
}

#endif // CAESAR_RUNTIME_H
