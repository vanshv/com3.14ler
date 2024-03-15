#ifndef PARSER_H
#define PARSER_H

#include "lexer.hpp"
#include "ast.hpp"

class Parser{
    public:
        Lexer* l;
        Token currToken;
        Token peekToken;

        Parser(Lexer* l);
        void eatToken();
        Program* parseProgram();

};

#endif // PARSER_H
