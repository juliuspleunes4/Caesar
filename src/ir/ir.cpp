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
    
    // End label
    newBlock(end_label);
}

void IRGenerator::visit(ForStatement& node) {
    // For loops in Caesar are typically: for i in range(start, stop, step)
    // We need to detect range() calls and convert them to while loop equivalents
    
    // Check if iterable is a range() call BEFORE evaluating it
    CallExpression* range_call = dynamic_cast<CallExpression*>(node.iterable.get());
    bool is_range = false;
    int start_reg = -1;
    int stop_reg = -1;
    int step_reg = -1;
    
    // Check if the function being called is "range"
    if (range_call) {
        IdentifierExpression* func_id = dynamic_cast<IdentifierExpression*>(range_call->function.get());
        if (func_id && func_id->name == "range") {
            is_range = true;
            
            // Parse range arguments: range(stop) or range(start, stop) or range(start, stop, step)
            if (range_call->arguments.size() == 1) {
                // range(stop) - start defaults to 0, step defaults to 1
                // Load start = 0
                start_reg = next_register++;
                emit(IRInstruction(IROpcode::LOAD_CONST, IROperand::Reg(start_reg), IROperand::Const("0")));
                
                // Evaluate stop
                range_call->arguments[0]->accept(*this);
                stop_reg = next_register - 1;
                
                // Load step = 1
                step_reg = next_register++;
                emit(IRInstruction(IROpcode::LOAD_CONST, IROperand::Reg(step_reg), IROperand::Const("1")));
            } else if (range_call->arguments.size() == 2) {
                // range(start, stop) - step defaults to 1
                range_call->arguments[0]->accept(*this);
                start_reg = next_register - 1;
                
                range_call->arguments[1]->accept(*this);
                stop_reg = next_register - 1;
                
                // Load step = 1
                step_reg = next_register++;
                emit(IRInstruction(IROpcode::LOAD_CONST, IROperand::Reg(step_reg), IROperand::Const("1")));
            } else if (range_call->arguments.size() >= 3) {
                // range(start, stop, step)
                range_call->arguments[0]->accept(*this);
                start_reg = next_register - 1;
                
                range_call->arguments[1]->accept(*this);
                stop_reg = next_register - 1;
                
                range_call->arguments[2]->accept(*this);
                step_reg = next_register - 1;
            }
        }
    }
    
    if (is_range && start_reg >= 0 && stop_reg >= 0 && step_reg >= 0) {
        // Generate range-based for loop as while loop
        // 1. Declare and initialize loop variable
        emit(IRInstruction(IROpcode::DECLARE, IROperand::Var(node.variable)));
        emit(IRInstruction(IROpcode::SET_VAR, IROperand::Var(node.variable), IROperand::Reg(start_reg)));
        
        // 2. Loop start label
        std::string loop_label = newLabel("forloop");
        newBlock(loop_label);
        
        // 3. Check loop condition: i < stop
        int loop_var_reg = next_register++;
        emit(IRInstruction(IROpcode::GET_VAR, IROperand::Reg(loop_var_reg), IROperand::Var(node.variable)));
        
        int cond_reg = next_register++;
        emit(IRInstruction(IROpcode::LT, IROperand::Reg(cond_reg), 
                          IROperand::Reg(loop_var_reg), IROperand::Reg(stop_reg)));
        
        std::string end_label = newLabel("endfor");
        emit(IRInstruction(IROpcode::JUMP_IF_FALSE, IROperand::Lab(end_label), IROperand::Reg(cond_reg)));
        
        // 4. Loop body
        node.body->accept(*this);
        
        // 5. Increment loop variable: i = i + step
        int current_val_reg = next_register++;
        emit(IRInstruction(IROpcode::GET_VAR, IROperand::Reg(current_val_reg), IROperand::Var(node.variable)));
        
        int new_val_reg = next_register++;
        emit(IRInstruction(IROpcode::ADD, IROperand::Reg(new_val_reg), 
                          IROperand::Reg(current_val_reg), IROperand::Reg(step_reg)));
        
        emit(IRInstruction(IROpcode::SET_VAR, IROperand::Var(node.variable), IROperand::Reg(new_val_reg)));
        
        // 6. Jump back to loop start
        emit(IRInstruction(IROpcode::JUMP, IROperand::Lab(loop_label)));
        
        // 7. End label
        newBlock(end_label);
    } else {
        // Generic iterator (not range) - simplified implementation
        // TODO: Implement proper iterator protocol for lists, dicts, etc.
        std::string loop_label = newLabel("forloop");
        std::string end_label = newLabel("endfor");
        
        // Declare loop variable
        emit(IRInstruction(IROpcode::DECLARE, IROperand::Var(node.variable)));
        
        // Initialize iterator (simplified)
        node.iterable->accept(*this);
        
        // Loop start
        newBlock(loop_label);
        
        // Loop body
        node.body->accept(*this);
        
        // Jump back to loop start (simplified - no proper iterator check)
        emit(IRInstruction(IROpcode::JUMP, IROperand::Lab(loop_label)));
        
        // End label
        newBlock(end_label);
    }
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
    // Break statements need loop context tracking (TODO)
    emit(IRInstruction(IROpcode::NOP));
}

void IRGenerator::visit(ContinueStatement& node) {
    (void)node;
    // Continue statements need loop context tracking (TODO)
    emit(IRInstruction(IROpcode::NOP));
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
