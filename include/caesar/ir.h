/**
 * @file ir.h
 * @brief Caesar Intermediate Representation (IR)
 * @author J.J.G. Pleunes
 * @version 1.5.1
 */

#ifndef CAESAR_IR_H
#define CAESAR_IR_H

#include "caesar/ast.h"
#include "caesar/caesar.h"
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include <sstream>

namespace caesar {

/**
 * @brief IR instruction opcodes
 */
enum class IROpcode {
    // Arithmetic
    ADD, SUB, MUL, DIV, MOD, NEG,
    
    // Comparison
    EQ, NE, LT, LE, GT, GE,
    
    // Logical
    AND, OR, NOT,
    
    // Memory operations
    LOAD, STORE, LOAD_CONST, ALLOC,
    
    // Control flow
    LABEL, JUMP, JUMP_IF_TRUE, JUMP_IF_FALSE, CALL, RETURN,
    
    // Variables
    DECLARE, ASSIGN, GET_VAR, SET_VAR,
    
    // Special
    PRINT, PARAM, NOP
};

/**
 * @brief Convert opcode to string
 */
inline std::string opcodeToString(IROpcode opcode) {
    switch (opcode) {
        case IROpcode::ADD: return "ADD";
        case IROpcode::SUB: return "SUB";
        case IROpcode::MUL: return "MUL";
        case IROpcode::DIV: return "DIV";
        case IROpcode::MOD: return "MOD";
        case IROpcode::NEG: return "NEG";
        case IROpcode::EQ: return "EQ";
        case IROpcode::NE: return "NE";
        case IROpcode::LT: return "LT";
        case IROpcode::LE: return "LE";
        case IROpcode::GT: return "GT";
        case IROpcode::GE: return "GE";
        case IROpcode::AND: return "AND";
        case IROpcode::OR: return "OR";
        case IROpcode::NOT: return "NOT";
        case IROpcode::LOAD: return "LOAD";
        case IROpcode::STORE: return "STORE";
        case IROpcode::LOAD_CONST: return "LOAD_CONST";
        case IROpcode::ALLOC: return "ALLOC";
        case IROpcode::LABEL: return "LABEL";
        case IROpcode::JUMP: return "JUMP";
        case IROpcode::JUMP_IF_TRUE: return "JUMP_IF_TRUE";
        case IROpcode::JUMP_IF_FALSE: return "JUMP_IF_FALSE";
        case IROpcode::CALL: return "CALL";
        case IROpcode::RETURN: return "RETURN";
        case IROpcode::DECLARE: return "DECLARE";
        case IROpcode::ASSIGN: return "ASSIGN";
        case IROpcode::GET_VAR: return "GET_VAR";
        case IROpcode::SET_VAR: return "SET_VAR";
        case IROpcode::PRINT: return "PRINT";
        case IROpcode::PARAM: return "PARAM";
        case IROpcode::NOP: return "NOP";
        default: return "UNKNOWN";
    }
}

/**
 * @brief IR operand types
 */
enum class IROperandType {
    NONE,
    REGISTER,
    CONSTANT,
    LABEL,
    VARIABLE
};

/**
 * @brief IR operand
 */
struct IROperand {
    IROperandType type;
    std::string value;
    
    IROperand() : type(IROperandType::NONE), value("") {}
    IROperand(IROperandType t, const std::string& v) : type(t), value(v) {}
    
    static IROperand Reg(int n) { return IROperand(IROperandType::REGISTER, "r" + std::to_string(n)); }
    static IROperand Const(const std::string& v) { return IROperand(IROperandType::CONSTANT, v); }
    static IROperand Lab(const std::string& l) { return IROperand(IROperandType::LABEL, l); }
    static IROperand Var(const std::string& n) { return IROperand(IROperandType::VARIABLE, n); }
    
    std::string toString() const {
        switch (type) {
            case IROperandType::REGISTER: return "%" + value;
            case IROperandType::CONSTANT: return "#" + value;
            case IROperandType::LABEL: return "@" + value;
            case IROperandType::VARIABLE: return "$" + value;
            default: return "";
        }
    }
};

/**
 * @brief Single IR instruction (three-address code)
 */
struct IRInstruction {
    IROpcode opcode;
    IROperand dest;       // Destination operand
    IROperand src1;       // First source operand
    IROperand src2;       // Second source operand
    
