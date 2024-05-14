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
};

//if we define at least one virtual function in our base classes, it
//lets us use dynamic_cast<>

// what if we dynamic_cast from derived1 to base.
// is it possible to cast to derived2? i think not, because of virtual tables 
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