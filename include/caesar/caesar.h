/**
 * @file caesar.h
 * @brief Main header file for the Caesar programming language
 * @author J.J.G. Pleunes
 * @version 1.0.0
 * 
 * Caesar is a Python-like programming language with C-speed performance,
 * built using modern C++ and LLVM backend for efficient compilation.
 */

#ifndef CAESAR_H
#define CAESAR_H

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

/**
 * @namespace caesar
 * @brief Main namespace for all Caesar language components
 */
namespace caesar {

/**
 * @brief Version information for Caesar
 */
struct Version {
    static constexpr int MAJOR = 1;
    static constexpr int MINOR = 0;
    static constexpr int PATCH = 0;
    static constexpr const char* STRING = "1.0.0";
};

/**
 * @brief Base exception class for all Caesar-related errors
 */
class CaesarException : public std::runtime_error {
public:
    explicit CaesarException(const std::string& message) : std::runtime_error(message) {}
};

/**
 * @brief Exception thrown during lexical analysis
 */
class LexerException : public CaesarException {
public:
    explicit LexerException(const std::string& message) : CaesarException("Lexer Error: " + message) {}
};

/**
 * @brief Exception thrown during parsing
 */
class ParserException : public CaesarException {
public:
    explicit ParserException(const std::string& message) : CaesarException("Parser Error: " + message) {}
};

/**
 * @brief Exception thrown during IR generation
 */
class IRException : public CaesarException {
public:
    explicit IRException(const std::string& message) : CaesarException("IR Error: " + message) {}
};

/**
 * @brief Exception thrown during code generation
 */
class CodegenException : public CaesarException {
public:
    explicit CodegenException(const std::string& message) : CaesarException("Codegen Error: " + message) {}
};

/**
 * @brief Exception thrown during runtime execution
 */
class RuntimeException : public CaesarException {
public:
    explicit RuntimeException(const std::string& message) : CaesarException("Runtime Error: " + message) {}
};

} // namespace caesar

#endif // CAESAR_H
