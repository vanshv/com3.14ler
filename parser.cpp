/*
expression return values and statements don't
for our context it is true, might not generlize

lmao function pointers wtf
*/

#include "parser.hpp"

Parser::Parser(Lexer* l){
    this->l = l;
    eatToken();
    eatToken();

    //set up parser functions
    registerPrefix(IDENT, &Parser::parseIdentifier);
    registerPrefix(INT, &Parser::parseIntegerLiteral);
    registerPrefix(BANG, &Parser::parsePrefixExpression);
    registerPrefix(MINUS, &Parser::parsePrefixExpression);

    registerInfix(PLUS, &Parser::parseInfixExpression);
    registerInfix(MINUS, &Parser::parseInfixExpression);
    registerInfix(SLASH, &Parser::parseInfixExpression);
    registerInfix(ASTERISK, &Parser::parseInfixExpression);
    registerInfix(EQ, &Parser::parseInfixExpression);
    registerInfix(NOT_EQ, &Parser::parseInfixExpression);
    registerInfix(LT, &Parser::parseInfixExpression);
    registerInfix(GT, &Parser::parseInfixExpression);
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
        default:        return LOWEST;
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

    //todo: parse expression

    while(currToken.type != SEMICOLON){
        eatToken();
    }

    return lstmt;
}

ReturnStatement* Parser::parseReturnStatement(){
    ReturnStatement *rstmt = new ReturnStatement();

    rstmt->tok = currToken;

    while(currToken.type != SEMICOLON){
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
    while(!peekTokenis(SEMICOLON) && precedence < peekPrecedence()){
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
    
    Prenum prec = currPrecedence();
    eatToken();
    ie->right = parseExpression(prec);

    return ie;
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

Prenum Parser::peekPrecedence(){
    return precedences(peekToken.type);
}

Prenum Parser::currPrecedence(){
    return precedences(currToken.type);
}