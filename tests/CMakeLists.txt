# Tests for Caesar programming language

# Basic tests
add_executable(test_lexer test_lexer.cpp)
target_link_libraries(test_lexer caesar_lib)

add_executable(test_parser test_parser.cpp)
target_link_libraries(test_parser caesar_lib)

# Advanced tests
add_executable(test_lexer_advanced test_lexer_advanced.cpp)
target_link_libraries(test_lexer_advanced caesar_lib)

add_executable(test_parser_advanced test_parser_advanced.cpp)
target_link_libraries(test_parser_advanced caesar_lib)

# Integration tests
add_executable(test_integration test_integration.cpp)
target_link_libraries(test_integration caesar_lib)

# Stress tests
add_executable(test_stress test_stress.cpp)
target_link_libraries(test_stress caesar_lib)

# Error handling tests
add_executable(test_error_handling test_error_handling.cpp)
target_link_libraries(test_error_handling caesar_lib)

# Build system tests
add_executable(test_build_system test_build_system.cpp)
target_link_libraries(test_build_system caesar_lib)

# Test coverage analysis
add_executable(test_coverage_analysis test_coverage_analysis.cpp)
target_link_libraries(test_coverage_analysis caesar_lib)

# IR and code generation tests
add_executable(test_ir test_ir.cpp)
target_link_libraries(test_ir caesar_lib)

# Compiler tests
add_executable(test_compiler test_compiler.cpp)
target_link_libraries(test_compiler caesar_lib)

# C code generation tests
add_executable(test_c_codegen test_c_codegen.cpp)
target_link_libraries(test_c_codegen caesar_lib)

# C code generation edge case tests
add_executable(test_c_edge_cases test_c_edge_cases.cpp)
target_link_libraries(test_c_edge_cases caesar_lib)

# Add tests to CTest
add_test(NAME lexer_test COMMAND test_lexer)
add_test(NAME parser_test COMMAND test_parser)
add_test(NAME lexer_advanced_test COMMAND test_lexer_advanced)
add_test(NAME parser_advanced_test COMMAND test_parser_advanced)
add_test(NAME integration_test COMMAND test_integration)
add_test(NAME stress_test COMMAND test_stress)
add_test(NAME error_handling_test COMMAND test_error_handling)
add_test(NAME build_system_test COMMAND test_build_system)
add_test(NAME coverage_analysis_test COMMAND test_coverage_analysis)
add_test(NAME ir_test COMMAND test_ir)
add_test(NAME compiler_test COMMAND test_compiler)
add_test(NAME c_codegen_test COMMAND test_c_codegen)
add_test(NAME c_edge_cases_test COMMAND test_c_edge_cases)