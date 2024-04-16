#ifndef AST_H
#define AST_H

#include <bits/stdc++.h>
#include "lexer.hpp"
#include "obj.hpp"

using namespace std;



class Node{
    public:
        virtual string tokenLiteral() = 0;
        virtual string toString() = 0;
        virtual Obj* eval(Environment* env) = 0;
};

class Statement : public Node{
};

class Expression : public Node{
};

// should Obj* be a class variable?
class Identifier : public Expression {
    public:
        Token tok;
        string value;

        Identifier(Token tok, string value);
        string tokenLiteral() override;
        string toString() override;
        Obj* eval(Environment* env) override;
};

class IntegerLiteral : public Expression{
    public:
        Token tok;
        int value;

        string tokenLiteral() override;
        string toString() override;
        Obj* eval(Environment* env) override;

};

class BlockStatement: public Statement{
    public:
        Token tok; // {
        vector<Statement*> stmts;

        string tokenLiteral() override;
        string toString() override;
        Obj* eval(Environment* env) override;
};

class FunctionObj : public Obj{
    public:
        vector<Identifier*> parameters;
        BlockStatement* body;
        Environment* env;

        FunctionObj (vector<Identifier*>, BlockStatement*, Environment*);
        ObjType Type() override;
        string Inspect() override;
};

class FunctionLiteral : public Expression{
    public:
        Token tok;
        vector<Identifier*> parameters;
        BlockStatement* body;

        string tokenLiteral() override;
        string toString() override;
        Obj* eval(Environment* env) override;

};

class CallExpression: public Expression{
    public:
        Token tok; // (
        Expression* function; // can be Identifier or FunctionLiteral
        vector<Expression*> args;

        string tokenLiteral() override;
        string toString() override;
        Obj* eval(Environment*) override;
        Obj* applyFunction(Obj*, vector<Obj*>);
        vector<Obj*> evalExpression(Environment*, vector<Expression*>&);
        Environment* extendFunctionEnv(FunctionObj*, vector<Obj*>&);
        Obj* unwrapReturnValue(Obj*);
};

class PrefixExpression : public Expression{
    public:
        Token tok;
        string op;
        Expression* right;

        string tokenLiteral() override;
        string toString() override;
        Obj* eval(Environment* env) override;
        Obj* evalOperator(string op, Obj* o);
        Obj* evalBang(Obj* right);
        Obj* evalMinus(Obj* right);
};

class InfixExpression : public Expression{
    public:
        Token tok;
        string op;
        Expression* right;
        Expression* left;

        string tokenLiteral() override;
        string toString() override;
        Obj* eval(Environment* env) override;
        Obj* evalInfixExpression(Obj* l, Obj* r);
        Obj* evalIntegerInfix(IntegerObj* l, IntegerObj* r);
};


// sub-statements do not need constructors
// parsing might fail, so we need to allocate bit-by-bit
class LetStatement : public Statement{
    public:
        Token tok;
        Identifier *name;
        Expression *value;

        string tokenLiteral() override;
        string toString() override;
        Obj* eval(Environment* env) override;

};

class ReturnStatement : public Statement{
    public:
        Token tok;
        Expression *value;

        string tokenLiteral() override;
        string toString() override;
        Obj* eval(Environment* env) override;

};

class ExpressionStatement : public Statement{
    public:
        Token tok;
        Expression *expression;

        string tokenLiteral() override;
        string toString() override;
        Obj* eval(Environment* env) override;

};

class Boolean : public Expression{
    public:
        Token tok;
        bool value;

        string tokenLiteral() override;
        string toString() override;
        Obj* eval(Environment* env) override;
};

//why is if an expression? what does it return?
class IfExpression: public Expression{
    public:
        Token tok;
        Expression* condition;
        BlockStatement* conseq;
        BlockStatement* alt;

        string tokenLiteral() override;
        string toString() override;
        Obj* eval(Environment* env) override;
};

class Program : public Node{
    public:
        vector<Statement*> statements;

        string tokenLiteral() override;  
        string toString() override;
        Obj* eval(Environment* env) override;
};



Obj* nativeBoolToBooleanObj(bool value);
bool isTruthy(Obj* o);
bool isError(Obj* o);

#endif // AST_H