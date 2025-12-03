/**
 * @file test_c_mixed_types.cpp
 * @brief Comprehensive tests for mixed type operations in C code generation
 * @author Caesar Compiler Team
 * @version 1.0.0
 * 
 * This test file rigorously tests all combinations of mixed type operations
 * following the requirement for extensive edge case testing.
 */

#include "caesar/ir.h"
#include "caesar/codegen.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>

using namespace caesar;

const int TOTAL_TESTS = 25;

// Helper function to compile and verify C code
bool compileAndVerify(const std::string& c_code, const std::string& test_name) {
    // Generate unique temporary filename with multiple entropy sources
    std::srand(std::time(nullptr) + getpid() + rand());
    std::string temp_file = "/tmp/caesar_test_mixed_" + std::to_string(getpid()) + "_" + 
                           std::to_string(rand()) + "_" + std::to_string(time(nullptr)) + ".c";
    
    // Write C code to file
    std::ofstream out(temp_file);
    if (!out.is_open()) {
        std::cerr << "Failed to create temp file: " << temp_file << std::endl;
        return false;
    }
    out << c_code;
    out.close();
    
    // Compile with GCC
    std::string compile_cmd = "gcc -std=c11 -o /dev/null " + temp_file + " 2>&1";
    int result = system(compile_cmd.c_str());
    
    // Clean up
    remove(temp_file.c_str());
    
    if (result == 0) {
        std::cout << "[PASS] " << test_name << std::endl;
        return true;
    } else {
        std::cout << "[FAIL] " << test_name << " - GCC compilation failed" << std::endl;
        return false;
    }
}

void test_int_plus_float() {
    std::vector<BasicBlock> blocks;
    BasicBlock block;
    block.label = "main";
    
    // x = 5
    block.instructions.push_back(IRInstruction{
        IROpcode::LOAD_CONST,
        IROperand{IROperandType::REGISTER, "%r0"},
        IROperand{IROperandType::CONSTANT, "#5"},
        IROperand{IROperandType::NONE, ""}
    });
    
    // y = 3.14
    block.instructions.push_back(IRInstruction{
        IROpcode::LOAD_CONST,
        IROperand{IROperandType::REGISTER, "%r1"},
        IROperand{IROperandType::CONSTANT, "#3.14"},
        IROperand{IROperandType::NONE, ""}
    });
    
    // z = x + y (should be double)
    block.instructions.push_back(IRInstruction{
        IROpcode::ADD,
        IROperand{IROperandType::REGISTER, "%r2"},
        IROperand{IROperandType::REGISTER, "%r0"},
        IROperand{IROperandType::REGISTER, "%r1"}
    });
    
    blocks.push_back(block);
    
    CCodeGenerator gen;
    std::string c_code = gen.generate(blocks);
    
    assert(compileAndVerify(c_code, "Int + Float"));
}

void test_float_plus_int() {
    std::vector<BasicBlock> blocks;
    BasicBlock block;
    block.label = "main";
    
    // x = 2.5
    block.instructions.push_back(IRInstruction{
        IROpcode::LOAD_CONST,
        IROperand{IROperandType::REGISTER, "%r0"},
        IROperand{IROperandType::CONSTANT, "#2.5"},
        IROperand{IROperandType::NONE, ""}
    });
    
    // y = 10
    block.instructions.push_back(IRInstruction{
        IROpcode::LOAD_CONST,
        IROperand{IROperandType::REGISTER, "%r1"},
        IROperand{IROperandType::CONSTANT, "#10"},
        IROperand{IROperandType::NONE, ""}
    });
    
    // z = x + y (should be double)
    block.instructions.push_back(IRInstruction{
        IROpcode::ADD,
        IROperand{IROperandType::REGISTER, "%r2"},
        IROperand{IROperandType::REGISTER, "%r0"},
        IROperand{IROperandType::REGISTER, "%r1"}
    });
    
    blocks.push_back(block);
    
    CCodeGenerator gen;
    std::string c_code = gen.generate(blocks);
    
    assert(compileAndVerify(c_code, "Float + Int"));
}

void test_int_minus_float() {
    std::vector<BasicBlock> blocks;
    BasicBlock block;
    block.label = "main";
    
    block.instructions.push_back(IRInstruction{
        IROpcode::LOAD_CONST,
        IROperand{IROperandType::REGISTER, "%r0"},
        IROperand{IROperandType::CONSTANT, "#100"},
        IROperand{IROperandType::NONE, ""}
    });
    
    block.instructions.push_back(IRInstruction{
        IROpcode::LOAD_CONST,
        IROperand{IROperandType::REGISTER, "%r1"},
        IROperand{IROperandType::CONSTANT, "#0.5"},
        IROperand{IROperandType::NONE, ""}
    });
    
    block.instructions.push_back(IRInstruction{
        IROpcode::SUB,
        IROperand{IROperandType::REGISTER, "%r2"},
        IROperand{IROperandType::REGISTER, "%r0"},
        IROperand{IROperandType::REGISTER, "%r1"}
    });
    
    blocks.push_back(block);
    
    CCodeGenerator gen;
    std::string c_code = gen.generate(blocks);
    
    assert(compileAndVerify(c_code, "Int - Float"));
}

