#include "Lexer.hpp"
#include <cctype>
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace pljit::lexer {
// --------------------------------------------------------------------------
Lexer::Lexer(code::Code* src) : m_src(src), m_code(src->view()), m_index(0)
{

}
// --------------------------------------------------------------------------
Token Lexer::next()
{
    advance_whitespace();

    if (m_index >= m_code.length())
    {
        code::Range eof_range(m_src, m_code.length() - 1, 0);
        return Token(eof_range, Token::Type::eof);
    }

    Token::Type type = token_type();

    Token token = buildToken(type);

    if (token.type() == Token::Type::undefined)
    {
        // stop compilation?
    }

    return token;
}
// --------------------------------------------------------------------------
Token Lexer::look()
{
    size_t tmp = m_index;
    Token t = next();
    m_index = tmp;
    return t;
}
// --------------------------------------------------------------------------
code::Code* Lexer::source() const
{
    return m_src;
}
// --------------------------------------------------------------------------
void Lexer::advance_whitespace()
{
    while (m_index < m_code.size() && whitespace())
    {
        m_index++;
    }
}
// --------------------------------------------------------------------------
bool Lexer::whitespace() const
{
    char c = m_code.at(m_index);
    return c == ' ' || c == '\n' || c == '\t';
}
// --------------------------------------------------------------------------
Token::Type Lexer::token_type()
{
    char c = m_code.at(m_index);

    if (isdigit(c))
        return Token::Type::literal;
    if (isalpha(c))
        return Token::Type::identifier;

    switch (c)
    {
        case '+':
            return Token::Type::plus_op;
        case '-':
            return Token::Type::minus_op;
        case '*':
            return Token::Type::mul_op;
        case '/':
            return Token::Type::div_op;
        case ':':
            return Token::Type::declarator;
        case '=':
            return Token::Type::assignment;
        case '(':
            return Token::Type::bracket_open;
        case ')':
            return Token::Type::bracket_close;
        case ',':
            return Token::Type::separator;
        case ';':
            return Token::Type::separator;
        case '.':
            return Token::Type::terminator;
        default:
            return Token::Type::undefined;
    }
}
// --------------------------------------------------------------------------
Token Lexer::buildToken(Token::Type type)
{
    switch (type)
    {
        case Token::Type::literal:
            return buildLiteralToken();
        case Token::Type::identifier:
            return buildIdentifierToken();
        case Token::Type::declarator:
            return buildDeclaratorToken();
        default:
            code::Range ref{m_src, m_index, 1};
            m_index++;
            return Token(ref, type);
    }
}
// --------------------------------------------------------------------------
Token Lexer::buildIdentifierToken()
{
    size_t start_index = m_index;
    size_t length = 0;
    Token::Type type = Token::Type::identifier;

    while (m_index < m_code.size())
    {
        char curr = m_code.at(m_index);
        if (!isalpha(curr) || whitespace())
        {
            break;
        }
        m_index++;
        length++;
    }

    if (keyword(m_code.substr(start_index, length)))
        type = Token::Type::keyword;

    return Token{code::Range(m_src, start_index, length), type};
}
// --------------------------------------------------------------------------
bool Lexer::keyword(string_view s) const
{
    return s == "PARAM" || s == "VAR" || s == "CONST" ||
           s == "BEGIN" || s == "END" || s == "RETURN";
}
// --------------------------------------------------------------------------
Token Lexer::buildLiteralToken()
{
    size_t start_index = m_index;
    size_t length = 0;
    Token::Type type = Token::Type::literal;
    while (m_index < m_code.size())
    {
        char curr = m_code.at(m_index);
        if (!isdigit(curr) || whitespace())
        {
            break;
        }
        m_index++;
        length++;
    }
    return Token{code::Range(m_src, start_index, length), type};
}
// --------------------------------------------------------------------------
Token Lexer::buildDeclaratorToken()
{
    size_t start_index = m_index;
    code::Range range{m_src, start_index, 2};
    if (m_code.at(m_index) == ':')
        m_index++;
    if (m_code.at(m_index) == '=')
    {
        m_index++;
        return Token{range, Token::Type::declarator};
    }
    return Token{range, Token::Type::undefined};
}
// --------------------------------------------------------------------------
} // pljit::lexer
//---------------------------------------------------------------------------
