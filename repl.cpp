#include <iostream>
#include <string>
#include <sstream>
#include "lexer.hpp"

using namespace std;

const std::string PROMPT = ">> ";

int main() {
    string line;
    cout << PROMPT;

    while (getline(cin, line)) {
        Lexer l(line);
        Token t = l.spitToken();
        while (t.type != MYEOF) {
            cout << t.type <<" "<<t.val << "\n";
            t = l.spitToken();
        }

        cout << PROMPT;
    }

    return 0;
}