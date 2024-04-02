/*
expression return values and statements don't
for our context it is true, might not generlize


skipped
lmao function pointers wtf
pratt parser working
cleaning up test suite
adding booleans to tests
grouped expressions and below
*/

#include "parser.hpp"

Parser::Parser(Lexer* l){
    this->l = l;
    eatToken();
    eatToken();

    //set up parser functions
    //could set up lookup table instead of maps
    registerPrefix(IDENT, &Parser::parseIdentifier);
    registerPrefix(INT, &Parser::parseIntegerLiteral);
    registerPrefix(BANG, &Parser::parsePrefixExpression);
    registerPrefix(MINUS, &Parser::parsePrefixExpression);
    registerPrefix(TRUE, &Parser::parseBoolean);
    registerPrefix(FALSE, &Parser::parseBoolean);
    registerPrefix(LPAREN, &Parser::parseGroupedExpression);
    registerPrefix(IF, &Parser::parseIfExpression);
    registerPrefix(FUNCTION, &Parser::parseFunctionLiteral);

    registerInfix(PLUS, &Parser::parseInfixExpression);
    registerInfix(MINUS, &Parser::parseInfixExpression);
    registerInfix(SLASH, &Parser::parseInfixExpression);
    registerInfix(ASTERISK, &Parser::parseInfixExpression);
    registerInfix(EQ, &Parser::parseInfixExpression);
    registerInfix(NOT_EQ, &Parser::parseInfixExpression);
    registerInfix(LT, &Parser::parseInfixExpression);
    registerInfix(GT, &Parser::parseInfixExpression);
    registerInfix(LPAREN, &Parser::parseCallExpression);
}

Prenum Parser::precedences(TokenType t){
    switch(t)
    {
        case EQ:        return EQUALS;
        case NOT_EQ:    return EQUALS;
        case LT:        return LESSGREATER;
        case GT:        return LESSGREATER;
        case PLUS:      return SUM;
        case MINUS:     return SUM;
        case SLASH:     return PRODUCT;
        case ASTERISK:  return PRODUCT;
        case LPAREN:    return CALL;
    }
    return LOWEST;
}

void Parser::eatToken(){
    currToken = peekToken;
    peekToken = l->spitToken();
}

Program* Parser::parseProgram(){
    Program *prog = new Program();
    
    while(currToken.type != MYEOF){
        Statement* stmt = parseStatement();
        if(stmt != nullptr){
            prog->statements.push_back(stmt);
        }
        eatToken();
    }   

    return prog;
}

Statement* Parser::parseStatement(){
    switch(currToken.type){
        case LET:
            return parseLetStatement();
        case RETURN:
            return parseReturnStatement();
        default:
            return parseExpressionStatement();            
    }
}

bool Parser::currTokenis(TokenType ttype){
    return (ttype == currToken.type);
}

bool Parser::peekTokenis(TokenType ttype){
    return (ttype == peekToken.type);
}

bool Parser::expectPeek(TokenType ttype){
    if(!peekTokenis(ttype)){
        peekError(ttype);
        return false;
    }
    eatToken();
    return true;
}

LetStatement* Parser::parseLetStatement(){
    LetStatement *lstmt = new LetStatement();

    //we only move forward tokens if we know the next thing is right
    //this is going to be reused multiple times in implementation
    if(!expectPeek(IDENT)){
        return nullptr;
    }

    lstmt->name = new Identifier(currToken, currToken.val);

    if(!expectPeek(ASSIGN)){
        return nullptr;
    }

    eatToken();

    lstmt->value = parseExpression(LOWEST);

    if(peekToken.type == SEMICOLON){
        eatToken();
    }

    return lstmt;
}

ReturnStatement* Parser::parseReturnStatement(){
    ReturnStatement *rstmt = new ReturnStatement();

    rstmt->tok = currToken;

    eatToken();

    rstmt->value = parseExpression(LOWEST);

    if(peekToken.type == SEMICOLON){
        eatToken();
    }

    return rstmt;
}

ExpressionStatement* Parser::parseExpressionStatement(){
    ExpressionStatement *expStmt = new ExpressionStatement();
    expStmt->tok = currToken;
    expStmt->expression = parseExpression(LOWEST);

    if(peekTokenis(SEMICOLON)){
        eatToken();
    }

    return expStmt;
}

void Parser::noPrefixParseFnError(){
    cout<<"no prefix parse function found for "<<currToken.val<<"\n";
    //errors.append();
}

Expression* Parser::parseExpression(int precedence){
    auto prefix = prefixParseFns[currToken.type];
    if(prefix == nullptr){
        noPrefixParseFnError();
        return nullptr;
    }

    Expression *leftExp = (this->*prefix)();
    while(!peekTokenis(SEMICOLON) && precedence < precedences(peekToken.type)){
        auto infix = infixParseFns[peekToken.type];
        if(infix == nullptr){
            return leftExp;
        }

        eatToken();

        leftExp = (this->*infix)(leftExp);
    }

    return leftExp;
}