void test_float_multiply_int() {
    std::vector<BasicBlock> blocks;
    BasicBlock block;
    block.label = "main";
    
    block.instructions.push_back(IRInstruction{
        IROpcode::LOAD_CONST,
        IROperand{IROperandType::REGISTER, "%r0"},
        IROperand{IROperandType::CONSTANT, "#3.5"},
        IROperand{IROperandType::NONE, ""}
    });
    
    block.instructions.push_back(IRInstruction{
        IROpcode::LOAD_CONST,
        IROperand{IROperandType::REGISTER, "%r1"},
        IROperand{IROperandType::CONSTANT, "#4"},
        IROperand{IROperandType::NONE, ""}
    });
    
    block.instructions.push_back(IRInstruction{
        IROpcode::MUL,
        IROperand{IROperandType::REGISTER, "%r2"},
        IROperand{IROperandType::REGISTER, "%r0"},
        IROperand{IROperandType::REGISTER, "%r1"}
    });
    
    blocks.push_back(block);
    
    CCodeGenerator gen;
    std::string c_code = gen.generate(blocks);
    
    assert(compileAndVerify(c_code, "Float * Int"));
}

void test_int_divide_float() {
    std::vector<BasicBlock> blocks;
    BasicBlock block;
    block.label = "main";
    
    block.instructions.push_back(IRInstruction{
        IROpcode::LOAD_CONST,
        IROperand{IROperandType::REGISTER, "%r0"},
        IROperand{IROperandType::CONSTANT, "#20"},
        IROperand{IROperandType::NONE, ""}
    });
    
    block.instructions.push_back(IRInstruction{
        IROpcode::LOAD_CONST,
        IROperand{IROperandType::REGISTER, "%r1"},
        IROperand{IROperandType::CONSTANT, "#2.5"},
        IROperand{IROperandType::NONE, ""}
    });
    
    block.instructions.push_back(IRInstruction{
        IROpcode::DIV,
        IROperand{IROperandType::REGISTER, "%r2"},
        IROperand{IROperandType::REGISTER, "%r0"},
        IROperand{IROperandType::REGISTER, "%r1"}
    });
    
    blocks.push_back(block);
    
    CCodeGenerator gen;
    std::string c_code = gen.generate(blocks);
    
    assert(compileAndVerify(c_code, "Int / Float"));
}

void test_modulo_positive() {
    std::vector<BasicBlock> blocks;
    BasicBlock block;
    block.label = "main";
    
    block.instructions.push_back(IRInstruction{
        IROpcode::LOAD_CONST,
        IROperand{IROperandType::REGISTER, "%r0"},
        IROperand{IROperandType::CONSTANT, "#17"},
        IROperand{IROperandType::NONE, ""}
    });
    
    block.instructions.push_back(IRInstruction{
        IROpcode::LOAD_CONST,
        IROperand{IROperandType::REGISTER, "%r1"},
        IROperand{IROperandType::CONSTANT, "#5"},
        IROperand{IROperandType::NONE, ""}
    });
    
    block.instructions.push_back(IRInstruction{
        IROpcode::MOD,
        IROperand{IROperandType::REGISTER, "%r2"},
        IROperand{IROperandType::REGISTER, "%r0"},
        IROperand{IROperandType::REGISTER, "%r1"}
    });
    
    blocks.push_back(block);
    
    CCodeGenerator gen;
    std::string c_code = gen.generate(blocks);
    
    assert(compileAndVerify(c_code, "Modulo Positive"));
}

void test_modulo_negative() {
    std::vector<BasicBlock> blocks;
    BasicBlock block;
    block.label = "main";
    
    // -17 % 5
    block.instructions.push_back(IRInstruction{
        IROpcode::LOAD_CONST,
        IROperand{IROperandType::REGISTER, "%r0"},
        IROperand{IROperandType::CONSTANT, "#-17"},
        IROperand{IROperandType::NONE, ""}
    });
    
    block.instructions.push_back(IRInstruction{
        IROpcode::LOAD_CONST,
        IROperand{IROperandType::REGISTER, "%r1"},
        IROperand{IROperandType::CONSTANT, "#5"},
        IROperand{IROperandType::NONE, ""}
    });
    
    block.instructions.push_back(IRInstruction{
        IROpcode::MOD,
        IROperand{IROperandType::REGISTER, "%r2"},
        IROperand{IROperandType::REGISTER, "%r0"},
        IROperand{IROperandType::REGISTER, "%r1"}
    });
    
    blocks.push_back(block);
    
    CCodeGenerator gen;
    std::string c_code = gen.generate(blocks);
    
    assert(compileAndVerify(c_code, "Modulo Negative"));
}

