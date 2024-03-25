#include <iostream>
#include <string>
#include <sstream>
#include "lexer.hpp"
#include "parser.hpp"
#include "ast.hpp"

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
            cout<<i->String();
        }

        cout << "\n" << PROMPT;
    }

    return 0;
}