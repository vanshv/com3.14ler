#ifndef AST_H
#define AST_H

#include<bits/stdc++.h>
#include "lexer.hpp"
using namespace std;

class Node{
    public:
        virtual string tokenLiteral() = 0;
        virtual string String() = 0;
        // write tester for String output   
};

class Statement : public Node{
};

class Expression : public Node{
};

class Identifier : public Expression {
    public:
        Token tok;
        string value;

        Identifier(Token tok, string value);
        string tokenLiteral() override;
        string String() override;
};

class IntegerLiteral : public Expression{
    public:
        Token tok;
        int value;

        string tokenLiteral() override;
        string String() override;
};

class PrefixExpression : public Expression{
    public:
        Token tok;
        string op;
        Expression* right;

        string tokenLiteral() override;
        string String() override;
};

class InfixExpression : public Expression{
    public:
        Token tok;
        string op;
        Expression* right;
        Expression* left;

        string tokenLiteral() override;
        string String() override;
};


// sub-statements do not need constructors
// parsing might fail, so we need to allocate bit-by-bit
class LetStatement : public Statement{
    public:
        Token tok;
        Identifier *name;
        Expression *value;

        string tokenLiteral() override;
        string String() override;
};

class ReturnStatement : public Statement{
    public:
        Token tok;
        Expression *value;

        string tokenLiteral() override;
        string String() override;
};

class ExpressionStatement : public Statement{
    public:
        Token tok;
        Expression *expression;

        string tokenLiteral() override;
        string String() override;
};

class Program : public Node{
    public:
        vector<Statement*> statements;

        string tokenLiteral();  
        string String();
};

#endif // AST_H