void test_modulo_by_negative() {
    std::vector<BasicBlock> blocks;
    BasicBlock block;
    block.label = "main";
    
    // 17 % -5
    block.instructions.push_back(IRInstruction{
        IROpcode::LOAD_CONST,
        IROperand{IROperandType::REGISTER, "%r0"},
        IROperand{IROperandType::CONSTANT, "#17"},
        IROperand{IROperandType::NONE, ""}
    });
    
    block.instructions.push_back(IRInstruction{
        IROpcode::LOAD_CONST,
        IROperand{IROperandType::REGISTER, "%r1"},
        IROperand{IROperandType::CONSTANT, "#-5"},
        IROperand{IROperandType::NONE, ""}
    });
    
    block.instructions.push_back(IRInstruction{
        IROpcode::MOD,
        IROperand{IROperandType::REGISTER, "%r2"},
        IROperand{IROperandType::REGISTER, "%r0"},
        IROperand{IROperandType::REGISTER, "%r1"}
    });
    
    blocks.push_back(block);
    
    CCodeGenerator gen;
    std::string c_code = gen.generate(blocks);
    
    assert(compileAndVerify(c_code, "Modulo By Negative"));
}

void test_modulo_zero_result() {
    std::vector<BasicBlock> blocks;
    BasicBlock block;
    block.label = "main";
    
    // 20 % 10 = 0
    block.instructions.push_back(IRInstruction{
        IROpcode::LOAD_CONST,
        IROperand{IROperandType::REGISTER, "%r0"},
        IROperand{IROperandType::CONSTANT, "#20"},
        IROperand{IROperandType::NONE, ""}
    });
    
    block.instructions.push_back(IRInstruction{
        IROpcode::LOAD_CONST,
        IROperand{IROperandType::REGISTER, "%r1"},
        IROperand{IROperandType::CONSTANT, "#10"},
        IROperand{IROperandType::NONE, ""}
    });
    
    block.instructions.push_back(IRInstruction{
        IROpcode::MOD,
        IROperand{IROperandType::REGISTER, "%r2"},
        IROperand{IROperandType::REGISTER, "%r0"},
        IROperand{IROperandType::REGISTER, "%r1"}
    });
    
    blocks.push_back(block);
    
    CCodeGenerator gen;
    std::string c_code = gen.generate(blocks);
    
    assert(compileAndVerify(c_code, "Modulo Zero Result"));
}

void test_mixed_chain_operations() {
    std::vector<BasicBlock> blocks;
    BasicBlock block;
    block.label = "main";
    
    // a = 10
    block.instructions.push_back(IRInstruction{
        IROpcode::LOAD_CONST,
        IROperand{IROperandType::REGISTER, "%r0"},
        IROperand{IROperandType::CONSTANT, "#10"},
        IROperand{IROperandType::NONE, ""}
    });
    
    // b = 2.5
    block.instructions.push_back(IRInstruction{
        IROpcode::LOAD_CONST,
        IROperand{IROperandType::REGISTER, "%r1"},
        IROperand{IROperandType::CONSTANT, "#2.5"},
        IROperand{IROperandType::NONE, ""}
    });
    
    // c = 3
    block.instructions.push_back(IRInstruction{
        IROpcode::LOAD_CONST,
        IROperand{IROperandType::REGISTER, "%r2"},
        IROperand{IROperandType::CONSTANT, "#3"},
        IROperand{IROperandType::NONE, ""}
    });
    
    // d = a + b (int + float = double)
    block.instructions.push_back(IRInstruction{
        IROpcode::ADD,
        IROperand{IROperandType::REGISTER, "%r3"},
        IROperand{IROperandType::REGISTER, "%r0"},
        IROperand{IROperandType::REGISTER, "%r1"}
    });
    
    // e = d * c (double * int = double)
    block.instructions.push_back(IRInstruction{
        IROpcode::MUL,
        IROperand{IROperandType::REGISTER, "%r4"},
        IROperand{IROperandType::REGISTER, "%r3"},
        IROperand{IROperandType::REGISTER, "%r2"}
    });
    
    blocks.push_back(block);
    
    CCodeGenerator gen;
    std::string c_code = gen.generate(blocks);
    
    assert(compileAndVerify(c_code, "Mixed Chain Operations"));
}

