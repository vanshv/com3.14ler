#include <iostream>
#include <string>
#include <sstream>
#include "parser.hpp"

using namespace std;

const string PROMPT = ">> ";

int main() {
    string line;
    cout << PROMPT;

    Environment* env = new Environment();
    set_builtins();
    while (getline(cin, line)) {
        Lexer* l = new Lexer(line);
        Parser* p = new Parser(l);
        Program* prog = p->parseProgram();
        if(!p->errors.empty()){
            cout<<"errors found\n";
            continue;
        }
        // for(auto i : prog->statements){
        //     cout<<i->toString()<<"\n";
        // }

        Obj* o = prog->eval(env);
        cout<<o->Inspect();
        // for(auto i : prog->statements){
        //     cout<<o->Inspect()<<"\n";
        // }

        cout << "\n" << PROMPT;
    }

    return 0;
}