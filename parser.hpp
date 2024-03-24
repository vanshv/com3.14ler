#ifndef PARSER_H
#define PARSER_H

#include "lexer.hpp"
#include "ast.hpp"

enum Prenum {
    LOWEST = 0, 
    EQUALS = 1,
    LESSGREATER = 2,
    SUM = 3,
    PRODUCT = 4,
    PREFIX = 5,
    CALL = 6,
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
        Prenum precedences(TokenType);
        // function, but basically a map

        Parser(Lexer* l);
        void eatToken();
        bool expectPeek(TokenType ttype);
        bool currTokenis(TokenType ttype);
        bool peekTokenis(TokenType ttype);
        Prenum peekPrecedence();
        Prenum currPrecedence();

        Program* parseProgram();
        Statement* parseStatement();
        LetStatement* parseLetStatement();
        ReturnStatement* parseReturnStatement();
        ExpressionStatement* parseExpressionStatement();
        Expression* parseExpression(int precedence);
        Expression* parseIdentifier();
        Expression* parseIntegerLiteral();
        Expression* parsePrefixExpression();
        Expression* parseInfixExpression(Expression* );

        void checkParserErrors();
        void peekError(TokenType ttype);
        void noPrefixParseFnError();
        void registerPrefix(TokenType ttype, Expression* (Parser::*fn) ());
        void registerInfix(TokenType ttype, Expression* (Parser::*fn) (Expression*));
};

#endif // PARSER_H
