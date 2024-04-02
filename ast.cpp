#include "ast.hpp"

NullObj* const_null = new NullObj();
BooleanObj* const_true = new BooleanObj(true);
BooleanObj* const_false = new BooleanObj(false);

string Program::tokenLiteral(){
    if(!statements.empty()){
        return statements[0]->tokenLiteral();
    }
    return "";
}

string Program::toString(){
    string ret = "";
    for(auto stmt : statements){
        ret += stmt->toString();
    }
    return ret;
}

Obj* Program::eval(){
    Obj* o = nullptr;
    for(auto i : statements){
        o = i->eval();
    }

    return o;
}

//

Identifier::Identifier(Token tok, string value){
    this->tok = tok;
    this->value = value;
}

string Identifier::tokenLiteral(){
    return tok.val;
}

string Identifier::toString(){
    return value;
}

Obj* Identifier::eval(){
    return nullptr;
} 

//

string LetStatement::tokenLiteral(){
    return tok.val;
}

string LetStatement::toString(){
    string ret = "";
    ret += tok.val + " ";
    ret += name->toString() + " = ";
    if(value != nullptr){
        ret += value->toString();
    }
    ret += ";";
    return ret;
}

Obj* LetStatement::eval(){
    return nullptr;

}

//

string ReturnStatement::tokenLiteral(){
    return tok.val;
}

string ReturnStatement::toString(){
    string ret = "";
    ret += tok.val + " ";
    if(value != nullptr){
        ret += value->toString();
    }
    ret += ";";
    return ret;
}

Obj* ReturnStatement::eval(){
    return nullptr;

}

//

string ExpressionStatement::tokenLiteral(){
    return tok.val;
}

string ExpressionStatement::toString(){
    if(expression != nullptr){
        return expression->toString();
    }
    return "";
}

Obj* ExpressionStatement::eval(){
    return expression->eval();
}

//

string IntegerLiteral::tokenLiteral(){
    return tok.val;
}

string IntegerLiteral::toString(){
    return to_string(value);
}

Obj* IntegerLiteral::eval(){
    IntegerObj* o = new IntegerObj(value);
    return o;
}

//

string FunctionLiteral::tokenLiteral(){
    return tok.val;
}

string FunctionLiteral::toString(){
    string ret = tok.val;
    ret += "(";
    for(auto i : parameters){
        ret += i->toString() + ",";
    }
    ret += ")";
    ret += body->toString();
    return ret;
}

Obj* FunctionLiteral::eval(){
    return nullptr;

}

//

string CallExpression::tokenLiteral(){
    return tok.val;
}

string CallExpression::toString(){
    string ret = function->toString();
    ret += "(";
    for(auto i : args){
        ret += i->toString() + ",";
    }
    ret += ")";

    return ret;
}

Obj* CallExpression::eval(){
    return nullptr;
}

//

string PrefixExpression::tokenLiteral(){
    return tok.val;
}

string PrefixExpression::toString(){
    string ret = "(";
    ret += op;
    ret += right->toString();
    ret += ")";
    return ret;
}

// -true should output null, but outputs nothing
Obj* PrefixExpression::eval(){
    Obj* o = right->eval();
    o = evalOperator(op, o);
    return o;
}

Obj* PrefixExpression::evalOperator(string op, Obj* right){
    switch (op[0]) {
        case '!':
            return evalBang(right);
        case '-':
            return evalMinus(right);
        default:
            return const_null;
    }
}

Obj* PrefixExpression::evalBang(Obj* right){
    if(right->Type() == INTEGER_OBJ){
        IntegerObj* r = dynamic_cast<IntegerObj*> (right);
        if(r->val != 0){
            return const_false;
        }
        else{
            return const_true;
        }
    }

    BooleanObj* r = dynamic_cast<BooleanObj*> (right);
    if(r == nullptr){
        return const_null;
    }
    if(r->val){
        return const_false;
    }    
    return const_true;
}

Obj* PrefixExpression::evalMinus(Obj* right){
    IntegerObj* r = dynamic_cast<IntegerObj*> (right);
    //should i free this?
    //maybe learn how to check memory leaks with valgrind too
    if(r == nullptr){
        return const_null;
    }

    IntegerObj* newr = new IntegerObj(-(r->val));
    return newr;
}
//

string InfixExpression::tokenLiteral(){
    return tok.val;
}

string InfixExpression::toString(){
    string ret = "(";
    ret += left->toString();
    ret += op;
    ret += right->toString();
    ret += ")";
    return ret;
}

Obj* InfixExpression::eval(){
    Obj* r = right->eval();
    Obj* l = left->eval();
    return evalInfixExpression(l, r);
}

Obj* InfixExpression::evalInfixExpression(Obj* l, Obj* r){
    IntegerObj* il = dynamic_cast<IntegerObj*> (l);
    IntegerObj* ir = dynamic_cast<IntegerObj*> (r);
    if(il != nullptr && ir != nullptr){
        return evalIntegerInfix(il, ir);
    }
    // //their memroy addresses are the same if they are bool
    if(op == "=="){
        return nativeBoolToBooleanObj(r == l);
    }
    if(op == "!="){
        return nativeBoolToBooleanObj(r != l);
    }

    return const_null;
}

Obj* InfixExpression::evalIntegerInfix(IntegerObj* il, IntegerObj* ir){
    IntegerObj* o = nullptr;
    switch (op[0]){
        case '+' : 
            o = new IntegerObj(il->val + ir->val);
            break;
        case '-':
            o = new IntegerObj(il->val - ir->val);
            break;
        case '*':
            o = new IntegerObj(il->val * ir->val);
            break;
        case '/':
            o = new IntegerObj(il->val / ir->val);
            break;
        case '<':
            return nativeBoolToBooleanObj(il->val < ir->val);
            break;
        case '>':
            return nativeBoolToBooleanObj(il->val > ir->val);
            break;
        case '=':{
            if(op[1] == '='){
                return nativeBoolToBooleanObj(il->val == ir->val);
            }
            else{
                return const_null;
            }
        }
        case '!':{
            if(op[1] == '='){
                return nativeBoolToBooleanObj(il->val != ir->val);
            }
            else{
                return const_null;
            }
        }

        default:
            return const_null;        
    }
    return o;
}

//

string Boolean::tokenLiteral(){
    return tok.val;
}

string Boolean::toString(){
    return tok.val;
}

Obj* Boolean::eval(){
    return nativeBoolToBooleanObj(value);
}

Obj* nativeBoolToBooleanObj(bool value){
    if(value){
        return const_true;
    }
    return const_false;
}
//

string IfExpression::tokenLiteral(){
    return tok.val;
}

string IfExpression::toString(){
    string ret = tokenLiteral();
    ret += "(";
    ret += condition->toString();
    ret += ")";
    ret += conseq->toString();

    if(alt != nullptr){
        ret += "else";
        ret += alt->toString();
    }

    return ret;
}

Obj* IfExpression::eval(){
    return nullptr;

}

//

string BlockStatement::tokenLiteral(){
    return tok.val;
}

string BlockStatement::toString(){
    string ret = "{";
    for(auto i : stmts){
        ret += i->toString();
    }
    ret += "}";
    return ret;
}

Obj* BlockStatement::eval(){
    return nullptr;
}

//