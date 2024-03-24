#ifndef PARSER_H
#define PARSER_H

#include "lexer.hpp"
#include "ast.hpp"

enum {
    LOWEST, 
    EQUALS,
    LESSGREATER,
    SUM,
    PRODUCT,
    PREFIX,
    CALL,
};



class Parser{
    public:
        Lexer* l;
        Token currToken;
        Token peekToken;
        vector<pair<TokenType, TokenType>> errors;
        //expected, actual token type
        map<TokenType, Expression* (Parser::*) ()> prefixParseFns;
        map<TokenType, Expression* (Parser::*) (Expression*)> infixParseFns;

        Parser(Lexer* l);
        void eatToken();
        bool expectPeek(TokenType ttype);
        bool currTokenis(TokenType ttype);
        bool peekTokenis(TokenType ttype);

        Program* parseProgram();
        Statement* parseStatement();
        LetStatement* parseLetStatement();
        ReturnStatement* parseReturnStatement();
        ExpressionStatement* parseExpressionStatement();
        Expression* parseExpression(int precedence);
        Expression* parseIdentifier();

        void checkParserErrors();
        void peekError(TokenType ttype);
        void registerPrefix(TokenType ttype, Expression* (Parser::*fn) ());
        void registerInfix(TokenType ttype, Expression* (Parser::*fn) (Expression*));
};

#endif // PARSER_H
