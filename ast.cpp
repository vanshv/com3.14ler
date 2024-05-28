#include "ast.hpp"

NullObj* const_null = new NullObj();
BooleanObj* const_true = new BooleanObj(true);
BooleanObj* const_false = new BooleanObj(false);
    
extern map<string, BuiltinObj *> builtins_map;

/*
    i should free things
    learn how to check memory leaks with valgrind 

*/

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

Obj* Program::eval(Environment* env){
    Obj* o = nullptr;
    for(auto i : statements){
        o = i->eval(env);
        if(o->Type() == RETURN_OBJ){
            ReturnObj* ro = dynamic_cast<ReturnObj*>(o);
            return ro->val;
        }
        if(o->Type() == ERROR_OBJ){
            return o;
        }
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

Obj* Identifier::eval(Environment* env){
    Obj* o = env->get(value);
    if(o == nullptr){
        if(builtins_map.find(value) == builtins_map.end()){
            return new ErrorObj("identifier not found " + value);
        }
        BuiltinObj* bo = builtins_map[value];
        return bo;
    }
    return o;
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

Obj* LetStatement::eval(Environment* env){
    Obj* o = value->eval(env);
    if(isError(o)){
        return o;
    }
    env->set(name->value, o);
    return o;
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

Obj* ReturnStatement::eval(Environment* env){
    Obj* o = value->eval(env);
    if (isError(o)) {
        return o;
    }
    ReturnObj* ro = new ReturnObj(o);
    return ro;
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

Obj* ExpressionStatement::eval(Environment* env){
    Obj* o = expression->eval(env);
    if (isError(o)) {
        return o;
    }
    return o;
}

//

string IntegerLiteral::tokenLiteral(){
    return tok.val;
}

string IntegerLiteral::toString(){
    return to_string(value);
}

Obj* IntegerLiteral::eval(Environment* env){
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

Obj* FunctionLiteral::eval(Environment* env){
    FunctionObj* fo = new FunctionObj(parameters, body, env);
    return fo;
}

//

string StringLiteral::tokenLiteral(){
    return tok.val;
}

string StringLiteral::toString(){
    return tok.val;
}

Obj* StringLiteral::eval(Environment* env){
    StringObj* so = new StringObj(value);
    return so;
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

Obj* CallExpression::eval(Environment* env){
    Obj* o = function->eval(env);
    BuiltinObj* bo = dynamic_cast<BuiltinObj*> (o);
    if(isError(o)){
        return o;
    }
    vector<Obj*> veco = evalExpression(env, args);
    if(veco.size() == 1 && isError(veco[0])){
        return veco[0];
    }

    return applyFunction(o, veco);
}

Obj* CallExpression::applyFunction(Obj* o, vector<Obj*> vecos){
    FunctionObj* fo = dynamic_cast<FunctionObj*> (o);
    if(fo == nullptr){
        BuiltinObj* bo = dynamic_cast<BuiltinObj*> (o);
        if(bo == nullptr){
            return new ErrorObj("Expected builtin/function object, received ?");
        }
        else{
            return bo->functor->call(vecos);
        }
    }

    Environment* env = extendFunctionEnv(fo, vecos);
    Obj* evaled = fo->body->eval(env);
    return unwrapReturnValue(evaled);
}

vector<Obj*> CallExpression::evalExpression(Environment* env, vector<Expression*>& args){
    vector<Obj*> res;
    for(auto a : args){
        Obj* o = a->eval(env);
        if(isError(o)){
            return vector<Obj*> {o};
        }
        res.push_back(o);
    }

    return res;
}

Environment* CallExpression::extendFunctionEnv(FunctionObj* fo, vector<Obj*>& args){
    Environment* newenv = enclose(fo->env);

    for(int i = 0; i < args.size(); i++){
        newenv->set(fo->parameters[i]->value, args[i]);
    }

    return newenv;
}

Obj* CallExpression::unwrapReturnValue(Obj* o){
    ReturnObj* ro = dynamic_cast<ReturnObj*> (o);
    if(ro == nullptr){
        return o;
    }

    return ro->val;
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

Obj* PrefixExpression::eval(Environment* env){
    Obj* o = right->eval(env);
    if (isError(o)) {
        return o;
    }
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
            return new ErrorObj("Unknown operator, " + op + right->Inspect());
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
    if(right->Type() == BOOLEAN_OBJ){
        return new ErrorObj("unknown operator: - " + right->Type());
    }

    //maybe just change the value, why create new variable?
    IntegerObj* r = dynamic_cast<IntegerObj*> (right);
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

Obj* InfixExpression::eval(Environment* env){
    Obj* r = right->eval(env);
    if (isError(r)) {
        return r;
    }
    Obj* l = left->eval(env);
    if (isError(l)) {
        return l;
    }
    return evalInfixExpression(l, r);
}

Obj* InfixExpression::evalInfixExpression(Obj* l, Obj* r){
    if(l->Type() == INTEGER_OBJ && r->Type() == INTEGER_OBJ){
        IntegerObj* il = dynamic_cast<IntegerObj*> (l);
        IntegerObj* ir = dynamic_cast<IntegerObj*> (r);
        return evalIntegerInfix(il, ir);
    }
    else if(l->Type() == STRING_OBJ && r->Type() == STRING_OBJ){
        StringObj* sl = dynamic_cast<StringObj*> (l);
        StringObj* sr = dynamic_cast<StringObj*> (r);
        return evalStringInfix(sl, sr);
    }
    // //their memroy addresses are the same if they are bool
    if(op == "=="){
        return nativeBoolToBooleanObj(r == l);
    }
    if(op == "!="){
        return nativeBoolToBooleanObj(r != l);
    }
    if(l->Type() != r->Type()){
        return new ErrorObj("Mismatched Types - " + l->Inspect() + op + r->Inspect());
    }

    return new ErrorObj("Unknown Operator - " + l->Inspect() + op + r->Inspect());
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
                return new ErrorObj("Unknown Opeator - " + il->Inspect() + op + ir->Inspect());        
            }
        }
        case '!':{
            if(op[1] == '='){
                return nativeBoolToBooleanObj(il->val != ir->val);
            }
            else{
                return new ErrorObj("Unknown Opeator - " + il->Inspect() + op + ir->Inspect());        
            }
        }

        default:
            return new ErrorObj("Unknown Opeator - " + il->Inspect() + op + ir->Inspect());        
    }
    return o;
}

Obj* InfixExpression::evalStringInfix(StringObj* sl, StringObj* sr){
    if(op[0] != '+'){
        return new ErrorObj("String operand other than +, used" + op[0]);
    }
    
    string left = sl->val;
    string right = sr->val;
    StringObj* so = new StringObj(left + right);
    return so;
}

//

string Boolean::tokenLiteral(){
    return tok.val;
}

string Boolean::toString(){
    return tok.val;
}

Obj* Boolean::eval(Environment* env){
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

Obj* IfExpression::eval(Environment* env){
    Obj* o = condition->eval(env);
    if (isError(o)) {
        return o;
    }
    if(isTruthy(o)){
        return conseq->eval(env);
    }
    else if(alt != nullptr){
        return alt->eval(env);
    }
    return const_null;
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

Obj* BlockStatement::eval(Environment* env){
    Obj* o = nullptr;
    for(auto i : stmts){
        o = i->eval(env);
        if(o->Type() == RETURN_OBJ || o->Type() == ERROR_OBJ){
            return o;
        }
    }
    return o;
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

//

FunctionObj::FunctionObj(vector<Identifier*> params, BlockStatement* body, Environment* env){
    this->parameters = params;
    this->body = body;
    this->env = env;
}

ObjType FunctionObj::Type(){
    return FUNTION_OBJ;
}

string FunctionObj::Inspect(){
    string res = "fn";
    res += " (";
    for(auto i : parameters){
        res += i->toString() + ", ";
    }
    res += ") ";
    res += body->toString();

    return res;
}

//