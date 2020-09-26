#pragma once
//---------------------------------------------------------------------------
#include <pljit/code/Code.hpp>
#include <pljit/code/Reference.hpp>
//---------------------------------------------------------------------------
namespace pljit::lexer {
//---------------------------------------------------------------------------
class Token {
    public:
    enum class Type {
        keyword,
        identifier,
        literal,
        declarator,
        assignment,
        separator,
        bracket_open,
        bracket_close,
        plus_op,
        minus_op,
        mul_op,
        div_op,
        terminator,
        eof,
        undefined
    };

    private:
    code::Range m_source;
    Type m_type;

    public:
    Token();
    Token (code::Range source, Type type);
    Type type() const;
    std::string_view view() const;
    code::Range range() const;
};
//---------------------------------------------------------------------------
} // namespace pljit::lexer
//---------------------------------------------------------------------------