Expression* Parser::parseIdentifier(){
    Identifier* id = new Identifier(currToken, currToken.val);
    return id;
}

Expression* Parser::parseIntegerLiteral(){
    IntegerLiteral *il = new IntegerLiteral();
    il->tok = currToken;
    il->value = stoi(currToken.val);
    return il;
}

Expression* Parser::parsePrefixExpression(){
    PrefixExpression* pe = new PrefixExpression();
    pe->tok = currToken;
    pe->op = currToken.val;
    
    eatToken();
    pe->right = parseExpression(PREFIX);

    return pe;
}

Expression* Parser::parseInfixExpression(Expression* left){
    InfixExpression* ie = new InfixExpression();
    ie->tok = currToken;
    ie->op = currToken.val;
    ie->left = left;
    
    Prenum prec = precedences(currToken.type);
    eatToken();
    ie->right = parseExpression(prec);

    return ie;
}

Expression* Parser::parseBoolean(){
    Boolean* bo = new Boolean();
    bo->tok = currToken;
    bo->value = (currToken.type == TRUE);

    return bo;
}

Expression* Parser::parseGroupedExpression(){
    eatToken();

    Expression* ex = parseExpression(LOWEST);
    if(!expectPeek(RPAREN)){
        return nullptr;
    }

    return ex;
}

Expression* Parser::parseIfExpression(){
    IfExpression* ie = new IfExpression();
    ie->tok = currToken;

    if(!expectPeek(LPAREN)){
        return nullptr;
    }

    eatToken();
    ie->condition = parseExpression(LOWEST);

    if(!expectPeek(RPAREN)){
        return nullptr;
    }
    if(!expectPeek(LBRACE)){
        return nullptr;
    }

    ie->conseq = parseBlockStatement();

    if(peekToken.type == ELSE){
        eatToken();
        if(expectPeek(RPAREN)){
            return nullptr;
        }
        ie->alt = parseBlockStatement();
    }

    return ie;
}

BlockStatement* Parser::parseBlockStatement(){
    BlockStatement* bs = new BlockStatement();
    bs->tok = currToken;

    eatToken();

    while(peekToken.type != RBRACE && peekToken.type != MYEOF){
        Statement* st = parseStatement();
        if(st != nullptr){
            bs->stmts.push_back(st);
        }
        eatToken(); //semicolon 
    }

    return bs;
}

Expression* Parser::parseFunctionLiteral(){
    FunctionLiteral* fl = new FunctionLiteral();
    fl->tok = currToken;

    if(!expectPeek(LPAREN)){
        return nullptr;
    }

    fl->parameters = parseParameters();

    if(!expectPeek(LBRACE)){
        return nullptr;
    }

    fl->body = parseBlockStatement();

    return fl;
}

vector<Identifier*> Parser::parseParameters(){
    vector<Identifier*> params;
    if(expectPeek(RBRACE)){
        return params;
    }
    eatToken();

    Identifier* id = new Identifier(currToken, currToken.val);
    params.push_back(id);
    while(peekToken.type != RBRACE){
        eatToken();
        eatToken();
        Identifier* id = new Identifier(currToken, currToken.val);
        params.push_back(id);
    }

    if(!expectPeek(RPAREN)){
        params.clear();
    }

    return params;
}

Expression* Parser::parseCallExpression(Expression* left){
    CallExpression* ce = new CallExpression();
    ce->tok = currToken;
    ce->function = left;
    ce->args = parseCallArguements();

    return ce;
}

vector<Expression*> Parser::parseCallArguements(){
    vector<Expression*> args;

    if(expectPeek(RBRACE)){
        return args;
    }

    eatToken();
    Expression* exp = parseExpression(LOWEST);
    args.push_back(exp);

    if(peekToken.type == COMMA){
        eatToken();
        eatToken();
        Expression* exp = parseExpression(LOWEST);
        args.push_back(exp);
    }

    if(!expectPeek(RPAREN)){
        args.clear();
    }

    return args;
}

void Parser::peekError(TokenType ttype){
    errors.push_back({ttype, currToken.type});
}

void Parser::checkParserErrors(){
    if(errors.empty()){
        return;
    }

    cout<<"parser has "<<errors.size()<<" errors\n";
    for(auto [a, b] : errors){
        cout<<a<<" "<<b<<"\n";
    }
}

void Parser::registerPrefix(TokenType ttype, Expression* (Parser::*fn) ()) {
    prefixParseFns[ttype] = fn;
}

void Parser::registerInfix(TokenType ttype, Expression* (Parser::*fn) (Expression*)) {
    infixParseFns[ttype] = fn;
}