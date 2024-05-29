#ifndef LEXER_H
#define LEXER_H

#include<bits/stdc++.h>
using namespace std;

enum TokenType {
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
    ILLEGAL,
    MINUS,
    BANG,
    ASTERISK,
    SLASH,
    LT,
    GT,
    IF,
    ELSE,
    RETURN,
    EQ,
    NOT_EQ,
    TRUE,
    FALSE,
    STRING,
    LBRACKET,
    RBRACKET,
    COLON,
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
        char ch; // points to pos
        int pos; // points to ch
        int nextpos; // next pos after pos

        map<string, TokenType> keywords;

        Lexer(string input);
        void readChar();
        Token spitToken();
        void setKeywords();
        void eatWhiteSpace();
        bool isLetter();
        bool isDigit();
        string readNumber();
        string readIdentifier();
        string readString();
};

#endif // LEXER_H