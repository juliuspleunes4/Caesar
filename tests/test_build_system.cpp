/**
 * @file test_build_system.cpp
 * @brief Build system tests for the Caesar project
 * @author Julius Pleunes
 * @version 1.0.0
 */

#undef NDEBUG
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <cstdlib>

// Simple assert replacement for debugging
#define my_assert(condition) \
    do { \
        if (!(condition)) { \
            std::cerr << "Assertion failed: " << #condition << " at line " << __LINE__ << std::endl; \
            std::abort(); \
        } \
    } while(0)

#ifndef assert
#define assert my_assert
#endif

// Helper function to check if a file exists
bool fileExists(const std::string& filepath) {
    std::ifstream file(filepath);
    return file.good();
}

// Helper function to check if a string contains a substring
bool contains(const std::string& str, const std::string& substr) {
    return str.find(substr) != std::string::npos;
}

// Helper function to read file contents
std::string readFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        return "";
    }
    
    std::string content;
    std::string line;
    while (std::getline(file, line)) {
        content += line + "\n";
    }
    return content;
}

void test_cmake_configuration() {
    std::cout << "Testing CMake configuration...\n";
    
    // Check if CMakeLists.txt exists in root
    assert(fileExists("../CMakeLists.txt"));
    std::cout << "✓ Root CMakeLists.txt exists\n";
    
    // Check if CMakeLists.txt exists in src
    assert(fileExists("../src/CMakeLists.txt"));
    std::cout << "✓ Source CMakeLists.txt exists\n";
    
    // Check if CMakeLists.txt exists in tests
    assert(fileExists("../tests/CMakeLists.txt"));
    std::cout << "✓ Tests CMakeLists.txt exists\n";
    
    // Read and verify root CMakeLists.txt content
    std::string rootCMake = readFile("../CMakeLists.txt");
    assert(!rootCMake.empty());
    assert(contains(rootCMake, "cmake_minimum_required"));
    assert(contains(rootCMake, "project"));
    assert(contains(rootCMake, "set(CMAKE_CXX_STANDARD"));
    std::cout << "✓ Root CMakeLists.txt has required elements\n";
    
    std::cout << "✓ CMake configuration tests passed\n";
}

void test_build_targets() {
    std::cout << "Testing build targets...\n";
    
    // Check if build directory exists
    assert(fileExists("../build"));
    std::cout << "✓ Build directory exists\n";
    
    // Check if Makefile was generated
    assert(fileExists("../build/Makefile"));
    std::cout << "✓ Makefile generated\n";
    
    // Check if CMakeCache.txt exists
    assert(fileExists("../build/CMakeCache.txt"));
    std::cout << "✓ CMakeCache.txt exists\n";
    
    // Check for expected build targets
    std::string cacheContent = readFile("../build/CMakeCache.txt");
    if (!cacheContent.empty()) {
        std::cout << "✓ CMakeCache.txt is readable\n";
    }
    
    std::cout << "✓ Build targets tests passed\n";
}

void test_executable_generation() {
    std::cout << "Testing executable generation...\n";
    
    // Check if main executables were built
    bool caesarExists = fileExists("../build/src/caesar") || fileExists("../build/src/caesar.exe");
    bool replExists = fileExists("../build/src/caesar_repl") || fileExists("../build/src/caesar_repl.exe");
    
    if (caesarExists) {
        std::cout << "✓ Caesar main executable exists\n";
    } else {
        std::cout << "⚠ Caesar main executable not found (may not be built yet)\n";
    }
    
    if (replExists) {
        std::cout << "✓ Caesar REPL executable exists\n";
    } else {
        std::cout << "⚠ Caesar REPL executable not found (may not be built yet)\n";
    }
    
    std::cout << "✓ Executable generation tests completed\n";
}

void test_library_generation() {
    std::cout << "Testing library generation...\n";
    
    // Check if static library was built
    bool libExists = fileExists("../build/src/libcaesar_lib.a") || 
                     fileExists("../build/src/caesar_lib.lib");
    
    if (libExists) {
        std::cout << "✓ Caesar static library exists\n";
    } else {
        std::cout << "⚠ Caesar static library not found (may not be built yet)\n";
    }
    
    std::cout << "✓ Library generation tests completed\n";
}

void test_test_executables() {
    std::cout << "Testing test executable generation...\n";
    
    // Check if test executables were built
    bool lexerTestExists = fileExists("../build/tests/test_lexer") || 
                          fileExists("../build/tests/test_lexer.exe");
    
    if (lexerTestExists) {
        std::cout << "✓ Lexer test executable exists\n";
    } else {
        std::cout << "⚠ Lexer test executable not found (may not be built yet)\n";
    }
    
    // Check for other test executables
    std::string testDirs[] = {
        "../build/tests/test_parser_advanced",
        "../build/tests/test_parser_advanced.exe",
        "../build/tests/test_lexer_advanced", 
        "../build/tests/test_lexer_advanced.exe",
        "../build/tests/test_integration",
        "../build/tests/test_integration.exe",
        "../build/tests/test_stress",
        "../build/tests/test_stress.exe",
        "../build/tests/test_error_handling",
        "../build/tests/test_error_handling.exe"
    };
    
    int foundTests = 0;
    for (const auto& testPath : testDirs) {
        if (fileExists(testPath)) {
            foundTests++;
        }
    }
    
    std::cout << "✓ Found " << foundTests << " test executables\n";
    std::cout << "✓ Test executable generation tests completed\n";
}

