#include "pljit/lexer/Lexer.hpp"
#include "pljit/lexer/Token.hpp"
#include "pljit/code/Code.hpp"
#include <cmath>
#include <utility>
#include <gtest/gtest.h>
//---------------------------------------------------------------------------
using namespace pljit::lexer;
using namespace pljit::code;
using namespace std;
//---------------------------------------------------------------------------
TEST(TestLexer, keywordToken) {
    string input = "PARAM VAR CONST BEGIN END RETURN";
    Code code(input);
    Lexer l(&code);
    
    Token t = l.next();
    while (t.type() != Token::Type::eof) {
        EXPECT_EQ(t.type(), Token::Type::keyword);
        t = l.next();
    }
}
TEST(TestLexer, program) {
    string input =
            "PARAM width, height, depth;\n"
            "VAR volume;\n"
            "CONST density = 2400;\n"
            "BEGIN\n"
            "volume := width * height * depth;\n"
            "RETURN density * volume\n"
            "END.";
    
    vector<Token::Type> expected = {
            Token::Type::keyword,
            Token::Type::identifier,
            Token::Type::separator,
            Token::Type::identifier,
            Token::Type::separator,
            Token::Type::identifier,
            Token::Type::separator,
            Token::Type::keyword,
            Token::Type::identifier,
            Token::Type::separator,
            Token::Type::keyword,
            Token::Type::identifier,
            Token::Type::assignment,
            Token::Type::literal,
            Token::Type::separator,
            Token::Type::keyword,
            Token::Type::identifier,
            Token::Type::declarator,
            Token::Type::identifier,
            Token::Type::mul_op,
            Token::Type::identifier,
            Token::Type::mul_op,
            Token::Type::identifier,
            Token::Type::separator,
            Token::Type::keyword,
            Token::Type::identifier,
            Token::Type::mul_op,
            Token::Type::identifier,
            Token::Type::keyword,
            Token::Type::terminator
    };
    vector<Token> tokens;
    Code code(input);
    Lexer l(&code);
    Token t = l.next();
    while (t.type() != Token::Type::eof) {
        tokens.push_back(t);
        t = l.next();
    }
    EXPECT_EQ(tokens.size(), expected.size());


    for (size_t i = 0; i < tokens.size(); i++) {
        EXPECT_EQ(tokens[i].type(), expected[i]);
    }

}