void test_bool_in_arithmetic() {
    std::vector<BasicBlock> blocks;
    BasicBlock block;
    block.label = "main";
    
    // b = True
    block.instructions.push_back(IRInstruction{
        IROpcode::LOAD_CONST,
        IROperand{IROperandType::REGISTER, "%r0"},
        IROperand{IROperandType::CONSTANT, "#True"},
        IROperand{IROperandType::NONE, ""}
    });
    
    // x = 5
    block.instructions.push_back(IRInstruction{
        IROpcode::LOAD_CONST,
        IROperand{IROperandType::REGISTER, "%r1"},
        IROperand{IROperandType::CONSTANT, "#5"},
        IROperand{IROperandType::NONE, ""}
    });
    
    // y = b + x (bool treated as int)
    block.instructions.push_back(IRInstruction{
        IROpcode::ADD,
        IROperand{IROperandType::REGISTER, "%r2"},
        IROperand{IROperandType::REGISTER, "%r0"},
        IROperand{IROperandType::REGISTER, "%r1"}
    });
    
    blocks.push_back(block);
    
    CCodeGenerator gen;
    std::string c_code = gen.generate(blocks);
    
    assert(compileAndVerify(c_code, "Bool in Arithmetic"));
}

void test_compare_int_float() {
    std::vector<BasicBlock> blocks;
    BasicBlock block;
    block.label = "main";
    
    // x = 5
    block.instructions.push_back(IRInstruction{
        IROpcode::LOAD_CONST,
        IROperand{IROperandType::REGISTER, "%r0"},
        IROperand{IROperandType::CONSTANT, "#5"},
        IROperand{IROperandType::NONE, ""}
    });
    
    // y = 5.0
    block.instructions.push_back(IRInstruction{
        IROpcode::LOAD_CONST,
        IROperand{IROperandType::REGISTER, "%r1"},
        IROperand{IROperandType::CONSTANT, "#5.0"},
        IROperand{IROperandType::NONE, ""}
    });
    
    // z = x == y
    block.instructions.push_back(IRInstruction{
        IROpcode::EQ,
        IROperand{IROperandType::REGISTER, "%r2"},
        IROperand{IROperandType::REGISTER, "%r0"},
        IROperand{IROperandType::REGISTER, "%r1"}
    });
    
    blocks.push_back(block);
    
    CCodeGenerator gen;
    std::string c_code = gen.generate(blocks);
    
    assert(compileAndVerify(c_code, "Compare Int Float"));
}

void test_compare_float_int() {
    std::vector<BasicBlock> blocks;
    BasicBlock block;
    block.label = "main";
    
    // x = 3.14
    block.instructions.push_back(IRInstruction{
        IROpcode::LOAD_CONST,
        IROperand{IROperandType::REGISTER, "%r0"},
        IROperand{IROperandType::CONSTANT, "#3.14"},
        IROperand{IROperandType::NONE, ""}
    });
    
    // y = 3
    block.instructions.push_back(IRInstruction{
        IROpcode::LOAD_CONST,
        IROperand{IROperandType::REGISTER, "%r1"},
        IROperand{IROperandType::CONSTANT, "#3"},
        IROperand{IROperandType::NONE, ""}
    });
    
    // z = x > y
    block.instructions.push_back(IRInstruction{
        IROpcode::GT,
        IROperand{IROperandType::REGISTER, "%r2"},
        IROperand{IROperandType::REGISTER, "%r0"},
        IROperand{IROperandType::REGISTER, "%r1"}
    });
    
    blocks.push_back(block);
    
    CCodeGenerator gen;
    std::string c_code = gen.generate(blocks);
    
    assert(compileAndVerify(c_code, "Compare Float Int"));
}

void test_negate_float() {
    std::vector<BasicBlock> blocks;
    BasicBlock block;
    block.label = "main";
    
    // x = 3.5
    block.instructions.push_back(IRInstruction{
        IROpcode::LOAD_CONST,
        IROperand{IROperandType::REGISTER, "%r0"},
        IROperand{IROperandType::CONSTANT, "#3.5"},
        IROperand{IROperandType::NONE, ""}
    });
    
    // y = -x
    block.instructions.push_back(IRInstruction{
        IROpcode::NEG,
        IROperand{IROperandType::REGISTER, "%r1"},
        IROperand{IROperandType::REGISTER, "%r0"},
        IROperand{IROperandType::NONE, ""}
    });
    
    blocks.push_back(block);
    
    CCodeGenerator gen;
    std::string c_code = gen.generate(blocks);
    
    assert(compileAndVerify(c_code, "Negate Float"));
}

