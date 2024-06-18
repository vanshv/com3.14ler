#ifndef CODE_H
#define CODE_H

#include<bits/stdc++.h>

using namespace std;

typedef uint8_t Opcode;
// like push will have opcode 0000
// add will have opcode 0001, and so on

// Instruction is an opcode with trailing operands
// to find the length of instruction, we can check the opdefinitionsmap
// Better to use vector<uint8_t> than malloc in modern cpp

// you will mess this up - i am using little endian, not big endian
class Instruction{
    public:
    Opcode op;
    vector<uint8_t> operands;

    Instruction(int len) : operands(len) {}
};

typedef vector<Instruction> Instructions;

enum{
    Opconstant,
};

class OpDefinitions{
    public:
    string name;
    vector<int> opwidths;
};

unordered_map<Opcode, OpDefinitions*> opdefinitionsmap;

OpDefinitions* lookup(Opcode op);
// should these be ints? 
Instruction* makeInstruction(Opcode op, vector<int>& operands); 
void putOpInInstruction(int offset, int op, Instruction* ins);


// Don't define ByteCode here because of circular dependencies

#endif
