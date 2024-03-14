#include <gtest/gtest.h>
#include "lexer.hpp"
#include<string>

using namespace std;

class LexerTest : public ::testing::Test {
protected:
    void SetUp() override {
        input = "=+(){},;";
        lexer = Lexer(input);
    }

    Lexer lexer;
    string input;
};

TEST_F(LexerTest, spitToken) {
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

    for (const auto& expected : expectedTokens) {
        auto tok = lexer.spitToken();
        EXPECT_EQ(tok.type, expected.first); 
        EXPECT_EQ(tok.val, expected.second);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
