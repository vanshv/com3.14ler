#include "lexer.hpp"
#include "parser.hpp"
#include<string>

using namespace std;

void testLetStatements(){
    string input = "\
    let x = 5;\
    let y = 10;\
    let foobar = 838383;\
    ";
    // string input = "\
    // let x 5;\
    // let = 10;\
    // let 838383;\
    // ";
    Lexer *l = new Lexer(input);
    Parser *p = new Parser(l);

    Program *prog = p->parseProgram();
    if(prog == nullptr){
        cout<<"parseProgram() returned nullptr\n";
        return;
    }
	p->checkParserErrors();

    if(prog->statements.size() != 3){
        cout << "program has not enough statements. expected=3, got=" << prog->statements.size() << endl;
        return;
    }

    vector<string> expectedIdentifiers = {"x", "y", "foobar"};

    for(int i = 0; i < expectedIdentifiers.size(); i++){
        Statement* stmt = prog->statements[i];
        LetStatement *letStmt = dynamic_cast<LetStatement*>(stmt);
        if(letStmt == nullptr){
            cout<<"stmt is not LetStatement\n";
            return;
        }
        if(letStmt->name->value != expectedIdentifiers[i]){
            cout << " expected " << letStmt->name->value << " found " << i << "\n";
            return;
        }
        if(letStmt->name->tokenLiteral() != expectedIdentifiers[i]){
            cout << " expected " << letStmt->name->tokenLiteral() << " found " << i << "\n";
            return;
        }
    }

    cout<<"testLetStatements passed\n";
}

void testReturnStatements(){
    string input = "\
    return 5;\
    return 10;\
    return 993322;\
    ";
    // string input = "\
    // let x 5;\
    // let = 10;\
    // let 838383;\
    // ";
    Lexer *l = new Lexer(input);
    Parser *p = new Parser(l);

    Program *prog = p->parseProgram();
    if(prog == nullptr){
        cout<<"parseProgram() returned nullptr\n";
        return;
    }
	p->checkParserErrors();

    if(prog->statements.size() != 3){
        cout << "program has not enough statements. expected=3, got=" << prog->statements.size() << endl;
        return;
    }

    for(auto i : prog->statements){
        Statement* stmt = i;
        ReturnStatement *retStmt = dynamic_cast<ReturnStatement*>(stmt);
        if(retStmt == nullptr){
            cout<<"stmt is not returnStatement\n";
            return;
        }
        if(retStmt->tokenLiteral() != "return"){
            cout << " expected " << retStmt->tokenLiteral() << " found " << i << "\n";
            return;
        }
    }

    cout<<"testReturnStatements passed\n";
}

void testString(){
    Program* prog = new Program();
    LetStatement* letStmt = new LetStatement();

    Token tok = Token(LET, "let");
    letStmt->tok = tok;

    Identifier* ident = new Identifier(Token{IDENT, "myVar"}, "myVar");
    letStmt->name = ident;

    Identifier* anotherIdent = new Identifier(Token{IDENT, "anotherVar"}, "anotherVar");
    letStmt->value = anotherIdent;

    prog->statements.push_back(letStmt);

    if(prog->String() != "let myVar = anotherVar;"){
        cout<<"program.String() wrong. got=" << prog->String() << "\n";
        return;
    }
    cout<<"testString passed\n";
}

void testIdentiferExpression(){
    string input = "\
    foobar;\
    ";
    Lexer *l = new Lexer(input);
    Parser *p = new Parser(l);

    Program *prog = p->parseProgram();
    if(prog == nullptr){
        cout<<"parseProgram() returned nullptr\n";
        return;
    }
	p->checkParserErrors();

    if(prog->statements.size() != 1){
        cout << "program has not enough statements. expected=1, got=" << prog->statements.size() << endl;
        return;
    }

    Statement* stmt = prog->statements[0];
    ExpressionStatement *expStmt = dynamic_cast<ExpressionStatement*>(stmt);
    if(expStmt == nullptr){
        cout<<"stmt is not expressionStatement\n";
        return;
    }
    Identifier *ident = dynamic_cast<Identifier*>(expStmt->expression);
    if(ident == nullptr){
        cout<<"expStmt->expression is not Identifier\n";
        return;
    }
    if(ident->value != "foobar"){
        cout << " expected " << "foobar" << " found " << ident->value << "\n";
        return;
    }
    if(ident->tokenLiteral() != "foobar"){
        cout << " expected " << "foobar" << " found " << ident->tokenLiteral() << "\n";
        return;
    }

    cout<<"testIdentifierExpression passed\n";
}

int main() {

    testLetStatements();
    testReturnStatements();
    testString();
    testIdentiferExpression();

    return 0;
}