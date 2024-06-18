#include "ast.hpp"
    
extern map<string, BuiltinObj *> builtins_map;

/*
    i should free things
    learn how to check memory leaks with valgrind 

*/

// multiple definitions
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

Obj* Program::accept(NodeVisitor& vis, Environment* env){
    return vis.visit(*this, env);
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

Obj* Identifier::accept(NodeVisitor& vis, Environment* env){
    return vis.visit(*this, env);
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

Obj* LetStatement::accept(NodeVisitor& vis, Environment* env){
    return vis.visit(*this, env);
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

Obj* ReturnStatement::accept(NodeVisitor& vis, Environment* env){
    return vis.visit(*this, env);
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

Obj* ExpressionStatement::accept(NodeVisitor& vis, Environment* env){
    return vis.visit(*this, env);
}

//

string IntegerLiteral::tokenLiteral(){
    return tok.val;
}

string IntegerLiteral::toString(){
    return to_string(value);
}

Obj* IntegerLiteral::accept(NodeVisitor& vis, Environment* env){
    return vis.visit(*this, env);
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

Obj* FunctionLiteral::accept(NodeVisitor& vis, Environment* env){
    return vis.visit(*this, env);
}

//

string ArrayLiteral::tokenLiteral(){
    return tok.val;
}

string ArrayLiteral::toString(){
    string res = "[";
    for(auto el : elements){
        res += el->toString();
        res += ", ";
    }
    res += "]";
    return res;
}

Obj* ArrayLiteral::accept(NodeVisitor& vis, Environment* env){
    return vis.visit(*this, env);
}

//

string IndexExpression::tokenLiteral(){
    return tok.val;
}

string IndexExpression::toString(){
    string res = "";
    res += left->toString();
    res += "[";
    res += index->toString();
    res += "]";

    return res;
}

Obj* IndexExpression::accept(NodeVisitor& vis, Environment* env){
    return vis.visit(*this, env);
}

//

string HashLiteral::tokenLiteral(){
    return tok.val;
}

string HashLiteral::toString(){
    string ret = "{";
    for(auto [k, v] : kvmap){
        ret += k->toString();
        ret += " : ";
        ret += v->toString();
        ret += ", ";
    }

    ret += "}";
    return ret;
}

Obj* HashLiteral::accept(NodeVisitor& vis, Environment* env){
    return vis.visit(*this, env);
}

//

string StringLiteral::tokenLiteral(){
    return tok.val;
}

string StringLiteral::toString(){
    return tok.val;
}

Obj* StringLiteral::accept(NodeVisitor& vis, Environment* env){
    return vis.visit(*this, env);
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

Obj* CallExpression::accept(NodeVisitor& vis, Environment* env){
    return vis.visit(*this, env);
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

Obj* PrefixExpression::accept(NodeVisitor& vis, Environment* env){
    return vis.visit(*this, env);
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

Obj* InfixExpression::accept(NodeVisitor& vis, Environment* env){
    return vis.visit(*this, env);
}

//

string Boolean::tokenLiteral(){
    return tok.val;
}

string Boolean::toString(){
    return tok.val;
}

Obj* Boolean::accept(NodeVisitor& vis, Environment* env){
    return vis.visit(*this, env);
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

Obj* IfExpression::accept(NodeVisitor& vis, Environment* env){
    return vis.visit(*this, env);
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

Obj* BlockStatement::accept(NodeVisitor& vis, Environment* env){
    return vis.visit(*this, env);
}

//
bool isTruthy(Obj* o){
    if(o->Type() == INTEGER_OBJ){
        IntegerObj* io = dynamic_cast<IntegerObj*> (o);
        if(io != nullptr && io->val != 0){
            return true;
        }
    }
    else if(o->Type() == BOOLEAN_OBJ){
        BooleanObj* bo = dynamic_cast<BooleanObj*>(o);
        if(bo != nullptr && bo->val == true){
            return true;
        }
    }
    return false;
}

bool isError(Obj* o){
    if(o != nullptr){
        return (o->Type() == ERROR_OBJ);
    }
    return false;
}