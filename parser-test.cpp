#include "lexer.hpp"
#include "parser.hpp"
#include<string>

using namespace std;

int main() {
    string input = "\
    let x 5;\
    let = 10;\
    let foobar 838383;";
    // Lexer lexer(input);

    Lexer *l = new Lexer(input);
    Parser *p = new Parser(l);

    Program *prog = p->parseProgram();
	p->checkParserErrors();

    return 0;
}