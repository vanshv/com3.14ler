/*
expression return values and statements don't
for our context it is true, might not generlize

the parser just contains a pointer to the lexer

do i even bother with error-checking?

do i need to implement the way he does, 
isn't my way better?

think - what will peekError do?
I don't think I need to write testcases with equal to checks,
if it not parse-able it will throw an error itself.

maybe i don't even need gtest this way, could be nice.

*/

#include "parser.hpp"

Parser::Parser(Lexer* l){
    this->l = l;
    eatToken();
    eatToken();
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
            return nullptr;            
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