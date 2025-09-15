# Tests for Caesar programming language
add_executable(test_lexer test_lexer.cpp)
target_link_libraries(test_lexer caesar_lib)

# Add tests to CTest
add_test(NAME lexer_test COMMAND test_lexer)