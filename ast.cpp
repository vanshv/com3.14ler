
#include "ast.hpp"

string Program::tokenLiteral(){
    if(!statements.empty()){
        return statements[0]->tokenLiteral();
    }
    return "";
}

Identifier::Identifier(Token tok, string value){
    this->tok = tok;
    this->value = value;
}

LetStatement::LetStatement(){

}
ReturnStatement::ReturnStatement(){

}


string LetStatement::tokenLiteral(){
    return tok.val;
}
string ReturnStatement::tokenLiteral(){
    return tok.val;
}

string Identifier::tokenLiteral(){
    return tok.val;
}