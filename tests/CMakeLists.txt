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

# C code generation string tests
add_executable(test_c_strings test_c_strings.cpp)
target_link_libraries(test_c_strings caesar_lib)

# C code generation float tests
add_executable(test_c_floats test_c_floats.cpp)
target_link_libraries(test_c_floats caesar_lib)

# C code generation mixed type tests
add_executable(test_c_mixed_types test_c_mixed_types.cpp)
target_link_libraries(test_c_mixed_types caesar_lib)

# C code generation loop tests
add_executable(test_c_loops test_c_loops.cpp)
target_link_libraries(test_c_loops caesar_lib)

# C code generation print built-in tests
add_executable(test_c_print test_c_print.cpp)
target_link_libraries(test_c_print caesar_lib)

# C code generation len built-in tests
add_executable(test_c_len test_c_len.cpp)
target_link_libraries(test_c_len caesar_lib)

# C code generation str built-in tests
add_executable(test_c_str test_c_str.cpp)
target_link_libraries(test_c_str caesar_lib)

# C code generation int built-in tests
add_executable(test_c_int test_c_int.cpp)
target_link_libraries(test_c_int caesar_lib)

# C code generation float built-in tests
add_executable(test_c_float test_c_float.cpp)
target_link_libraries(test_c_float caesar_lib)

# C code generation abs built-in tests
add_executable(test_c_abs test_c_abs.cpp)
target_link_libraries(test_c_abs caesar_lib)

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
add_test(NAME c_strings_test COMMAND test_c_strings)
add_test(NAME c_floats_test COMMAND test_c_floats)
add_test(NAME c_mixed_types_test COMMAND test_c_mixed_types)
add_test(NAME c_loops_test COMMAND test_c_loops)
add_test(NAME c_print_test COMMAND test_c_print)
add_test(NAME c_len_test COMMAND test_c_len)
add_test(NAME c_str_test COMMAND test_c_str)
add_test(NAME c_int_test COMMAND test_c_int)
add_test(NAME c_float_test COMMAND test_c_float)
add_test(NAME c_abs_test COMMAND test_c_abs)