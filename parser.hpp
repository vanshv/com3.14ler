#ifndef PARSER_H
#define PARSER_H

#include "lexer.hpp"
#include "ast.hpp"

class Parser{
    public:
        Lexer* l;
        Token currToken;
        Token peekToken;
        vector<pair<TokenType, TokenType>> errors;
        //expected, actual token type

        Parser(Lexer* l);
        void eatToken();
        bool expectPeek(TokenType ttype);
        bool currTokenis(TokenType ttype);
        bool peekTokenis(TokenType ttype);
        Program* parseProgram();
        Statement* parseStatement();
        LetStatement* parseLetStatement();
        ReturnStatement* parseReturnStatement();
        void checkParserErrors();
        void peekError(TokenType ttype);
};

#endif // PARSER_H
