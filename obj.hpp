#ifndef OBJ_H
#define OBJ_H

#include <bits/stdc++.h>
using namespace std;

enum ObjType {
    INTEGER_OBJ,
    BOOLEAN_OBJ,
    NULL_OBJ,
    RETURN_OBJ,
    ERROR_OBJ,
    FUNTION_OBJ,
    STRING_OBJ,
    BUILTIN_OBJ,
};

//if we define at least one virtual function in our base classes, it
//lets us use dynamic_cast<>

// what if we dynamic_cast from derived1 to base.
// is it possible to cast to derived2? i think not, because of virtual tables 

// implement a simple type specifier instead of using dynamic_case everywhere lol
class Obj {
    public:
        virtual ObjType Type() = 0;
        virtual string Inspect() = 0;
};

class IntegerObj : public Obj{
    public:
        int val;

        IntegerObj(int);
        ObjType Type() override;
        string Inspect() override;
};

class BooleanObj : public Obj{
    public:
        bool val;
        
        BooleanObj(bool);
        ObjType Type() override;
        string Inspect() override;
};

class NullObj : public Obj{
    public:
        NullObj();
        ObjType Type() override;
        string Inspect() override;
};

class ReturnObj : public Obj{
    public:
        Obj* val;

        ReturnObj(Obj*);
        ObjType Type() override;
        string Inspect() override;
};

class ErrorObj : public Obj{
    public:
        string message;

        ErrorObj(string msg);
        ObjType Type() override;
        string Inspect() override;
};

class StringObj : public Obj{
    public:
        string val;

        StringObj(string str);
        ObjType Type() override;
        string Inspect() override;
};

class BuiltinFunc {
    public:
    virtual Obj* call(vector<Obj*>& args) = 0;
};

class BuiltinObj : public Obj{
    public:
        BuiltinFunc* functor;
        
        BuiltinObj(BuiltinFunc* f);
        ObjType Type() override;
        string Inspect() override;
};

// the biggest lie ever told?
class ArrayObj : public Obj{
    public:
        vector<Obj*> arr;

        ArrayObj();
        ObjType Type() override;
        string Inspect() override;
};

class Environment{
    public:
        unordered_map<string, Obj*> envmap;
        Environment* outer;

        Environment();
        Obj* get(string);
        void set(string, Obj*);
};

Environment* enclose(Environment*);

#endif // OBJ_H