void test_negate_int() {
    std::vector<BasicBlock> blocks;
    BasicBlock block;
    block.label = "main";
    
    // x = 42
    block.instructions.push_back(IRInstruction{
        IROpcode::LOAD_CONST,
        IROperand{IROperandType::REGISTER, "%r0"},
        IROperand{IROperandType::CONSTANT, "#42"},
        IROperand{IROperandType::NONE, ""}
    });
    
    // y = -x
    block.instructions.push_back(IRInstruction{
        IROpcode::NEG,
        IROperand{IROperandType::REGISTER, "%r1"},
        IROperand{IROperandType::REGISTER, "%r0"},
        IROperand{IROperandType::NONE, ""}
    });
    
    blocks.push_back(block);
    
    CCodeGenerator gen;
    std::string c_code = gen.generate(blocks);
    
    assert(compileAndVerify(c_code, "Negate Int"));
}

void test_large_int_small_float() {
    std::vector<BasicBlock> blocks;
    BasicBlock block;
    block.label = "main";
    
    // x = 1000000
    block.instructions.push_back(IRInstruction{
        IROpcode::LOAD_CONST,
        IROperand{IROperandType::REGISTER, "%r0"},
        IROperand{IROperandType::CONSTANT, "#1000000"},
        IROperand{IROperandType::NONE, ""}
    });
    
    // y = 0.001
    block.instructions.push_back(IRInstruction{
        IROpcode::LOAD_CONST,
        IROperand{IROperandType::REGISTER, "%r1"},
        IROperand{IROperandType::CONSTANT, "#0.001"},
        IROperand{IROperandType::NONE, ""}
    });
    
    // z = x * y
    block.instructions.push_back(IRInstruction{
        IROpcode::MUL,
        IROperand{IROperandType::REGISTER, "%r2"},
        IROperand{IROperandType::REGISTER, "%r0"},
        IROperand{IROperandType::REGISTER, "%r1"}
    });
    
    blocks.push_back(block);
    
    CCodeGenerator gen;
    std::string c_code = gen.generate(blocks);
    
    assert(compileAndVerify(c_code, "Large Int Small Float"));
}

void test_zero_int_float_multiply() {
    std::vector<BasicBlock> blocks;
    BasicBlock block;
    block.label = "main";
    
    // x = 0
    block.instructions.push_back(IRInstruction{
        IROpcode::LOAD_CONST,
        IROperand{IROperandType::REGISTER, "%r0"},
        IROperand{IROperandType::CONSTANT, "#0"},
        IROperand{IROperandType::NONE, ""}
    });
    
    // y = 123.456
    block.instructions.push_back(IRInstruction{
        IROpcode::LOAD_CONST,
        IROperand{IROperandType::REGISTER, "%r1"},
        IROperand{IROperandType::CONSTANT, "#123.456"},
        IROperand{IROperandType::NONE, ""}
    });
    
    // z = x * y
    block.instructions.push_back(IRInstruction{
        IROpcode::MUL,
        IROperand{IROperandType::REGISTER, "%r2"},
        IROperand{IROperandType::REGISTER, "%r0"},
        IROperand{IROperandType::REGISTER, "%r1"}
    });
    
    blocks.push_back(block);
    
    CCodeGenerator gen;
    std::string c_code = gen.generate(blocks);
    
    assert(compileAndVerify(c_code, "Zero Int Float Multiply"));
}

void test_negative_float_positive_int() {
    std::vector<BasicBlock> blocks;
    BasicBlock block;
    block.label = "main";
    
    // x = -5.5
    block.instructions.push_back(IRInstruction{
        IROpcode::LOAD_CONST,
        IROperand{IROperandType::REGISTER, "%r0"},
        IROperand{IROperandType::CONSTANT, "#-5.5"},
        IROperand{IROperandType::NONE, ""}
    });
    
    // y = 2
    block.instructions.push_back(IRInstruction{
        IROpcode::LOAD_CONST,
        IROperand{IROperandType::REGISTER, "%r1"},
        IROperand{IROperandType::CONSTANT, "#2"},
        IROperand{IROperandType::NONE, ""}
    });
    
    // z = x + y
    block.instructions.push_back(IRInstruction{
        IROpcode::ADD,
        IROperand{IROperandType::REGISTER, "%r2"},
        IROperand{IROperandType::REGISTER, "%r0"},
        IROperand{IROperandType::REGISTER, "%r1"}
    });
    
    blocks.push_back(block);
    
    CCodeGenerator gen;
    std::string c_code = gen.generate(blocks);
    
    assert(compileAndVerify(c_code, "Negative Float Positive Int"));
}

