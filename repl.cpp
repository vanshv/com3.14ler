#include <iostream>
#include <string>
#include <sstream>
#include "lexer.hpp"
#include "parser.hpp"
#include "ast.hpp"
#include "obj.hpp"

using namespace std;

const string PROMPT = ">> ";

int main() {
    string line;
    cout << PROMPT;

    while (getline(cin, line)) {
        Lexer* l = new Lexer(line);
        Parser* p = new Parser(l);
        Program* prog = p->parseProgram();
        if(!p->errors.empty()){
            cout<<"errors found\n";
            continue;
        }
        for(auto i : prog->statements){
            Obj* o = i->eval();
            if(o->Type() != NULL_OBJ){
                cout<<o->Inspect();
            }
        }

        cout << "\n" << PROMPT;
    }

    return 0;
}