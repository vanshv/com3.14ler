#ifndef VISITOR_H
#define VISITOR_H

#include "ast.hpp"
#include "obj.hpp"

// this is designed for evalvisitor.
// if we want to use this for compilevisitor, we can just pass nullpointers

// although it is worth looking into what we could do here

class Program;
class Identifier;
class LetStatement;
class ReturnStatement;
class IntegerLiteral;
class CallExpression;
class PrefixExpression;
class InfixExpression;
class IndexExpression;
class HashLiteral;
class StringLiteral;
class ArrayLiteral;
class ExpressionStatement;
class FunctionLiteral;
class Boolean;
class IfExpression;
class BlockStatement;

class NodeVisitor{
    public:
    virtual ~NodeVisitor() = default;
    virtual Obj* visit(Identifier& i, Environment* env) = 0;
    virtual Obj* visit(LetStatement& ls, Environment* env) = 0;
    virtual Obj* visit(ReturnStatement& rs, Environment* env) = 0;
    virtual Obj* visit(IntegerLiteral& il, Environment* env) = 0;
    virtual Obj* visit(CallExpression& ce, Environment* env) = 0;
    virtual Obj* visit(PrefixExpression& pe, Environment* env) = 0;
    virtual Obj* visit(InfixExpression& ie, Environment* env) = 0;
    virtual Obj* visit(IndexExpression& ie, Environment* env) = 0;
    virtual Obj* visit(HashLiteral& hl, Environment* env) = 0;
    virtual Obj* visit(StringLiteral& sl, Environment* env) = 0;
    virtual Obj* visit(ArrayLiteral& al, Environment* env) = 0;
    virtual Obj* visit(ExpressionStatement& el, Environment* env) = 0;
    virtual Obj* visit(FunctionLiteral& fl, Environment* env) = 0;
    virtual Obj* visit(Boolean& b, Environment* env) = 0;
    virtual Obj* visit(IfExpression& ie, Environment* env) = 0;
    virtual Obj* visit(BlockStatement& bs, Environment* env) = 0;
    virtual Obj* visit(Program& p, Environment* env) = 0;
};

#endif