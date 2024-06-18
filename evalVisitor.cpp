#include "evalVisitor.hpp"


// multiple definitions
// change const_null2 to const_null, doing so gives multiple declarations for now
NullObj* const_null2 = new NullObj();
BooleanObj* const_true2 = new BooleanObj(true);
BooleanObj* const_false2 = new BooleanObj(false);

Obj* EvalVisitor::visit(Program& p, Environment* env){
    Obj* o = nullptr;
    for(auto i : p.statements){
        o = i->accept(*this, env);
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

Obj* EvalVisitor::visit(Identifier& i, Environment* env) {
    string value = i.value;

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

Obj* EvalVisitor::visit(LetStatement& ls, Environment* env) {
    Obj* o = ls.value->accept(*this, env);
    if(isError(o)){
        return o;
    }
    env->set(ls.name->value, o);
    return o;
}

Obj* EvalVisitor::visit(ReturnStatement& rs, Environment* env) {
    Obj* o = rs.value->accept(*this, env);
    if (isError(o)) {
        return o;
    }
    ReturnObj* ro = new ReturnObj(o);
    return ro;
}

Obj* EvalVisitor::visit(IntegerLiteral& il, Environment* env) {
    IntegerObj* o = new IntegerObj(il.value);
    return o;
}

Obj* EvalVisitor::visit(CallExpression& ce, Environment* env) {
    Obj* o = ce.function->accept(*this, env);
    BuiltinObj* bo = dynamic_cast<BuiltinObj*> (o);
    if(isError(o)){
        return o;
    }
    vector<Obj*> veco = evalExpression(env, ce.args);
    if(veco.size() == 1 && isError(veco[0])){
        return veco[0];
    }

    return applyFunction(o, veco);
}

Obj* EvalVisitor::applyFunction(Obj* o, vector<Obj*> vecos){
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
    Obj* evaled = fo->body->accept(*this, env);
    return unwrapReturnValue(evaled);
}

vector<Obj*> EvalVisitor::evalExpression(Environment* env, vector<Expression*>& args){
    vector<Obj*> res;
    for(auto a : args){
        Obj* o = a->accept(*this, env);
        if(isError(o)){
            return vector<Obj*> {o};
        }
        res.push_back(o);
    }

    return res;
}

Environment* EvalVisitor::extendFunctionEnv(FunctionObj* fo, vector<Obj*>& args){
    Environment* newenv = enclose(fo->env);

    for(int i = 0; i < args.size(); i++){
        newenv->set(fo->parameters[i]->value, args[i]);
    }

    return newenv;
}

Obj* EvalVisitor::unwrapReturnValue(Obj* o){
    ReturnObj* ro = dynamic_cast<ReturnObj*> (o);
    if(ro == nullptr){
        return o;
    }

    return ro->val;
}

Obj* EvalVisitor::visit(PrefixExpression& pe, Environment* env) {
    Obj* o = pe.right->accept(*this, env);
    if (isError(o)) {
        return o;
    }
    o = evalOperator(pe.op, o);
    return o;
}

Obj* EvalVisitor::evalOperator(string op, Obj* right){
    switch (op[0]) {
        case '!':
            return evalBang(right);
        case '-':
            return evalMinus(right);
        default:
            return new ErrorObj("Unknown operator, " + op + right->Inspect());
    }
}

Obj* EvalVisitor::evalBang(Obj* right){
    if(right->Type() == INTEGER_OBJ){
        IntegerObj* r = dynamic_cast<IntegerObj*> (right);
        if(r->val != 0){
            return const_false2;
        }
        else{
            return const_true2;
        }
    }

    BooleanObj* r = dynamic_cast<BooleanObj*> (right);
    if(r == nullptr){
        return const_null2;
    }
    if(r->val){
        return const_false2;
    }    
    return const_true2;
}

Obj* EvalVisitor::evalMinus(Obj* right){
    if(right->Type() == BOOLEAN_OBJ){
        return new ErrorObj("unknown operator: - " + right->Type());
    }

    //maybe just change the value, why create new variable?
    IntegerObj* r = dynamic_cast<IntegerObj*> (right);
    IntegerObj* newr = new IntegerObj(-(r->val));
    return newr;
}

Obj* EvalVisitor::visit(InfixExpression& ie, Environment* env) {
    Obj* r = ie.right->accept(*this, env);
    if (isError(r)) {
        return r;
    }
    Obj* l = ie.left->accept(*this, env);
    if (isError(l)) {
        return l;
    }
    return evalInfixExpression(ie, l, r);
}

Obj* EvalVisitor::evalInfixExpression(InfixExpression& ie, Obj* l, Obj* r){
    if(l->Type() == INTEGER_OBJ && r->Type() == INTEGER_OBJ){
        IntegerObj* il = dynamic_cast<IntegerObj*> (l);
        IntegerObj* ir = dynamic_cast<IntegerObj*> (r);
        return evalIntegerInfix(ie, il, ir);
    }
    else if(l->Type() == STRING_OBJ && r->Type() == STRING_OBJ){
        StringObj* sl = dynamic_cast<StringObj*> (l);
        StringObj* sr = dynamic_cast<StringObj*> (r);
        return evalStringInfix(ie, sl, sr);
    }
    // //their memroy addresses are the same if they are bool
    if(ie.op == "=="){
        return nativeBoolToBooleanObj(r == l);
    }
    if(ie.op == "!="){
        return nativeBoolToBooleanObj(r != l);
    }
    if(l->Type() != r->Type()){
        return new ErrorObj("Mismatched Types - " + l->Inspect() + ie.op + r->Inspect());
    }

    return new ErrorObj("Unknown Operator - " + l->Inspect() + ie.op + r->Inspect());
}

Obj* EvalVisitor::evalIntegerInfix(InfixExpression& ie, IntegerObj* il, IntegerObj* ir){
    IntegerObj* o = nullptr;
    switch (ie.op[0]){
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
            if(ie.op[1] == '='){
                return nativeBoolToBooleanObj(il->val == ir->val);
            }
            else{
                return new ErrorObj("Unknown Opeator - " + il->Inspect() + ie.op + ir->Inspect());        
            }
        }
        case '!':{
            if(ie.op[1] == '='){
                return nativeBoolToBooleanObj(il->val != ir->val);
            }
            else{
                return new ErrorObj("Unknown Opeator - " + il->Inspect() + ie.op + ir->Inspect());        
            }
        }

        default:
            return new ErrorObj("Unknown Opeator - " + il->Inspect() + ie.op + ir->Inspect());        
    }
    return o;
}

Obj* EvalVisitor::evalStringInfix(InfixExpression& ie, StringObj* sl, StringObj* sr){
    if(ie.op[0] != '+'){
        return new ErrorObj("String operand other than +, used" + ie.op[0]);
    }
    
    string left = sl->val;
    string right = sr->val;
    StringObj* so = new StringObj(left + right);
    return so;
}

Obj* EvalVisitor::visit(IndexExpression& ie, Environment* env) {
    Obj* l = ie.left->accept(*this, env);
    if(isError(l)){
        return l;
    }

    Obj* i = ie.index->accept(*this, env);
    if(isError(i)){
        return i;
    }
    return evalIndexExpression(l, i);
}

Obj* EvalVisitor::visit(HashLiteral& hl, Environment* env) {
    map<long long, HashPair*> pairsmap;

    for(auto kv : hl.kvmap){
        Obj* key = kv.first->accept(*this, env);
        if(isError(key)){
            return key;
        }

        //key should be integer, string or boolean
        long long h;
        if(IntegerObj* io = dynamic_cast<IntegerObj*>(key)){
            h = io->val;
        }
        else if(StringObj* so = dynamic_cast<StringObj*>(key)){
            h = getHashKey(so->val);
        }
        else if(BooleanObj* bo = dynamic_cast<BooleanObj*>(key)){
            h = bo->val;
        }
        else{
            return new ErrorObj("did not find appropriate type");
        }

        Obj* val = kv.second->accept(*this, env);
        if(isError(val)){
            return val;
        }

        HashPair* hp = new HashPair(key, val);
        pairsmap[h] = hp;
    }

    HashObj* ho = new HashObj();
    ho->pairs = pairsmap;
    return ho;
}

Obj* EvalVisitor::visit(StringLiteral& sl, Environment* env) {
    StringObj* so = new StringObj(sl.value);
    return so;
}

Obj* EvalVisitor::visit(ArrayLiteral& al, Environment* env) {
    vector<Obj*> eles = evalExpression(env, al.elements);

    if(eles.size() == 1 && isError(eles[0])){
        return eles[0];
    }

    ArrayObj* ao = new ArrayObj();
    ao->arr = eles;

    return ao;
}

Obj* EvalVisitor::visit(ExpressionStatement& el, Environment* env) {
    Obj* o = el.expression->accept(*this, env);
    if (isError(o)) {
        return o;
    }
    return o;
}

Obj* EvalVisitor::visit(FunctionLiteral& fl, Environment* env) {
    FunctionObj* fo = new FunctionObj(fl.parameters, fl.body, env);
    return fo;
}

Obj* EvalVisitor::visit(Boolean& b, Environment* env) {
    return nativeBoolToBooleanObj(b.value);
}

Obj* EvalVisitor::nativeBoolToBooleanObj(bool value){
    if(value){
        return const_true2;
    }
    return const_false2;
}
    
Obj* EvalVisitor::visit(IfExpression& ie, Environment* env) {
    Obj* o = ie.condition->accept(*this, env);
    if (isError(o)) {
        return o;
    }
    if(isTruthy(o)){
        return ie.conseq->accept(*this, env);
    }
    else if(ie.alt != nullptr){
        return ie.alt->accept(*this, env);
    }
    return const_null2;
}

Obj* EvalVisitor::visit(BlockStatement& bs, Environment* env) {
    Obj* o = nullptr;
    for(auto i : bs.stmts){
        o = i->accept(*this, env);
        if(o->Type() == RETURN_OBJ || o->Type() == ERROR_OBJ){
            return o;
        }
    }
    return o;
}

Obj* EvalVisitor::evalIndexExpression(Obj* l, Obj* i){
    ArrayObj* ao = dynamic_cast<ArrayObj*> (l);
    IntegerObj* io = dynamic_cast<IntegerObj*> (i);
    if(ao != nullptr && io != nullptr){
        return evalArrayIndexExpression(ao, io);
    }

    HashObj* ho = dynamic_cast<HashObj*> (l);
    if(ho != nullptr){
        return evalHashIndexExpression(ho, i);
    }

    return new ErrorObj("thought it was an index expression, but failed");
}

Obj* EvalVisitor::evalHashIndexExpression(HashObj* ho, Obj* key){
    long long h;
    if(IntegerObj* io = dynamic_cast<IntegerObj*>(key)){
        h = io->val;
    }
    else if(StringObj* so = dynamic_cast<StringObj*>(key)){
        h = getHashKey(so->val);
    }
    else if(BooleanObj* bo = dynamic_cast<BooleanObj*>(key)){
        h = bo->val;
    }
    else{
        return new ErrorObj("did not find appropriate type");
    }

    if(ho->pairs.find(h) == ho->pairs.end()){
        return new NullObj();
    }
    return (ho->pairs[h])->val;
}

// i am casting to object and reverting it back to arrayobj or some other object, is there some point to this?
Obj* EvalVisitor::evalArrayIndexExpression(ArrayObj* left, IntegerObj* index){
    int ind = index->val;
    vector<Obj*> arrr = left->arr;
    int len = arrr.size();

    if(ind < 0 || ind >= len){
        return new NullObj();
    }

    return arrr[ind];
}