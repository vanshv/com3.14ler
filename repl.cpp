#include <iostream>
#include <string>
#include <sstream>
#include "parser.hpp"
#include "evalVisitor.hpp"

using namespace std;

const string PROMPT = ">> ";

// compile like this - get rid of circular dependencies
// then compile with EvalVisitor

int main() {
    string line;
    cout << PROMPT;

    Environment* env = new Environment();
    set_builtins();
    EvalVisitor evalvis;
    while (getline(cin, line)) {
        Lexer* l = new Lexer(line);
        Parser* p = new Parser(l);
        Program* prog = p->parseProgram();
        if(!p->errors.empty()){
            cout<<"errors found\n";
            continue;
        }

        // Obj* o = prog->eval(env);
        Obj* o = prog->accept(evalvis, env);
        
        cout << o->Inspect();
        cout << "\n" << PROMPT;
    }

    return 0;
}