/**
 * @file ir.cpp
 * @brief Caesar IR Generator implementation
 * @author J.J.G. Pleunes
 * @version 1.5.1
 */

#include "caesar/ir.h"
#include "caesar/token.h"
#include <sstream>

namespace caesar {

std::vector<BasicBlock> IRGenerator::generate(Program* program) {
    blocks.clear();
    next_register = 0;
    next_label = 0;
    
    // Create entry block
    newBlock("entry");
    
    // Generate IR for program
    program->accept(*this);
    
    return blocks;
}

std::string IRGenerator::toString() const {
    std::stringstream ss;
    for (const auto& block : blocks) {
        ss << block.toString();
    }
    return ss.str();
}

// Expression visitors

void IRGenerator::visit(LiteralExpression& node) {
    // Load constant into register
    std::string reg = newRegister();
    std::string value;
    
    switch (node.value.type) {
        case TokenType::INTEGER:
        case TokenType::FLOAT:
            value = node.value.value;
            break;
        case TokenType::STRING:
            value = "\"" + node.value.value + "\"";
            break;
        case TokenType::BOOLEAN:
            value = node.value.value;  // "True" or "False"
            break;
        case TokenType::NONE:
            value = "null";
            break;
        default:
            value = node.value.value;
    }
    
    emit(IRInstruction(IROpcode::LOAD_CONST, IROperand::Reg(next_register - 1), IROperand::Const(value)));
    last_register = reg;
}

void IRGenerator::visit(IdentifierExpression& node) {
    // Load variable value into register
    std::string reg = newRegister();
    emit(IRInstruction(IROpcode::GET_VAR, IROperand::Reg(next_register - 1), IROperand::Var(node.name)));
    last_register = reg;
}

void IRGenerator::visit(BinaryExpression& node) {
    // Evaluate left operand
    node.left->accept(*this);
    std::string left_reg = last_register;
    
    // Evaluate right operand
    node.right->accept(*this);
    std::string right_reg = last_register;
    
    // Perform operation and store result
    std::string result_reg = newRegister();
    IROpcode opcode;
    
    switch (node.operator_type) {
        case TokenType::PLUS: opcode = IROpcode::ADD; break;
        case TokenType::MINUS: opcode = IROpcode::SUB; break;
        case TokenType::MULTIPLY: opcode = IROpcode::MUL; break;
        case TokenType::DIVIDE: opcode = IROpcode::DIV; break;
        case TokenType::MODULO: opcode = IROpcode::MOD; break;
        case TokenType::EQUAL: opcode = IROpcode::EQ; break;
        case TokenType::NOT_EQUAL: opcode = IROpcode::NE; break;
        case TokenType::LESS: opcode = IROpcode::LT; break;
        case TokenType::LESS_EQUAL: opcode = IROpcode::LE; break;
        case TokenType::GREATER: opcode = IROpcode::GT; break;
        case TokenType::GREATER_EQUAL: opcode = IROpcode::GE; break;
        case TokenType::AND: opcode = IROpcode::AND; break;
        case TokenType::OR: opcode = IROpcode::OR; break;
        default:
            throw IRException("Unsupported binary operator in IR generation");
    }
    
    emit(IRInstruction(opcode, 
                      IROperand(IROperandType::REGISTER, result_reg),
                      IROperand(IROperandType::REGISTER, left_reg),
                      IROperand(IROperandType::REGISTER, right_reg)));
    
    last_register = result_reg;
}

void IRGenerator::visit(UnaryExpression& node) {
    // Evaluate operand
    node.operand->accept(*this);
    std::string operand_reg = last_register;
    
    // Perform operation
    std::string result_reg = newRegister();
    IROpcode opcode;
    
    switch (node.operator_type) {
        case TokenType::MINUS: opcode = IROpcode::NEG; break;
        case TokenType::NOT: opcode = IROpcode::NOT; break;
        default:
            throw IRException("Unsupported unary operator in IR generation");
    }
    
    emit(IRInstruction(opcode,
                      IROperand(IROperandType::REGISTER, result_reg),
                      IROperand(IROperandType::REGISTER, operand_reg)));
    
    last_register = result_reg;
}

void IRGenerator::visit(CallExpression& node) {
    // Evaluate arguments
    for (auto& arg : node.arguments) {
        arg->accept(*this);
        emit(IRInstruction(IROpcode::PARAM, IROperand(IROperandType::REGISTER, last_register)));
    }
    
    // Get function name
    std::string func_name;
    if (auto ident = dynamic_cast<IdentifierExpression*>(node.function.get())) {
        func_name = ident->name;
    } else {
        throw IRException("Only simple function calls supported in IR");
    }
    
    // Call function
    std::string result_reg = newRegister();
    emit(IRInstruction(IROpcode::CALL,
                      IROperand(IROperandType::REGISTER, result_reg),
                      IROperand::Var(func_name)));
    
    last_register = result_reg;
}

void IRGenerator::visit(MemberExpression& node) {
    // Not fully implemented yet
    std::string reg = newRegister();
    emit(IRInstruction(IROpcode::NOP));
    last_register = reg;
}

void IRGenerator::visit(AssignmentExpression& node) {
    // Evaluate value
    node.value->accept(*this);
    std::string value_reg = last_register;
    
    // Get target variable name
    if (auto ident = dynamic_cast<IdentifierExpression*>(node.target.get())) {
        emit(IRInstruction(IROpcode::SET_VAR,
                          IROperand::Var(ident->name),
                          IROperand(IROperandType::REGISTER, value_reg)));
    } else {
        throw IRException("Only simple variable assignments supported in IR");
    }
    
    last_register = value_reg;
}

void IRGenerator::visit(ListExpression& node) {
    // Allocate list
    std::string list_reg = newRegister();
    emit(IRInstruction(IROpcode::ALLOC, IROperand(IROperandType::REGISTER, list_reg)));
    
    // Evaluate and store elements
    for (auto& element : node.elements) {
        element->accept(*this);
        // Store element (simplified)
        emit(IRInstruction(IROpcode::STORE,
                          IROperand(IROperandType::REGISTER, list_reg),
                          IROperand(IROperandType::REGISTER, last_register)));
    }
    
    last_register = list_reg;
}

void IRGenerator::visit(DictExpression& node) {
    // Allocate dictionary
    std::string dict_reg = newRegister();
    emit(IRInstruction(IROpcode::ALLOC, IROperand(IROperandType::REGISTER, dict_reg)));
    
    // Evaluate and store key-value pairs
    for (const auto& pair : node.pairs) {
        pair.first->accept(*this);
        std::string key_reg = last_register;
        
        pair.second->accept(*this);
        std::string val_reg = last_register;
        
        // Store key-value pair (simplified)
        emit(IRInstruction(IROpcode::STORE,
                          IROperand(IROperandType::REGISTER, dict_reg),
                          IROperand(IROperandType::REGISTER, key_reg)));
    }
    
    last_register = dict_reg;
}

// Statement visitors

void IRGenerator::visit(ExpressionStatement& node) {
    node.expression->accept(*this);
}

void IRGenerator::visit(BlockStatement& node) {
    for (auto& stmt : node.statements) {
        stmt->accept(*this);
    }
}

void IRGenerator::visit(IfStatement& node) {
    std::string else_label = newLabel("else");
    std::string end_label = newLabel("endif");
    
    // Evaluate condition
    node.condition->accept(*this);
    std::string cond_reg = last_register;
    
    // Jump to else if condition is false
    emit(IRInstruction(IROpcode::JUMP_IF_FALSE,
                      IROperand::Lab(else_label),
                      IROperand(IROperandType::REGISTER, cond_reg)));
    
    // Then block
    node.then_block->accept(*this);
    emit(IRInstruction(IROpcode::JUMP, IROperand::Lab(end_label)));
    
    // Else block
    newBlock(else_label);
    if (node.else_block) {
        node.else_block->accept(*this);
    }
    
    // End label
    newBlock(end_label);
}

void IRGenerator::visit(WhileStatement& node) {
    std::string loop_label = newLabel("loop");
    std::string end_label = newLabel("endloop");
    
    // Push loop context for break/continue handling
    pushLoopContext(loop_label, end_label);
    
    // Loop start
    newBlock(loop_label);
    
    // Evaluate condition
    node.condition->accept(*this);
    std::string cond_reg = last_register;
    
    // Jump to end if condition is false
    emit(IRInstruction(IROpcode::JUMP_IF_FALSE,
                      IROperand::Lab(end_label),
                      IROperand(IROperandType::REGISTER, cond_reg)));
    
    // Loop body
    node.body->accept(*this);
    
    // Jump back to loop start
    emit(IRInstruction(IROpcode::JUMP, IROperand::Lab(loop_label)));
    
    // Pop loop context
    popLoopContext();
    
    // End label
    newBlock(end_label);
}

void IRGenerator::visit(ForStatement& node) {
    // Generate proper iterator-based for-loop
    std::string loop_label = newLabel("forloop");
    std::string end_label = newLabel("endfor");
    std::string check_label = newLabel("forcheck");
    
    // Push loop context for break/continue handling
    // Continue should jump to check (advance iterator), not loop body
    pushLoopContext(check_label, end_label);
    
    // Evaluate iterable expression (e.g., range(10))
    node.iterable->accept(*this);
    int iterable_reg = next_register - 1;  // Last allocated register
    
    // Initialize iterator: iter = ITER_INIT(iterable)
    int iter_reg = next_register++;
    emit(IRInstruction(IROpcode::ITER_INIT, IROperand::Reg(iter_reg), IROperand::Reg(iterable_reg)));
    
    // Jump to check before first iteration
    emit(IRInstruction(IROpcode::JUMP, IROperand::Lab(check_label)));
    
    // Loop body label
    newBlock(loop_label);
    
    // Get next value: %value = ITER_NEXT %iter
    int value_reg = next_register++;
    emit(IRInstruction(IROpcode::ITER_NEXT, IROperand::Reg(value_reg), IROperand::Reg(iter_reg)));
    
    // Assign to loop variable: loop_var = %value
    emit(IRInstruction(IROpcode::SET_VAR, IROperand::Var(node.variable), IROperand::Reg(value_reg)));
    
    // Execute loop body
    node.body->accept(*this);
    
    // Check condition label
    newBlock(check_label);
    
    // Check if iterator is done: done = ITER_DONE(iter)
    int done_reg = next_register++;
    emit(IRInstruction(IROpcode::ITER_DONE, IROperand::Reg(done_reg), IROperand::Reg(iter_reg)));
    
    // If not done, jump back to loop body
    emit(IRInstruction(IROpcode::JUMP_IF_FALSE, IROperand::Lab(loop_label), IROperand::Reg(done_reg)));
    
    // Pop loop context
    popLoopContext();
    
    // End label
    newBlock(end_label);
}

void IRGenerator::visit(FunctionDefinition& node) {
    // Create function block
    std::string func_label = "func_" + node.name;
    newBlock(func_label);
    
    // Declare parameters
    for (const auto& param : node.parameters) {
        emit(IRInstruction(IROpcode::DECLARE, IROperand::Var(param.name)));
    }
    
    // Function body
    node.body->accept(*this);
    
    // Return (if no explicit return)
    emit(IRInstruction(IROpcode::RETURN));
}

void IRGenerator::visit(ClassDefinition& node) {
    // Class definitions not fully implemented yet
    emit(IRInstruction(IROpcode::NOP));
}

void IRGenerator::visit(ReturnStatement& node) {
    if (node.value) {
        node.value->accept(*this);
        emit(IRInstruction(IROpcode::RETURN, IROperand(IROperandType::REGISTER, last_register)));
    } else {
        emit(IRInstruction(IROpcode::RETURN));
    }
}

void IRGenerator::visit(BreakStatement& node) {
    (void)node;
    std::string break_label = getBreakLabel();
    if (!break_label.empty()) {
        emit(IRInstruction(IROpcode::JUMP, IROperand::Lab(break_label)));
    } else {
        throw IRException("'break' statement outside of loop");
    }
}

void IRGenerator::visit(ContinueStatement& node) {
    (void)node;
    std::string continue_label = getContinueLabel();
    if (!continue_label.empty()) {
        emit(IRInstruction(IROpcode::JUMP, IROperand::Lab(continue_label)));
    } else {
        throw IRException("'continue' statement outside of loop");
    }
}

void IRGenerator::visit(PassStatement& node) {
    (void)node;
    emit(IRInstruction(IROpcode::NOP));
}

void IRGenerator::visit(Program& node) {
    for (auto& stmt : node.statements) {
        stmt->accept(*this);
    }
}

} // namespace caesar
