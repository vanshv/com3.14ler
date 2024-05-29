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
    if(so != nullptr){
        return new IntegerObj(so->val.length());
    }

    //need to implement a type specifier    
    ArrayObj* ao = dynamic_cast<ArrayObj*> (o);
    if(ao != nullptr){
        return new IntegerObj(ao->arr.size());
    }

    return new ErrorObj("wrong arguement type\n");
}

Obj* HeadFunction::call(vector<Obj*>& args) {
    if(args.size() != 1){
        return new ErrorObj("wrong number of arguements given to builtin function 'head' :" + args.size());
    }

    Obj* o = args[0];
    ArrayObj* ao = dynamic_cast<ArrayObj*> (o);
    if(ao != nullptr){
        if(!(ao->arr).empty()){
            return *((ao->arr).begin());
        }
        else{
            return new NullObj();
        }
    }

    return new ErrorObj("wrong arguement type4\n");
}

Obj* TailFunction::call(vector<Obj*>& args) {
    if(args.size() != 1){
        return new ErrorObj("wrong number of arguements given to builtin function 'head' :" + args.size());
    }

    Obj* o = args[0];
    ArrayObj* ao = dynamic_cast<ArrayObj*> (o);
    if(ao != nullptr){
        if(!(ao->arr).empty()){
            int n = ao->arr.size();
            vector<Obj*> newarr(n - 1);
            for(int i = 0; i < n - 1; i++){
                newarr[i] = ao->arr[i + 1];
            }

            ArrayObj* newao = new ArrayObj();
            newao->arr = newarr;
            return newao;
        }
        else{
            return new NullObj();
        }
    }

    return new ErrorObj("wrong arguement type3\n");
}

Obj* PushFunction::call(vector<Obj*>& args) {
    if(args.size() != 2){
        return new ErrorObj("wrong number of arguements given to builtin function 'head' :" + args.size());
    }

    Obj* o = args[0];
    ArrayObj* ao = dynamic_cast<ArrayObj*> (o);
    Obj* o2 = args[1];
    if(ao != nullptr){
        int n = ao->arr.size();
        vector<Obj*> newarr(n + 1);
        for(int i = 0; i < n; i++){
            newarr[i] = ao->arr[i];
        }
        newarr[n] = o2;

        ArrayObj* newao = new ArrayObj();
        newao->arr = newarr;
       return newao;
    }

    return new ErrorObj("wrong arguement type2\n");
}

Obj* LastFunction::call(vector<Obj*>& args) {
    if(args.size() != 1){
        return new ErrorObj("wrong number of arguements given to builtin function 'last' :" + args.size());
    }

    Obj* o = args[0];
    ArrayObj* ao = dynamic_cast<ArrayObj*> (o);
    if(ao != nullptr){
        if(!(ao->arr).empty()){
            return *prev(ao->arr.end());
        }
        else{
            return new NullObj();
        }
    }

    return new ErrorObj("wrong arguement type1\n");
}

void set_builtins(){
    BuiltinFunc* len = new LenFunction();
    builtins_map["len"] = new BuiltinObj(len);

    BuiltinFunc* head = new HeadFunction();
    builtins_map["head"] = new BuiltinObj(head);

    BuiltinFunc* tail = new TailFunction();
    builtins_map["tail"] = new BuiltinObj(tail);

    BuiltinFunc* last = new LastFunction();
    builtins_map["last"] = new BuiltinObj(last);

    BuiltinFunc* push = new PushFunction();
    builtins_map["push"] = new BuiltinObj(push);
}