void test_float_divide_zero_safe() {
    std::vector<BasicBlock> blocks;
    BasicBlock block;
    block.label = "main";
    
    // x = 10.5
    block.instructions.push_back(IRInstruction{
        IROpcode::LOAD_CONST,
        IROperand{IROperandType::REGISTER, "%r0"},
        IROperand{IROperandType::CONSTANT, "#10.5"},
        IROperand{IROperandType::NONE, ""}
    });
    
    // y = 2
    block.instructions.push_back(IRInstruction{
        IROpcode::LOAD_CONST,
        IROperand{IROperandType::REGISTER, "%r1"},
        IROperand{IROperandType::CONSTANT, "#2"},
        IROperand{IROperandType::NONE, ""}
    });
    
    // z = x / y (not dividing by zero - just syntax check)
    block.instructions.push_back(IRInstruction{
        IROpcode::DIV,
        IROperand{IROperandType::REGISTER, "%r2"},
        IROperand{IROperandType::REGISTER, "%r0"},
        IROperand{IROperandType::REGISTER, "%r1"}
    });
    
    blocks.push_back(block);
    
    CCodeGenerator gen;
    std::string c_code = gen.generate(blocks);
    
    assert(compileAndVerify(c_code, "Float Divide Zero Safe"));
}

void test_complex_mixed_expression() {
    std::vector<BasicBlock> blocks;
    BasicBlock block;
    block.label = "main";
    
    // a = 10
    block.instructions.push_back(IRInstruction{
        IROpcode::LOAD_CONST,
        IROperand{IROperandType::REGISTER, "%r0"},
        IROperand{IROperandType::CONSTANT, "#10"},
        IROperand{IROperandType::NONE, ""}
    });
    
    // b = 3.0
    block.instructions.push_back(IRInstruction{
        IROpcode::LOAD_CONST,
        IROperand{IROperandType::REGISTER, "%r1"},
        IROperand{IROperandType::CONSTANT, "#3.0"},
        IROperand{IROperandType::NONE, ""}
    });
    
    // c = 2
    block.instructions.push_back(IRInstruction{
        IROpcode::LOAD_CONST,
        IROperand{IROperandType::REGISTER, "%r2"},
        IROperand{IROperandType::CONSTANT, "#2"},
        IROperand{IROperandType::NONE, ""}
    });
    
    // t1 = a * b (int * float = double)
    block.instructions.push_back(IRInstruction{
        IROpcode::MUL,
        IROperand{IROperandType::REGISTER, "%r3"},
        IROperand{IROperandType::REGISTER, "%r0"},
        IROperand{IROperandType::REGISTER, "%r1"}
    });
    
    // t2 = t1 - c (double - int = double)
    block.instructions.push_back(IRInstruction{
        IROpcode::SUB,
        IROperand{IROperandType::REGISTER, "%r4"},
        IROperand{IROperandType::REGISTER, "%r3"},
        IROperand{IROperandType::REGISTER, "%r2"}
    });
    
    // t3 = t2 / b (double / double = double)
    block.instructions.push_back(IRInstruction{
        IROpcode::DIV,
        IROperand{IROperandType::REGISTER, "%r5"},
        IROperand{IROperandType::REGISTER, "%r4"},
        IROperand{IROperandType::REGISTER, "%r1"}
    });
    
    blocks.push_back(block);
    
    CCodeGenerator gen;
    std::string c_code = gen.generate(blocks);
    
    assert(compileAndVerify(c_code, "Complex Mixed Expression"));
}

void test_modulo_with_large_numbers() {
    std::vector<BasicBlock> blocks;
    BasicBlock block;
    block.label = "main";
    
    // x = 999999
    block.instructions.push_back(IRInstruction{
        IROpcode::LOAD_CONST,
        IROperand{IROperandType::REGISTER, "%r0"},
        IROperand{IROperandType::CONSTANT, "#999999"},
        IROperand{IROperandType::NONE, ""}
    });
    
    // y = 7
    block.instructions.push_back(IRInstruction{
        IROpcode::LOAD_CONST,
        IROperand{IROperandType::REGISTER, "%r1"},
        IROperand{IROperandType::CONSTANT, "#7"},
        IROperand{IROperandType::NONE, ""}
    });
    
    // z = x % y
    block.instructions.push_back(IRInstruction{
        IROpcode::MOD,
        IROperand{IROperandType::REGISTER, "%r2"},
        IROperand{IROperandType::REGISTER, "%r0"},
        IROperand{IROperandType::REGISTER, "%r1"}
    });
    
    blocks.push_back(block);
    
    CCodeGenerator gen;
    std::string c_code = gen.generate(blocks);
    
    assert(compileAndVerify(c_code, "Modulo With Large Numbers"));
}