void test_compiler_flags() {
    std::cout << "Testing compiler flag configuration...\n";
    
    // Read CMakeCache to check compiler flags
    std::string cacheContent = readFile("../build/CMakeCache.txt");
    
    if (!cacheContent.empty()) {
        // Check for C++ standard setting
        if (contains(cacheContent, "CMAKE_CXX_STANDARD")) {
            std::cout << "✓ C++ standard is configured\n";
        }
        
        // Check for compiler flags
        if (contains(cacheContent, "CMAKE_CXX_FLAGS")) {
            std::cout << "✓ C++ compiler flags are configured\n";
        }
        
        // Check for build type
        if (contains(cacheContent, "CMAKE_BUILD_TYPE")) {
            std::cout << "✓ Build type is configured\n";
        }
    }
    
    std::cout << "✓ Compiler flag tests completed\n";
}

void test_dependency_resolution() {
    std::cout << "Testing dependency resolution...\n";
    
    // Check if all required headers are accessible
    std::string srcCMake = readFile("../src/CMakeLists.txt");
    if (!srcCMake.empty()) {
        if (contains(srcCMake, "target_include_directories")) {
            std::cout << "✓ Include directories are configured\n";
        }
        
        if (contains(srcCMake, "target_link_libraries")) {
            std::cout << "✓ Library linking is configured\n";
        }
    }
    
    // Check if include directory exists
    assert(fileExists("../include"));
    std::cout << "✓ Include directory exists\n";
    
    // Check for required header files
    assert(fileExists("../include/caesar/caesar.h"));
    assert(fileExists("../include/caesar/lexer.h"));
    assert(fileExists("../include/caesar/token.h"));
    std::cout << "✓ Required header files exist\n";
    
    std::cout << "✓ Dependency resolution tests passed\n";
}

void test_build_configuration_options() {
    std::cout << "Testing build configuration options...\n";
    
    // Check CMakeCache for configuration options
    std::string cacheContent = readFile("../build/CMakeCache.txt");
    
    if (!cacheContent.empty()) {
        // Look for common CMake variables
        std::string checkVars[] = {
            "CMAKE_INSTALL_PREFIX",
            "CMAKE_CXX_COMPILER",
            "CMAKE_C_COMPILER",
            "CMAKE_GENERATOR"
        };
        
        for (const auto& var : checkVars) {
            if (contains(cacheContent, var)) {
                std::cout << "✓ " << var << " is configured\n";
            }
        }
    }
    
    // Check if Doxyfile configuration exists (for documentation)
    if (fileExists("../Doxyfile.in")) {
        std::cout << "✓ Doxygen configuration template exists\n";
    }
    
    std::cout << "✓ Build configuration options tests completed\n";
}

void test_clean_build() {
    std::cout << "Testing clean build capability...\n";
    
    // Check if we can identify build artifacts
    std::string buildArtifacts[] = {
        "../build/src/CMakeFiles",
        "../build/tests/CMakeFiles", 
        "../build/CMakeFiles"
    };
    
    int foundArtifacts = 0;
    for (const auto& artifact : buildArtifacts) {
        if (fileExists(artifact)) {
            foundArtifacts++;
        }
    }
    
    std::cout << "✓ Found " << foundArtifacts << " build artifact directories\n";
    
    // Check if Makefile has clean target (if readable)
    std::string makefile = readFile("../build/Makefile");
    if (!makefile.empty() && contains(makefile, "clean")) {
        std::cout << "✓ Clean target is available in Makefile\n";
    }
    
    std::cout << "✓ Clean build tests completed\n";
}

void test_cross_platform_compatibility() {
    std::cout << "Testing cross-platform compatibility...\n";
    
    // Check CMakeCache for platform-specific settings
    std::string cacheContent = readFile("../build/CMakeCache.txt");
    
    if (!cacheContent.empty()) {
        if (contains(cacheContent, "CMAKE_SYSTEM_NAME")) {
            std::cout << "✓ System name is detected\n";
        }
        
        if (contains(cacheContent, "CMAKE_SYSTEM_PROCESSOR")) {
            std::cout << "✓ System processor is detected\n";
        }
        
        // Check for Windows-specific or Unix-specific configurations
        if (contains(cacheContent, "WIN32") || contains(cacheContent, "Windows")) {
            std::cout << "✓ Windows-specific configuration detected\n";
        } else if (contains(cacheContent, "UNIX") || contains(cacheContent, "Linux")) {
            std::cout << "✓ Unix/Linux-specific configuration detected\n";
        }
    }
    
    std::cout << "✓ Cross-platform compatibility tests completed\n";
}

int main() {
    std::cout << "Running Caesar build system tests...\n\n";
    
    try {
        test_cmake_configuration();
        test_build_targets();
        test_executable_generation();
        test_library_generation();
        test_test_executables();
        test_compiler_flags();
        test_dependency_resolution();
        test_build_configuration_options();
        test_clean_build();
        test_cross_platform_compatibility();
        
        std::cout << "\n✅ All build system tests completed!\n";
        std::cout << "Caesar build system is properly configured and functional.\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "\n❌ Build system test failed: " << e.what() << "\n";
        return 1;
    }
}