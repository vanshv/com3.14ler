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

    if(prog->toString() != "let myVar = anotherVar;"){
        cout<<"program.toString() wrong. got=" << prog->toString() << "\n";
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

void testIntegerLiteral(){
    string input = "\
    5;\
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
    IntegerLiteral *ident = dynamic_cast<IntegerLiteral*>(expStmt->expression);
    if(ident == nullptr){
        cout<<"expStmt->expression is not Identifier\n";
        return;
    }
    if(ident->value != 5){
        cout << " expected " << 5 << " found " << ident->value << "\n";
        return;
    }
    if(ident->tokenLiteral() != "5"){
        cout << " expected " << "foobar" << " found " << ident->tokenLiteral() << "\n";
        return;
    }

    cout<<"testIdentifierExpression passed\n";
}

bool testIntegerLiteralHelper(Expression* exp, int intval){
    IntegerLiteral *ident = dynamic_cast<IntegerLiteral*>(exp);
    if(ident == nullptr){
        cout<<"pref->right is not integer\n";
        return false;
    }
    if(ident->value != intval){
        cout << " expected " << intval << " found " << ident->value << "\n";
        return false;
    }
    if(ident->tokenLiteral() != to_string(intval)){
        cout << " expected " << to_string(intval) << " found " << ident->tokenLiteral() << "\n";
        return false;
    }
    return true;
}

void testPrefixExpression(){
    vector<string> tests = {"-5", "!15"};
    vector<pair<string, int>> to = {{"-", 5}, {"!", 15}};

    for(int i = 0; i < tests.size(); i++){
        string input = tests[i];
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
        ExpressionStatement* expst = dynamic_cast<ExpressionStatement*> (stmt);
        if(expst == nullptr){
            cout<<"stmt is not ExpressionStatement\n";
        }


        PrefixExpression *pe = dynamic_cast<PrefixExpression*>(expst->expression);
        if(pe == nullptr){
            cout<<"ExpressionStatement is not PrefixExpression\n";
            return;
        }
        if(pe->tokenLiteral() != to[i].first){
            cout<<"pe->tokenLiteral is "<<pe->tokenLiteral() <<" instead of "<<to[i].first<<"\n";
        }
        if(!testIntegerLiteralHelper(pe->right, to[i].second)){
            return;
        }
    }

    cout<<"prefixExpression tests passed\n";
}

void testParsingInfixExpressions(){
    struct T{
        string input;
        int leftValue;
        string op;
        int rightValue;
    };
    vector<T> infixTests = {
        {"5 + 5;", 5, "+", 5},
        {"5 - 5;", 5, "-", 5},
        {"5 * 5;", 5, "*", 5},
        {"5 / 5;", 5, "/", 5},
        {"5 > 5;", 5, ">", 5},
        {"5 < 5;", 5, "<", 5},
        {"5 == 5;", 5, "==", 5},
        {"5 != 5;", 5, "!=", 5},
    };

    for (const auto& tt : infixTests) {
        Lexer lexer(tt.input);
        Parser parser(&lexer);
        Program* program = parser.parseProgram();

        if (program->statements.size() != 1) {
            cout << "Program does not have exactly one statement" <<program->statements.size()<<"\n";
            return;
        }

        Statement* stmt = program->statements[0];
        ExpressionStatement* expStmt = dynamic_cast<ExpressionStatement*>(stmt);
        if (expStmt == nullptr) {
            cout << "Statement is not an ExpressionStatement\n";
            return;
        }

        Expression* exp = expStmt->expression;
        InfixExpression* infixExp = dynamic_cast<InfixExpression*>(exp);
        if (infixExp == nullptr) {
            cout << "Expression is not an InfixExpression\n";
            return;
        }

        if(!testIntegerLiteralHelper(infixExp->left, tt.leftValue)){
            return;
        }
        if (infixExp->op != tt.op) {
            cout << "Infix operator is incorrect. Expected: " << tt.op
                      << ", Got: " << infixExp->op << "\n";
            return;
        }
        if(!testIntegerLiteralHelper(infixExp->right, tt.rightValue)){
            return;
        }
    }

    cout<<"InfixExpression tests passed \n";
}

void testOperatorPrecedenceParsing(){
    struct T{
        string input;
        string expected;
    };
    vector<T> tests = {
        {"-a * b", "((-a)*b)"},
        {"!-a", "(!(-a))"},
        {"a + b + c", "((a+b)+c)"},
        {"a + b - c", "((a+b)-c)"},
        {"a * b * c", "((a*b)*c)"},
        {"a * b / c", "((a*b)/c)"},
        {"a + b / c", "(a+(b/c))"},
        {"a + b * c + d / e - f", "(((a+(b*c))+(d/e))-f)"},
        {"3 + 4; -5 * 5", "(3+4)((-5)*5)"},
        {"5 > 4 == 3 < 4", "((5>4)==(3<4))"},
        {"5 < 4 != 3 > 4", "((5<4)!=(3>4))"},
        {"3 + 4 * 5 == 3 * 1 + 4 * 5", "((3+(4*5))==((3*1)+(4*5)))"},
        {"3 + 4 * 5 == 3 * 1 + 4 * 5", "((3+(4*5))==((3*1)+(4*5)))"},
    };

    for (const auto& tt : tests) {
        Lexer lexer(tt.input);
        Parser parser(&lexer);
        Program* program = parser.parseProgram();

        parser.checkParserErrors();

        string actual = program->toString();
        if (actual != tt.expected) {
            cout << "Test failed for input: " << tt.input << endl;
            cout << "Expected: " << tt.expected << ", Got: " << actual << endl;
            return;
        }
    }
    cout<<"Operator Precedence tests passed\n";
}

int main() {

    testLetStatements();
    testReturnStatements();
    testString();
    testIdentiferExpression();
    testIntegerLiteral();
    testPrefixExpression();
    testParsingInfixExpressions();
    testOperatorPrecedenceParsing();

    return 0;
}