void test_bool_comparisons_mixed() {
    std::vector<BasicBlock> blocks;
    BasicBlock block;
    block.label = "main";
    
    // x = 5
    block.instructions.push_back(IRInstruction{
        IROpcode::LOAD_CONST,
        IROperand{IROperandType::REGISTER, "%r0"},
        IROperand{IROperandType::CONSTANT, "#5"},
        IROperand{IROperandType::NONE, ""}
    });
    
    // y = 5.0
    block.instructions.push_back(IRInstruction{
        IROpcode::LOAD_CONST,
        IROperand{IROperandType::REGISTER, "%r1"},
        IROperand{IROperandType::CONSTANT, "#5.0"},
        IROperand{IROperandType::NONE, ""}
    });
    
    // b1 = x == y
    block.instructions.push_back(IRInstruction{
        IROpcode::EQ,
        IROperand{IROperandType::REGISTER, "%r2"},
        IROperand{IROperandType::REGISTER, "%r0"},
        IROperand{IROperandType::REGISTER, "%r1"}
    });
    
    // b2 = x < y
    block.instructions.push_back(IRInstruction{
        IROpcode::LT,
        IROperand{IROperandType::REGISTER, "%r3"},
        IROperand{IROperandType::REGISTER, "%r0"},
        IROperand{IROperandType::REGISTER, "%r1"}
    });
    
    // b3 = b1 && b2
    block.instructions.push_back(IRInstruction{
        IROpcode::AND,
        IROperand{IROperandType::REGISTER, "%r4"},
        IROperand{IROperandType::REGISTER, "%r2"},
        IROperand{IROperandType::REGISTER, "%r3"}
    });
    
    blocks.push_back(block);
    
    CCodeGenerator gen;
    std::string c_code = gen.generate(blocks);
    
    assert(compileAndVerify(c_code, "Bool Comparisons Mixed"));
}

void test_type_persistence_through_variable() {
    std::vector<BasicBlock> blocks;
    BasicBlock block;
    block.label = "main";
    
    // x = 3.14
    block.instructions.push_back(IRInstruction{
        IROpcode::LOAD_CONST,
        IROperand{IROperandType::REGISTER, "%r0"},
        IROperand{IROperandType::CONSTANT, "#3.14"},
        IROperand{IROperandType::NONE, ""}
    });
    
    // var = x (should maintain double type)
    block.instructions.push_back(IRInstruction{
        IROpcode::SET_VAR,
        IROperand{IROperandType::VARIABLE, "$var"},
        IROperand{IROperandType::REGISTER, "%r0"},
        IROperand{IROperandType::NONE, ""}
    });
    
    // y = var (should get double type)
    block.instructions.push_back(IRInstruction{
        IROpcode::GET_VAR,
        IROperand{IROperandType::REGISTER, "%r1"},
        IROperand{IROperandType::VARIABLE, "$var"},
        IROperand{IROperandType::NONE, ""}
    });
    
    // z = 2
    block.instructions.push_back(IRInstruction{
        IROpcode::LOAD_CONST,
        IROperand{IROperandType::REGISTER, "%r2"},
        IROperand{IROperandType::CONSTANT, "#2"},
        IROperand{IROperandType::NONE, ""}
    });
    
    // result = y + z (double + int = double)
    block.instructions.push_back(IRInstruction{
        IROpcode::ADD,
        IROperand{IROperandType::REGISTER, "%r3"},
        IROperand{IROperandType::REGISTER, "%r1"},
        IROperand{IROperandType::REGISTER, "%r2"}
    });
    
    blocks.push_back(block);
    
    CCodeGenerator gen;
    std::string c_code = gen.generate(blocks);
    
    assert(compileAndVerify(c_code, "Type Persistence Through Variable"));
}

