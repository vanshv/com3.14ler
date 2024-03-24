
#include "ast.hpp"

Identifier::Identifier(Token tok, string value){
    this->tok = tok;
    this->value = value;
}

string Identifier::tokenLiteral(){
    return tok.val;
}

string Identifier::String(){
    return value;
}

string LetStatement::tokenLiteral(){
    return tok.val;
}

string LetStatement::String(){
    string ret = "";
    ret += tok.val + " ";
    ret += name->String() + " = ";
    if(value != nullptr){
        ret += value->String();
    }
    ret += ";";
    return ret;
}


string ReturnStatement::tokenLiteral(){
    return tok.val;
}

string ReturnStatement::String(){
    string ret = "";
    ret += tok.val + " ";
    if(value != nullptr){
        ret += value->String();
    }
    ret += ";";
    return ret;
}


string ExpressionStatement::tokenLiteral(){
    return tok.val;
}

string ExpressionStatement::String(){
    if(expression != nullptr){
        return expression->String();
    }
    return "";
}

string IntegerLiteral::tokenLiteral(){
    return tok.val;
}

string IntegerLiteral::String(){
    return to_string(value);
}

string PrefixExpression::tokenLiteral(){
    return tok.val;
}

string PrefixExpression::String(){
    string ret = "(";
    ret += op;
    ret += right->String();
    ret += ")";
    return ret;
}

string InfixExpression::tokenLiteral(){
    return tok.val;
}

string InfixExpression::String(){
    string ret = "(";
    ret += left->String();
    ret += op;
    ret += right->String();
    ret += ")";
    return ret;
}

string Program::tokenLiteral(){
    if(!statements.empty()){
        return statements[0]->tokenLiteral();
    }
    return "";
}

string Program::String(){
    string ret = "";
    for(auto stmt : statements){
        ret += stmt->String();
    }
    return ret;
}