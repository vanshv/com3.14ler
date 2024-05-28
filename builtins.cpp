#include "builtins.hpp"

map<string, BuiltinObj*> builtins_map;

//ok, i don't know what I was thinking.
// is there really a need for functors instead of normal functions?
Obj* LenFunction::call(vector<Obj*>& args) {
    if(args.size() != 1){
        return new ErrorObj("wrong number of arguements given to builtin function 'len' :" + args.size());
    }

    Obj* o = args[0];
    StringObj* so = dynamic_cast<StringObj*> (o);
    if(so == nullptr){
        return new ErrorObj("wrong arguement type\n");
    }

    return new IntegerObj(so->val.length());
}

void set_builtins(){
    BuiltinFunc* f = new LenFunction();
    builtins_map["len"] = new BuiltinObj(f);
}

