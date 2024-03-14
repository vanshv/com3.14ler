#ifndef LEXER_H
#define LEXER_H

#include<bits/stdc++.h>
using namespace std;

enum TokenType{
    ASSIGN,
    SEMICOLON,
    LPAREN,
    RPAREN,
    COMMA,
    PLUS,
    LBRACE,
    RBRACE,
    MYEOF,
    LET,
    IDENT,
    INT,
    FUNCTION,
};

class Token{
    public:
        TokenType type;
        string val;
        Token(TokenType type, string val);
        Token();
};

class Lexer{
    public: 
        string input;
        char ch;
        int pos;
        int redpos; // or readpos
        vector<Token> output;

        Lexer(string input);
        void readChar();
        Token spitToken();
};


#endif // LEXER_H