/*

why do i need to read next char?
interpreter will support ascii, not
unicode.  what is the difference?

*/

#include "lexer.hpp"

Token::Token(){

}
Token::Token(TokenType type, string val){
    this->type = type;
    this->val = val;
}

Lexer::Lexer(string input){
    this->input = input;
    pos = 0;
    redpos = pos - 1;
    ch = '.';
    readChar();
}

void Lexer::readChar(){
    if(pos >= input.size()){
        ch = 0;
    }
    else{
        ch = input[pos];
    }
    redpos = pos;
    pos++;
}

Token Lexer::spitToken(){
    Token t;

    switch(ch){
        case '=':
            t = Token(ASSIGN, "=");
            break;
        case '+':
            t = Token(PLUS, "+");
            break;
        case '(':
            t = Token(LPAREN, "(");
            break;
        case ')':
            t = Token(RPAREN, ")");
            break;
        case '{':
            t = Token(LBRACE, "{");
            break;
        case '}':
            t = Token(RBRACE, "}");
            break;
        case ',':
            t = Token(COMMA, ",");
            break;
        case ';':
            t = Token(SEMICOLON, ";");
            break;
        default:
            t = Token(MYEOF, "");
    }

    readChar();
    return t;
}

// int main(){
//     return 0;
// }