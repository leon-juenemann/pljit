#pragma once
//---------------------------------------------------------------------------
#include "pljit/lexer/Token.hpp"
#include "pljit/code/Code.hpp"
#include "pljit/code/Reference.hpp"
#include <string_view>
#include <vector>
//---------------------------------------------------------------------------
namespace pljit::lexer {
//---------------------------------------------------------------------------
class Lexer {
    private:
    code::Code* m_src;
    std::string_view m_code;
    size_t m_index;

    public:
    explicit Lexer(code::Code* src);
    Token next();
    Token look();
    code::Code* source() const;
    private:
    void advance_whitespace();
    bool whitespace() const;
    bool keyword(std::string_view s) const;
    Token::Type token_type();
    Token buildToken(Token::Type type);
    Token buildLiteralToken();
    Token buildIdentifierToken();
    Token buildDeclaratorToken();

};
//---------------------------------------------------------------------------
} // pljit::lexer
//---------------------------------------------------------------------------