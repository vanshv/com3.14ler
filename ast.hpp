#ifndef AST_H
#define AST_H

#include<bits/stdc++.h>
#include "lexer.hpp"
using namespace std;

class Node{
    public:
        virtual string tokenLiteral() = 0;
};

class Statement : public Node{
};

class Expression : public Node{
};

class Identifier : Expression {
    public:
        Token tok;
        string value;
        string tokenLiteral() override;
        Identifier(Token tok, string value);
};

class LetStatement : public Statement{
    public:
        LetStatement();
        string tokenLiteral() override;
        Token tok;
        Identifier *name;
        Expression *value;
};

class ReturnStatement : public Statement{
    public:
        ReturnStatement();
        string tokenLiteral() override;
        Token tok;
        Expression *value;
};

//should this be statements or statement pointers?
class Program{
    public:
        vector<Statement*> statements;
        // Program(){

        // }
        string tokenLiteral();  
};

#endif // AST_H
