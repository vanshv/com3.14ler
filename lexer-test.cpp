#include <gtest/gtest.h>
#include "lexer.hpp"
#include<string>

using namespace std;

// Define a test case
TEST(LexerTest, NextToken) {
    string input = "=+(){},;";
    Lexer lexer(input);

    vector<pair<TokenType, string>> expectedTokens = {
        {ASSIGN, "="},
        {PLUS, "+"},
        {LPAREN, "("},
        {RPAREN, ")"},
        {LBRACE, "{"},
        {RBRACE, "}"},
        {COMMA, ","},
        {SEMICOLON, ";"},
        {MYEOF, ""}
    };

    int i = 0;
    while (true) {
        Token tok = lexer.spitToken();
        if (tok.type == MYEOF) {
            break;
        }
        EXPECT_EQ(tok.type, expectedTokens[i].first);
        EXPECT_EQ(tok.val, expectedTokens[i].second);
        i++;
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
