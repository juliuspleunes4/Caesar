#include "include/caesar/lexer.h"
#include "include/caesar/parser.h"
#include <iostream>

int main() {
    std::string source = R"(class MyClass:
    def __init__(self):
        self.value = 42
    
    def method(self):
        return self.value
)";

    try {
        caesar::Lexer lexer(source);
        caesar::Parser parser(lexer);
        auto ast = parser.parse();
        
        std::cout << "Successfully parsed class definition!" << std::endl;
        std::cout << "AST: " << ast->toString() << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        return 1;
    }
}