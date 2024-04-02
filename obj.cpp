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