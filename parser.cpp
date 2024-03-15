/*
expression return values and statements don't
for our context it is true, might not generlize

the parser just contains a pointer to the lexer

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
    return nullptr;
}