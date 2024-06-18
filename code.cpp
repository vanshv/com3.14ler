#include "code.hpp"

OpDefinitions* lookup(Opcode op){
    if(opdefinitionsmap.find(op) == opdefinitionsmap.end()){
        cout << "Undefined opcode " << op << "\n";
        return nullptr;
    }

    return opdefinitionsmap[op];
}

// we are given an opcode - like add
// and arbitrary number of arguements - operands
// we need to create an instruction - opcode appended with the operands
Instruction* makeInstruction(Opcode op, vector<int>& operands){
    OpDefinitions* def = opdefinitionsmap[op];
    if(def == nullptr){
        return nullptr;
    }

    int instructionLen = 1;
    for(auto e : def->opwidths){
        instructionLen += e;
    }

    Instruction* ins = new Instruction(instructionLen);
    ins->op = op;

    int offset = 1;
    // get the width of the operand
    // put the operand into the instruction
    for(int i = 0; i < operands.size(); i++){
        putOpInInstruction(offset, operands[i], ins);
        offset += opdefinitionsmap[op]->opwidths[i];
    }

    return ins;
}

// offset - where we are going to write in the Instruction
// op - the operand we write
// wtf little endian is faster AND simpler to program 
// ok, i am making a design choice to code in little endian lol
void putOpInInstruction(int offset, int op, Instruction* ins){
    if(op == 0){
        return;
    }

    // write the last bit of op in Instruction
    ins->operands[offset] = op & (0xFF);
    putOpInInstruction(offset + 1, op << 8, ins); 
}

Instruction::Instruction(int len){
    vector<uint8_t> c(len);
    this->operands = c;
}