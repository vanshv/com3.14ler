#include <bits/stdc++.h>

using namespace std;

class Expression;
class Statement;

class NodeVisitor{
    public:
    virtual ~NodeVisitor() = default;
    virtual void visit(Expression& Expression) = 0;
    virtual void visit(Statement& Statement) = 0;
};

class Node{
    public:
    virtual ~Node() = default;
    virtual void accept(NodeVisitor& visitor) = 0;
};

class Expression : public Node{
    public:
    Expression(){
        cout << "Expression::Expression()\n";
    }
    void accept(NodeVisitor& visitor) {
        cout << "Expression::accept()\n";
        visitor.visit(*this);
    }
};

class Statement : public Node{
    public:
    Statement(){
        cout << "Statement::Statement()\n";
    }   
    void accept(NodeVisitor& visitor) {
        cout << "Statement::accept()\n";
        visitor.visit(*this);
    }
};

class EvalVisitor : public NodeVisitor{
    public:
    void visit(Expression& Expression){
        cout << "Evaluating Expression from Nodevisitor\n";
    }
    void visit(Statement& Statement){
        cout << "Evaluating Statement from Nodevisitor\n";
    }
};

class CompileVisitor : public NodeVisitor{
    public:
    void visit(Expression& Expression){
        cout << "Compiling Expression from Nodevisitor\n";
    }
    void visit(Statement& Statement){
        cout << "Compiling Statement from Nodevisitor\n";
    }
};

void evalAllNode(vector<Node*> Nodes){
    EvalVisitor dmv;
    // this will also pass environment to accept
    // environment will be enclosed in visitorcontext class
    for(auto m : Nodes){
        m->accept(dmv);
    }
}

void compileAllNode(vector<Node*> Nodes){
    CompileVisitor dmv;
    for(auto m : Nodes){
        m->accept(dmv);
    }
}

int main() {
    vector<Node*> nodes;
    nodes.emplace_back(new Expression());
    nodes.emplace_back(new Statement());
    cout << "\n";

    evalAllNode(nodes);
    cout << "\n";
    compileAllNode(nodes);
}