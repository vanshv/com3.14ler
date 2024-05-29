/*

why do i need to read next char?
interpreter will support ascii, not
unicode.  what is the difference?

switch statements do not make checks one by one, so how is precedence decided here?

create type to string mapping for repl debugging

break up lexer into token and lexer
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
    nextpos = pos + 1;
    ch = input[pos];
    setKeywords();
    // readChar();
}

void Lexer::setKeywords(){
    keywords = {
        {"let", LET},
        {"fn", FUNCTION},
        {"if", IF},
        {"else", ELSE},
        {"return", RETURN},
        {"true", TRUE},
        {"false", FALSE},
    };
}

//read the value and nextpos and move the pointer
void Lexer::readChar(){
    if(pos >= input.size()){
        ch = 0;
    }
    else{
        ch = input[nextpos];
    }
    pos = nextpos;
    nextpos++;
}

void Lexer::eatWhiteSpace(){
    while (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r') {
        readChar();
    }
}

bool Lexer::isDigit(){
    return (ch >= '0' && ch <= '9');
}

bool Lexer::isLetter(){
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z' || ch == '_');
}

string Lexer::readNumber(){
    int save = pos;
    while(isDigit()){
        readChar();
    }
    return input.substr(save, pos - save);
}

string Lexer::readIdentifier(){
    int save = pos;
    while(isLetter()){
        readChar();
    }
    return input.substr(save, pos - save);
}

Token Lexer::spitToken(){
    Token t;
    eatWhiteSpace();

    switch(ch){
        case '=':
        {
            readChar();
            if(ch == '='){
                t = Token(EQ, "==");
            }
            else{
                t = Token(ASSIGN, "=");
                return t;
            }
            break;
        }
        case '+':
            t = Token(PLUS, "+");
            break;
        case '-':
            t = Token(MINUS, "-");
            break;
        case '!':
        {
            readChar();
            if(ch == '='){
                t = Token(NOT_EQ, "!=");
            }
            else{
                t = Token(BANG, "!");
                return t;
            }
            break;
        }
        case '/':
            t = Token(SLASH, "/");
            break;
        case '*':
            t = Token(ASTERISK, "*");
            break;
        case '<':
            t = Token(LT, "<");
            break;
        case '>':
            t = Token(GT, ">");
            break;
        case ';':
            t = Token(SEMICOLON, ";");
            break;
        case ',':
            t = Token(COMMA, ",");
            break;
        case '{':
            t = Token(LBRACE, "{");
            break;
        case '}':
            t = Token(RBRACE, "}");
            break;
        case '(':
            t = Token(LPAREN, "(");
            break;
        case ')':
            t = Token(RPAREN, ")");
            break;
        case '[':
            t = Token(LBRACKET, "[");
            break;
        case ']':
            t = Token(RBRACKET, "]");
            break;
        case '"':
            t = Token(STRING, readString());
            break;
        case ':':
            t = Token(COLON, ":");
            break;
        case 0:
            t = Token(MYEOF, "");
            break;
        default:
            if(isLetter()){
                string res = readIdentifier();
                if(keywords.find(res) != keywords.end()){
                    t = Token(keywords[res], res);
                }
                else{
                    t = Token(IDENT, res);
                }
                return t;
            }
            else if(isDigit()){
                string res = readNumber();
                t = Token(INT, res);
                return t;
            }
            else{
                t = Token(ILLEGAL, string(ch, 1));
            }
    }

    readChar();
    return t;
}

string Lexer::readString(){
    int begin = pos + 1;
    while(true){
        readChar();
        if(ch == '"' || ch == 0){
            break;
        }
    }

    return input.substr(begin, pos - begin);
}