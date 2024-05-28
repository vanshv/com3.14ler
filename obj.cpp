#include "obj.hpp"

IntegerObj::IntegerObj(int val){
    this->val = val;
}

ObjType IntegerObj::Type(){
    return INTEGER_OBJ;
}

string IntegerObj::Inspect(){
    return to_string(val);
}
//

BooleanObj::BooleanObj(bool val){
    this->val = val;
}

ObjType BooleanObj::Type(){
    return BOOLEAN_OBJ;
}

string BooleanObj::Inspect(){
    if(val){
        return "true";
    }
    else{
        return "false";
    }
    //to_string returns 0 and 1, this is less ambiguous
}

//

NullObj::NullObj(){
    
}

ObjType NullObj::Type(){
    return NULL_OBJ;
}

string NullObj::Inspect(){
    return "null";
}

//

ReturnObj::ReturnObj(Obj* val){
    this->val = val;
}

ObjType ReturnObj::Type(){
    return RETURN_OBJ;
}

string ReturnObj::Inspect(){
    string str = val->Inspect();
    return str;
}

//

ErrorObj::ErrorObj(string msg){
    this->message = msg;
}

ObjType ErrorObj::Type(){
    return ERROR_OBJ;
}

string ErrorObj::Inspect(){
    return "Error: " + message;
}

//

StringObj::StringObj(string str){
    this->val = str;
}

ObjType StringObj::Type(){
    return STRING_OBJ;
}

string StringObj::Inspect(){
    return val;
}

//

BuiltinObj::BuiltinObj(BuiltinFunc* f){
    this->functor = f;
}

ObjType BuiltinObj::Type(){
    return BUILTIN_OBJ;
}

string BuiltinObj::Inspect(){
    return "Builtin_Function";
}

//

Environment::Environment(){
    this->outer = nullptr;
}

/////////////////
//    //////// //
// e1 // e2 // //
//    //////// //
/////////////////
// creates a new env inside that env and returns that
Environment* enclose(Environment* e1){
    Environment* e2 = new Environment();
    e2->outer = e1;
    return e2;
}

Obj* Environment::get(string ident){
    if(envmap.find(ident) == envmap.end()){
        if(outer != nullptr){
            return outer->get(ident);
        }
        return nullptr;
    }
    return envmap[ident];
}

void Environment::set(string ident, Obj* o){
    envmap[ident] = o;
}

//