#ifndef COMPILER_H
#define COMPILER_H

#include "ast.hpp"
#include "code.hpp"
#include "obj.hpp"

class ByteCode{
    public:
    Instructions instructions;
    vector<Obj*> constants;

    ByteCode();
};

class Compiler{
    public:
    Instructions instructions;
    vector<Obj*> constants;

    Compiler();
};

#endif