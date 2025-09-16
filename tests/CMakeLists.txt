# Tests for Caesar programming language
add_executable(test_lexer test_lexer.cpp)
target_link_libraries(test_lexer caesar_lib)

add_executable(test_parser test_parser.cpp)
target_link_libraries(test_parser caesar_lib)

# Add tests to CTest
add_test(NAME lexer_test COMMAND test_lexer)
add_test(NAME parser_test COMMAND test_parser)