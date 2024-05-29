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
    //might need to check this
    INDEX = 7,
};
//higher precedence is evaluated first

class Parser{
    public:
        Lexer* l;
        Token currToken;
        Token peekToken;
        // expected, actual token type
        vector<pair<TokenType, TokenType>> errors;

        // could turn these maps to loopup tables
        map<TokenType, Expression* (Parser::*) ()> prefixParseFns;
        map<TokenType, Expression* (Parser::*) (Expression*)> infixParseFns;
        Prenum precedences(TokenType);
        // function, but basically a map

        Parser(Lexer* l);
        void eatToken();
        bool expectPeek(TokenType ttype);
        bool currTokenis(TokenType ttype);
        bool peekTokenis(TokenType ttype);

        Program* parseProgram();
        Statement* parseStatement();
        //can i replace these with statement pointers?
        //should i?
        LetStatement* parseLetStatement();
        ReturnStatement* parseReturnStatement();
        ExpressionStatement* parseExpressionStatement();
        BlockStatement* parseBlockStatement();
        Expression* parseExpression(int precedence);
        Expression* parseIdentifier();
        Expression* parseIntegerLiteral();
        Expression* parsePrefixExpression();
        Expression* parseInfixExpression(Expression* );
        Expression* parseBoolean();
        Expression* parseGroupedExpression();
        Expression* parseIfExpression();
        Expression* parseFunctionLiteral();
        Expression* parseStringLiteral();
        vector<Identifier*> parseParameters(); 
        Expression* parseCallExpression(Expression* );
        Expression* parseArrayLiteral();
        vector<Expression*> parseExpressionList(TokenType end);
        Expression* parseIndexExpression(Expression* );
        Expression* parseHashLiteral();

        void checkParserErrors();
        void peekError(TokenType ttype);
        void noPrefixParseFnError();
        void registerPrefix(TokenType ttype, Expression* (Parser::*fn) ());
        void registerInfix(TokenType ttype, Expression* (Parser::*fn) (Expression*));
};

#endif // PARSER_H
