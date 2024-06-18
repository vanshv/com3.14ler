#ifndef EVALVISITOR_H
#define EVALVISITOR_H

#include "visitor.hpp"

class EvalVisitor : public NodeVisitor{
    public:
    Obj* visit(Identifier& i, Environment* env) override;
    Obj* visit(LetStatement& ls, Environment* env) override;
    Obj* visit(ReturnStatement& rs, Environment* env) override;
    Obj* visit(IntegerLiteral& il, Environment* env) override;
    Obj* visit(IfExpression& ie, Environment* env) override;
    Obj* visit(BlockStatement& bs, Environment* env) override;
    Obj* visit(HashLiteral& hl, Environment* env) override;
    Obj* visit(StringLiteral& sl, Environment* env) override;
    Obj* visit(ArrayLiteral& al, Environment* env) override;
    Obj* visit(ExpressionStatement& el, Environment* env) override;
    Obj* visit(FunctionLiteral& fl, Environment* env) override;
    Obj* visit(Program& p, Environment* env) override;

    Obj* visit(CallExpression& ce, Environment* env) override;
    Obj* applyFunction(Obj* o, vector<Obj*> vecos);
    vector<Obj*> evalExpression(Environment* env, vector<Expression*>& args);
    Environment* extendFunctionEnv(FunctionObj* fo, vector<Obj*>& args);
    Obj* unwrapReturnValue(Obj* o);

    Obj* visit(PrefixExpression& pe, Environment* env) override;
    Obj* evalOperator(string op, Obj* right);
    Obj* evalBang(Obj* right);
    Obj* evalMinus(Obj* right);

    Obj* visit(InfixExpression& ie, Environment* env) override;
    Obj* evalInfixExpression(InfixExpression& ie, Obj* l, Obj* r);
    Obj* evalIntegerInfix(InfixExpression& ie, IntegerObj* il, IntegerObj* ir);
    Obj* evalStringInfix(InfixExpression& ie, StringObj* sl, StringObj* sr);

    Obj* visit(IndexExpression& ie, Environment* env) override;
    Obj* evalIndexExpression(Obj* l, Obj* i);
    Obj* evalHashIndexExpression(HashObj* ho, Obj* key);
    Obj* evalArrayIndexExpression(ArrayObj* left, IntegerObj* index);

    Obj* visit(Boolean& b, Environment* env) override;
    Obj* nativeBoolToBooleanObj(bool value);
};

#endif