    IRInstruction(IROpcode op) 
        : opcode(op) {}
    
    IRInstruction(IROpcode op, IROperand d, IROperand s1 = IROperand(), IROperand s2 = IROperand())
        : opcode(op), dest(d), src1(s1), src2(s2) {}
    
    std::string toString() const {
        std::stringstream ss;
        ss << opcodeToString(opcode);
        
        if (dest.type != IROperandType::NONE) {
            ss << " " << dest.toString();
        }
        if (src1.type != IROperandType::NONE) {
            ss << ", " << src1.toString();
        }
        if (src2.type != IROperandType::NONE) {
            ss << ", " << src2.toString();
        }
        
        return ss.str();
    }
};

/**
 * @brief Basic block of IR instructions
 */
struct BasicBlock {
    std::string label;
    std::vector<IRInstruction> instructions;
    
    explicit BasicBlock(const std::string& l = "") : label(l) {}
    
    void addInstruction(const IRInstruction& instr) {
        instructions.push_back(instr);
    }
    
    std::string toString() const {
        std::stringstream ss;
        if (!label.empty()) {
            ss << label << ":\n";
        }
        for (const auto& instr : instructions) {
            ss << "    " << instr.toString() << "\n";
        }
        return ss.str();
    }
};

/**
 * @brief IR generation class - converts AST to IR
 */
class IRGenerator : public ASTVisitor {
private:
    std::vector<BasicBlock> blocks;
    BasicBlock* current_block;
    int next_register;
    int next_label;
    std::string last_register;  // Last generated register for expressions
    
    // Loop context tracking for break/continue statements
    struct LoopContext {
        std::string continue_label;  // Label to jump to for continue
        std::string break_label;     // Label to jump to for break
    };
    std::vector<LoopContext> loop_stack;
    
    // Generate unique register name
    std::string newRegister() {
        return "r" + std::to_string(next_register++);
    }
    
    // Generate unique label name
    std::string newLabel(const std::string& prefix = "L") {
        return prefix + std::to_string(next_label++);
    }
    
    // Emit instruction to current block
    void emit(const IRInstruction& instr) {
        if (current_block) {
            current_block->addInstruction(instr);
        }
    }
    
    // Create new basic block
    BasicBlock* newBlock(const std::string& label = "") {
        blocks.emplace_back(label);
        current_block = &blocks.back();
        return current_block;
    }
    
    // Push loop context for break/continue handling
    void pushLoopContext(const std::string& continue_label, const std::string& break_label) {
        loop_stack.push_back({continue_label, break_label});
    }
    
    // Pop loop context
    void popLoopContext() {
        if (!loop_stack.empty()) {
            loop_stack.pop_back();
        }
    }
    
    // Get current loop's continue label
    std::string getContinueLabel() const {
        return loop_stack.empty() ? "" : loop_stack.back().continue_label;
    }
    
    // Get current loop's break label
    std::string getBreakLabel() const {
        return loop_stack.empty() ? "" : loop_stack.back().break_label;
    }

public:
    IRGenerator() : current_block(nullptr), next_register(0), next_label(0) {}
    
    /**
     * @brief Generate IR from AST
     */
    std::vector<BasicBlock> generate(Program* program);
    
    /**
     * @brief Get generated IR as string
     */
    std::string toString() const;
    
    // Expression visitors
    void visit(LiteralExpression& node) override;
    void visit(IdentifierExpression& node) override;
    void visit(BinaryExpression& node) override;
    void visit(UnaryExpression& node) override;
    void visit(CallExpression& node) override;
    void visit(MemberExpression& node) override;
    void visit(AssignmentExpression& node) override;
    void visit(ListExpression& node) override;
    void visit(DictExpression& node) override;
    
    // Statement visitors
    void visit(ExpressionStatement& node) override;
    void visit(BlockStatement& node) override;
    void visit(IfStatement& node) override;
    void visit(WhileStatement& node) override;
    void visit(ForStatement& node) override;
    void visit(FunctionDefinition& node) override;
    void visit(ClassDefinition& node) override;
    void visit(ReturnStatement& node) override;
    void visit(BreakStatement& node) override;
    void visit(ContinueStatement& node) override;
    void visit(PassStatement& node) override;
    void visit(Program& node) override;
};

} // namespace caesar

#endif // CAESAR_IR_H
