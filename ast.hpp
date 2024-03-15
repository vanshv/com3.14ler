#ifndef AST_H
#define AST_H

#include<bits/stdc++.h>
using namespace std;

class Node{
    public:
        virtual void tokenLiteral() = 0;
};

class StatementNode : public Node{
    public:
        void tokenLiteral() override{

        }
};

class ExpressionNode : public Node{

};

class Identifier{

};

class Program{
    public:
        vector<StatementNode> statements;
        // Program(){

        // }
        string tokenLiteral();
};

#endif // AST_H
