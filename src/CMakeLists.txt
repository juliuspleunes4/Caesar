# Source files for Caesar compiler
set(CAESAR_SOURCES
    # Lexer
    lexer/token.cpp
    lexer/lexer.cpp
    
    # Parser (to be added)
    # parser/ast.cpp
    # parser/parser.cpp
    
    # IR Generation (to be added)
    # ir/ir_generator.cpp
    
    # Code Generation (to be added)
    # codegen/codegen.cpp
    
    # Runtime (to be added)
    # runtime/runtime.cpp
)

# Create the Caesar library
add_library(caesar_lib ${CAESAR_SOURCES})
target_link_libraries(caesar_lib ${llvm_libs})
target_include_directories(caesar_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Create the main Caesar executable
add_executable(caesar main.cpp)
target_link_libraries(caesar caesar_lib)

# Create a simple REPL executable for testing
add_executable(caesar_repl repl.cpp)
target_link_libraries(caesar_repl caesar_lib)