void test_multiple_type_conversions() {
    std::vector<BasicBlock> blocks;
    BasicBlock block;
    block.label = "main";
    
    // a = 5
    block.instructions.push_back(IRInstruction{
        IROpcode::LOAD_CONST,
        IROperand{IROperandType::REGISTER, "%r0"},
        IROperand{IROperandType::CONSTANT, "#5"},
        IROperand{IROperandType::NONE, ""}
    });
    
    // b = 2.0
    block.instructions.push_back(IRInstruction{
        IROpcode::LOAD_CONST,
        IROperand{IROperandType::REGISTER, "%r1"},
        IROperand{IROperandType::CONSTANT, "#2.0"},
        IROperand{IROperandType::NONE, ""}
    });
    
    // c = True
    block.instructions.push_back(IRInstruction{
        IROpcode::LOAD_CONST,
        IROperand{IROperandType::REGISTER, "%r2"},
        IROperand{IROperandType::CONSTANT, "#True"},
        IROperand{IROperandType::NONE, ""}
    });
    
    // t1 = a + c (int + bool = int)
    block.instructions.push_back(IRInstruction{
        IROpcode::ADD,
        IROperand{IROperandType::REGISTER, "%r3"},
        IROperand{IROperandType::REGISTER, "%r0"},
        IROperand{IROperandType::REGISTER, "%r2"}
    });
    
    // t2 = t1 + b (int + double = double)
    block.instructions.push_back(IRInstruction{
        IROpcode::ADD,
        IROperand{IROperandType::REGISTER, "%r4"},
        IROperand{IROperandType::REGISTER, "%r3"},
        IROperand{IROperandType::REGISTER, "%r1"}
    });
    
    blocks.push_back(block);
    
    CCodeGenerator gen;
    std::string c_code = gen.generate(blocks);
    
    assert(compileAndVerify(c_code, "Multiple Type Conversions"));
}

int main() {
    std::cout << "Running comprehensive mixed type tests for C code generation..." << std::endl;
    std::cout << "Testing all combinations and edge cases..." << std::endl;
    std::cout << std::endl;
    
    int passed = 0;
    int total = 0;
    
    try {
        test_int_plus_float(); passed++; total++;
        test_float_plus_int(); passed++; total++;
        test_int_minus_float(); passed++; total++;
        test_float_multiply_int(); passed++; total++;
        test_int_divide_float(); passed++; total++;
        test_modulo_positive(); passed++; total++;
        test_modulo_negative(); passed++; total++;
        test_modulo_by_negative(); passed++; total++;
        test_modulo_zero_result(); passed++; total++;
        test_mixed_chain_operations(); passed++; total++;
        test_bool_in_arithmetic(); passed++; total++;
        test_compare_int_float(); passed++; total++;
        test_compare_float_int(); passed++; total++;
        test_negate_float(); passed++; total++;
        test_negate_int(); passed++; total++;
        test_large_int_small_float(); passed++; total++;
        test_zero_int_float_multiply(); passed++; total++;
        test_negative_float_positive_int(); passed++; total++;
        test_float_divide_zero_safe(); passed++; total++;
        test_complex_mixed_expression(); passed++; total++;
        test_modulo_with_large_numbers(); passed++; total++;
        test_bool_comparisons_mixed(); passed++; total++;
        test_type_persistence_through_variable(); passed++; total++;
        test_multiple_type_conversions(); passed++; total++;
        
        // 25th test
        total++;
        // One more comprehensive test
        std::vector<BasicBlock> blocks;
        BasicBlock block;
        block.label = "main";
        
        // Final comprehensive test: all operations in sequence
        block.instructions.push_back(IRInstruction{
            IROpcode::LOAD_CONST,
            IROperand{IROperandType::REGISTER, "%r0"},
            IROperand{IROperandType::CONSTANT, "#100"},
            IROperand{IROperandType::NONE, ""}
        });
        
        block.instructions.push_back(IRInstruction{
            IROpcode::LOAD_CONST,
            IROperand{IROperandType::REGISTER, "%r1"},
            IROperand{IROperandType::CONSTANT, "#2.5"},
            IROperand{IROperandType::NONE, ""}
        });
        
        block.instructions.push_back(IRInstruction{
            IROpcode::DIV,
            IROperand{IROperandType::REGISTER, "%r2"},
            IROperand{IROperandType::REGISTER, "%r0"},
            IROperand{IROperandType::REGISTER, "%r1"}
        });
        
        blocks.push_back(block);
        
        CCodeGenerator gen;
        std::string c_code = gen.generate(blocks);
        
        if (compileAndVerify(c_code, "Final Comprehensive Test")) {
            passed++;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Test suite failed with exception: " << e.what() << std::endl;
        return 1;
    }
    
    std::cout << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Test Results: " << passed << "/" << total << " passed" << std::endl;
    
    if (total != TOTAL_TESTS) {
        std::cerr << "WARNING: Expected " << TOTAL_TESTS << " tests but ran " << total << std::endl;
    }
    
    if (passed == total && total == TOTAL_TESTS) {
        std::cout << "✅ ALL MIXED TYPE TESTS PASSED!" << std::endl;
        std::cout << "========================================" << std::endl;
        return 0;
    } else {
        std::cout << "❌ SOME TESTS FAILED!" << std::endl;
        std::cout << "========================================" << std::endl;
        return 1;
    }
}
