
#include "ast.hpp"

string Program::tokenLiteral(){
    if(!statements.empty()){
        return statements[0].tokenLiteral();
    }
    return "";
}