/**
 * @file codegen.h
 * @brief Caesar Code Generation
 * @author J.J.G. Pleunes
 * @version 1.5.1
 */

#ifndef CAESAR_CODEGEN_H
#define CAESAR_CODEGEN_H

#include "caesar/ir.h"
#include "caesar/caesar.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <cstdint>

namespace caesar {

/**
 * @brief Target architecture
 */
enum class TargetArch {
    X86_64,
    ARM64,
    BYTECODE  // Virtual machine bytecode
};

/**
 * @brief Code generator base class
 */
class CodeGenerator {
protected:
    std::vector<BasicBlock> ir_blocks;
    TargetArch target_arch;
    std::stringstream output;
    
public:
    explicit CodeGenerator(TargetArch arch = TargetArch::BYTECODE)
        : target_arch(arch) {}
    
    virtual ~CodeGenerator() = default;
    
    /**
     * @brief Generate code from IR
     */
    virtual std::string generate(const std::vector<BasicBlock>& blocks) = 0;
    
    /**
     * @brief Get generated code
     */
    std::string getOutput() const { return output.str(); }
};

/**
 * @brief Bytecode generator (stack-based virtual machine)
 */
class BytecodeGenerator : public CodeGenerator {
private:
    int stack_size;
    std::unordered_map<std::string, int> label_positions;
    
    void emitByte(uint8_t byte);
    void emitOpcode(const std::string& opcode);
    void emitOperand(const IROperand& operand);
    
public:
    BytecodeGenerator() : CodeGenerator(TargetArch::BYTECODE), stack_size(0) {}
    
    std::string generate(const std::vector<BasicBlock>& blocks) override;
};

/**
 * @brief x86-64 assembly generator
 */
class X86_64Generator : public CodeGenerator {
private:
    std::unordered_map<std::string, std::string> register_map;
    int stack_offset;
    
    void emitPrologue();
    void emitEpilogue();
    void emitInstruction(const IRInstruction& instr);
    std::string allocateRegister(const std::string& virtual_reg);
    
public:
    X86_64Generator() : CodeGenerator(TargetArch::X86_64), stack_offset(0) {}
    
    std::string generate(const std::vector<BasicBlock>& blocks) override;
};

/**
 * @brief C code generator (transpiler)
 */
class CCodeGenerator : public CodeGenerator {
private:
    int indent_level;
    std::unordered_map<std::string, std::string> variable_types;
    
    void emitLine(const std::string& line);
    void emitInstruction(const IRInstruction& instr);
    std::string indent() const;
    std::string convertConstant(const std::string& value) const;
    bool isStringLiteral(const std::string& value) const;
    
public:
    CCodeGenerator() : CodeGenerator(TargetArch::BYTECODE), indent_level(0) {}
    
    std::string generate(const std::vector<BasicBlock>& blocks) override;
};

/**
 * @brief Factory for creating code generators
 */
class CodeGeneratorFactory {
public:
    static std::unique_ptr<CodeGenerator> create(TargetArch arch) {
        switch (arch) {
            case TargetArch::X86_64:
                return std::make_unique<X86_64Generator>();
            case TargetArch::BYTECODE:
                return std::make_unique<BytecodeGenerator>();
            default:
                return std::make_unique<BytecodeGenerator>();
        }
    }
    
    static std::unique_ptr<CCodeGenerator> createCGenerator() {
        return std::make_unique<CCodeGenerator>();
    }
};

} // namespace caesar

#endif // CAESAR_CODEGEN_H
