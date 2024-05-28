#ifndef BUILTINS_H
#define BUILTINS_H

#include "obj.hpp"

class LenFunction : public BuiltinFunc
{
public:
    Obj* call(vector<Obj*> &args);
};

extern map<string, BuiltinObj*> builtins_map;
void set_builtins();

#endif