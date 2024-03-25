
#include "ast.hpp"


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

string FunctionLiteral::tokenLiteral(){
    return tok.val;
}

string FunctionLiteral::String(){
    string ret = tok.val;
    ret += "(";
    for(auto i : parameters){
        ret += i->String() + ",";
    }
    ret += ")";
    ret += body->String();
    return ret;
}

string CallExpression::tokenLiteral(){
    return tok.val;
}

string CallExpression::String(){
    string ret = function->String();
    ret += "(";
    for(auto i : args){
        ret += i->String() + ",";
    }
    ret += ")";

    return ret;
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

string Boolean::tokenLiteral(){
    return tok.val;
}

string Boolean::String(){
    return tok.val;
}

string IfExpression::tokenLiteral(){
    return tok.val;
}

string IfExpression::String(){
    string ret = tokenLiteral();
    ret += "(";
    ret += condition->String();
    ret += ")";
    ret += conseq->String();

    if(alt != nullptr){
        ret += "else";
        ret += alt->String();
    }

    return ret;
}

string BlockStatement::tokenLiteral(){
    return tok.val;
}

string BlockStatement::String(){
    string ret = "{";
    for(auto i : stmts){
        ret += i->String();
    }
    ret += "}";
    return ret;
}