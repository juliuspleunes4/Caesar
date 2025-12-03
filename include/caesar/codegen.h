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
    // Output buffers
    int indent_level;
    std::stringstream functions_output;  // Separate buffer for function definitions
    std::stringstream main_output;       // Buffer for main() code
    
    // Type and state tracking
    std::unordered_map<std::string, std::string> variable_types;
    std::vector<std::string> pending_params;  // For tracking PARAM instructions before CALL
    
    // Function registry for two-pass generation
    struct FunctionInfo {
        std::string name;                    // e.g., "func_add"
        std::vector<std::string> parameters; // Parameter names
        std::string return_type;             // Default: "int64_t"
        std::vector<IRInstruction> body;     // Function body instructions
    };
    std::unordered_map<std::string, FunctionInfo> function_registry;
    
    // State tracking for function generation
    bool in_function;
    std::string current_function_name;
    
    // Existing helper methods
    void emitLine(const std::string& line);
    void emitInstruction(const IRInstruction& instr);
    std::string indent() const;
    std::string convertConstant(const std::string& value) const;
    bool isStringLiteral(const std::string& value) const;
    bool isFloatLiteral(const std::string& value) const;
    std::string getResultType(const std::string& type1, const std::string& type2) const;
    std::string sanitizeName(const std::string& name) const;
    std::string getCaesarType(const std::string& ir_operand) const;
    std::string escapeCString(const std::string& str) const;
    
    // NEW: Function support helper methods
    bool isFunctionBlock(const BasicBlock& block) const;
    void registerFunction(const BasicBlock& block);
    void emitFunctionDefinition(const FunctionInfo& func);
    void emitFunctionCall(const std::string& func_name, const std::vector<std::string>& args, const std::string& dest_reg);
    
public:
    CCodeGenerator() : CodeGenerator(TargetArch::BYTECODE), indent_level(0), in_function(false) {}
    
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
