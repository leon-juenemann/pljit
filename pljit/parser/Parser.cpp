#include "Parser.hpp"
//---------------------------------------------------------------------------
using namespace std;
using namespace pljit::lexer;
using namespace pljit::code;
//---------------------------------------------------------------------------
namespace pljit::parser {
//---------------------------------------------------------------------------
Parser::Parser(lexer::Lexer* l) : m_lexer(l), m_error()
{

}
//---------------------------------------------------------------------------
std::unique_ptr<pt::ParseTree> Parser::parse()
{
    bool hasParams = false;
    bool hasVars = false;
    bool hasConsts = false;
    vector<unique_ptr<pt::PTNode>> nodes;
    Token next = m_lexer->look();
    if (next.view() == "PARAM")
    {
        unique_ptr<pt::ParamDecl> param_decl = paramDecl();
        if (!param_decl)
            return nullptr;
        nodes.push_back(move(param_decl));
        hasParams = true;
    }
    next = m_lexer->look();
    if (next.view() == "VAR")
    {
        unique_ptr<pt::VarDecl> var_decl = varDecl();
        if (!var_decl)
            return nullptr;
        nodes.push_back(move(var_decl));
        hasVars = true;
    }
    next = m_lexer->look();
    if (next.view() == "CONST")
    {
        unique_ptr<pt::ConstDecl> const_decl = constDecl();
        if (!const_decl)
            return nullptr;
        nodes.push_back(move(const_decl));
        hasConsts = true;
    }

    unique_ptr<pt::CompStmt> comp_stmt = compStmt();
    if (!comp_stmt)
        return nullptr;

    std::unique_ptr<pt::Generic> terminator = parseGeneric(".");
    if (!terminator)
        return nullptr;


    code::Range range(m_lexer->source(), 0, terminator->range().index());

    nodes.push_back(move(comp_stmt));
    nodes.push_back(move(terminator));

    return make_unique<pt::ParseTree>(range, move(nodes));

}
//---------------------------------------------------------------------------
std::unique_ptr<pt::Stmt> Parser::stmt()
{
    vector<unique_ptr<pt::PTNode>> nodes;
    Token next = m_lexer->look();

    if (next.view() == "RETURN")
    {

        unique_ptr<pt::Generic> ret = parseGeneric("RETURN");
        if (!ret)
            return nullptr;

        nodes.push_back(move(ret));

        unique_ptr<pt::AddExpr> add_expr = addExpr();
        if (!add_expr)
            return nullptr;

        nodes.push_back(move(add_expr));

        Range range = combinedRange(nodes);

        return make_unique<pt::Stmt>(range, move(nodes), false);

    }
    else
    {
        unique_ptr<pt::AssignExpr> assign_expr = assignExpr();
        if (!assign_expr)
            return nullptr;

        nodes.push_back(move(assign_expr));

        Range range = combinedRange(nodes);
        return make_unique<pt::Stmt>(range, move(nodes), true);
    }

}
//---------------------------------------------------------------------------
std::unique_ptr<pt::DeclList> Parser::declList()
{
    vector<unique_ptr<pt::PTNode>> nodes;

    unique_ptr<pt::Identifier> id = parseIdentifier();
    if (!id)
        return nullptr;

    nodes.push_back(move(id));


    while (m_lexer->look().view() == ",")
    {
        Token tmp = m_lexer->next();
        unique_ptr<pt::Generic> sep = make_unique<pt::Generic>(tmp.range());
        nodes.push_back(move(sep));

        unique_ptr<pt::Identifier> nextId = parseIdentifier();
        if (!nextId)
            return nullptr;

        nodes.push_back(move(nextId));

    }

    Range range = combinedRange(nodes);
    return make_unique<pt::DeclList>(range, move(nodes));
}
//---------------------------------------------------------------------------
std::unique_ptr<pt::InitDeclList> Parser::initDeclList()
{
    vector<unique_ptr<pt::PTNode>> nodes;
    unique_ptr<pt::InitDecl> init_decl = initDecl();
    if (!init_decl)
        return nullptr;
    nodes.push_back(move(init_decl));

    while (m_lexer->look().view() == ",")
    {
        Token tmp = m_lexer->next();
        unique_ptr<pt::Generic> sep = make_unique<pt::Generic>(tmp.range());
        nodes.push_back(move(sep));

        unique_ptr<pt::InitDecl> next = initDecl();
        if (!next)
            return nullptr;
        nodes.push_back(move(next));

    }

    Range range = combinedRange(nodes);
    return make_unique<pt::InitDeclList>(range, move(nodes));
}
//---------------------------------------------------------------------------
std::unique_ptr<pt::StmtList> Parser::stmtList()
{
    vector<unique_ptr<pt::PTNode>> nodes;
    unique_ptr<pt::Stmt> s = stmt();
    if (!s)
        return nullptr;
    nodes.push_back(move(s));

    while (m_lexer->look().view() == ";")
    {
        Token tmp = m_lexer->next();
        unique_ptr<pt::Generic> sep = make_unique<pt::Generic>(tmp.range());
        nodes.push_back(move(sep));

        unique_ptr<pt::Stmt> next = stmt();
        if (!next)
            return nullptr;
        nodes.push_back(move(next));

    }

    Range range = combinedRange(nodes);
    return make_unique<pt::StmtList>(range, move(nodes));
}
//---------------------------------------------------------------------------
std::unique_ptr<pt::ParamDecl> Parser::paramDecl()
{
    vector<unique_ptr<pt::PTNode>> nodes;

    unique_ptr<pt::Generic> param = parseGeneric("PARAM");
    if (!param)
        return nullptr;
    nodes.push_back(move(param));

    // declarator-list
    unique_ptr<pt::DeclList> decl_list = declList();
    if(!decl_list)
        return nullptr;

    nodes.push_back(move(decl_list));

    // ";"
    unique_ptr<pt::Generic> sep = parseGeneric(";");
    if(!sep)
        return nullptr;

    nodes.push_back(move(sep));

    Range range = combinedRange(nodes);
    return make_unique<pt::ParamDecl>(range, move(nodes));
}
//---------------------------------------------------------------------------
std::unique_ptr<pt::VarDecl> Parser::varDecl()
{
    vector<unique_ptr<pt::PTNode>> nodes;


    unique_ptr<pt::Generic> var = parseGeneric("VAR");
    if (!var)
        return nullptr;

    nodes.push_back(move(var));

    // declarator-list
    unique_ptr<pt::DeclList> decl_list = declList();
    if (!decl_list)
        return nullptr;

    nodes.push_back(move(decl_list));

    // ";"
    unique_ptr<pt::Generic> sep = parseGeneric(";");
    if (!sep)
        return nullptr;
    nodes.push_back(move(sep));

    Range range = combinedRange(nodes);
    return make_unique<pt::VarDecl>(range, move(nodes));
}
//---------------------------------------------------------------------------
std::unique_ptr<pt::ConstDecl> Parser::constDecl()
{
    vector<unique_ptr<pt::PTNode>> nodes;

    // "CONST"
    unique_ptr<pt::Generic> con = parseGeneric("CONST");
    if (!con)
        return nullptr;

    nodes.push_back(move(con));


    // declarator-list
    unique_ptr<pt::InitDeclList> init_decl_list = initDeclList();
    if (!init_decl_list)
        return nullptr;
    nodes.push_back(move(init_decl_list));

    // ";"
    unique_ptr<pt::Generic> sep = parseGeneric(";");
    if (!sep)
        return nullptr;

    nodes.push_back(move(sep));
    Range range = combinedRange(nodes);
    return make_unique<pt::ConstDecl>(range, move(nodes));
}
//---------------------------------------------------------------------------
std::unique_ptr<pt::InitDecl> Parser::initDecl()
{
    vector<unique_ptr<pt::PTNode>> nodes;

    unique_ptr<pt::Identifier> id = parseIdentifier();
    if (!id)
        return nullptr;

    nodes.push_back(move(id));

    unique_ptr<pt::Generic> equal = parseGeneric("=");
    if (!equal)
        return nullptr;
    nodes.push_back(move(equal));

    unique_ptr<pt::Literal> literal = parseLiteral();
    if (!literal)
        return nullptr;
    nodes.push_back(move(literal));


    Range range = combinedRange(nodes);
    return make_unique<pt::InitDecl>(range, move(nodes));
}
//---------------------------------------------------------------------------
std::unique_ptr<pt::CompStmt> Parser::compStmt()
{
    vector<unique_ptr<pt::PTNode>> nodes;

    unique_ptr<pt::Generic> begin = parseGeneric("BEGIN");
    if (!begin)
        return nullptr;
    nodes.push_back(move(begin));


    unique_ptr<pt::StmtList> stmt_list = stmtList();
    if (!stmt_list)
        return nullptr;
    nodes.push_back(move(stmt_list));


    unique_ptr<pt::Generic> end = parseGeneric("END");
    if (!end)
        return nullptr;
    nodes.push_back(move(end));

    Range range = combinedRange(nodes);
    return make_unique<pt::CompStmt>(range, move(nodes));
}
//---------------------------------------------------------------------------
std::unique_ptr<pt::AssignExpr> Parser::assignExpr()
{
    vector<unique_ptr<pt::PTNode>> nodes;


    unique_ptr<pt::Identifier> id = parseIdentifier();
    if (!id)
        return nullptr;
    nodes.push_back(move(id));

    unique_ptr<pt::Generic> assign = parseGeneric(":=");
    if (!assign)
        return nullptr;
    nodes.push_back(move(assign));

    unique_ptr<pt::AddExpr> add_expr = addExpr();
    if (!add_expr)
        return nullptr;
    nodes.push_back(move(add_expr));

    Range range = combinedRange(nodes);
    return make_unique<pt::AssignExpr>(range, move(nodes));
}
//---------------------------------------------------------------------------
std::unique_ptr<pt::AddExpr> Parser::addExpr()
{
    vector<unique_ptr<pt::PTNode>> nodes;
    unique_ptr<pt::MulExpr> mul_expr = mulExpr();
    if (!mul_expr)
        return nullptr;
    nodes.push_back(move(mul_expr));
    pt::AddExpr::Option option = pt::AddExpr::Option::no_option;

    if (m_lexer->look().range().view() == "+")
    {
        option = pt::AddExpr::Option::plus;
    }
    else if (m_lexer->look().range().view() == "-")
    {
        option = pt::AddExpr::Option::minus;
    }

    if (option != pt::AddExpr::Option::no_option)
    {
        Token next = m_lexer->next();
        unique_ptr<pt::Generic> add = make_unique<pt::Generic>(next.range());
        nodes.push_back(move(add));

        unique_ptr<pt::AddExpr> add_expr = addExpr();
        if (!add_expr)
            return nullptr;
        nodes.push_back(move(add_expr));

    }

    Range range = combinedRange(nodes);
    return make_unique<pt::AddExpr>(range, move(nodes), option);
}
//---------------------------------------------------------------------------
std::unique_ptr<pt::MulExpr> Parser::mulExpr()
{
    vector<unique_ptr<pt::PTNode>> nodes;
    unique_ptr<pt::UnExpr> un_expr = unExpr();
    if (!un_expr)
        return nullptr;
    nodes.push_back(move(un_expr));


    pt::MulExpr::Option option = pt::MulExpr::Option::no_option;
    Token look = m_lexer->look();

    if (look.view() == "*")
    {
        option = pt::MulExpr::Option::mul;
    }
    else if (look.view() == "/")
    {
        option = pt::MulExpr::Option::div;
    }

    if (option != pt::MulExpr::Option::no_option)
    {
        Token next = m_lexer->next();
        unique_ptr<pt::Generic> mul = make_unique<pt::Generic>(next.range());
        nodes.push_back(move(mul));

        unique_ptr<pt::MulExpr> mul_expr = mulExpr();
        if (!mul_expr)
            return nullptr;
        nodes.push_back(move(mul_expr));
    }

    Range range = combinedRange(nodes);
    return make_unique<pt::MulExpr>(range, move(nodes), option);
}
//---------------------------------------------------------------------------
std::unique_ptr<pt::UnExpr> Parser::unExpr()
{
    vector<unique_ptr<pt::PTNode>> nodes;


    pt::UnExpr::Option option = pt::UnExpr::Option::no_option;
    Token look = m_lexer->look();

    if (look.view() == "+")
    {
        option = pt::UnExpr::Option::plus;
    }
    else if (look.view() == "-")
    {
        option = pt::UnExpr::Option::minus;
    }

    if (option != pt::UnExpr::Option::no_option)
    {
        Token next = m_lexer->next();
        unique_ptr<pt::Generic> plus = make_unique<pt::Generic>(next.range());
        nodes.push_back(move(plus));

    }

    unique_ptr<pt::PrimExpr> prim_expr = primExpr();
    if (!prim_expr)
        return nullptr;
    nodes.push_back(move(prim_expr));

    Range range = combinedRange(nodes);
    return make_unique<pt::UnExpr>(range, move(nodes), option);
}
//---------------------------------------------------------------------------
std::unique_ptr<pt::PrimExpr> Parser::primExpr()
{
    vector<unique_ptr<pt::PTNode>> nodes;
    Token look = m_lexer->look();
    pt::PrimExpr::Option option = pt::PrimExpr::Option::additive;
    if (look.type() == Token::Type::identifier)
    {
        option = pt::PrimExpr::Option::identifier;
        unique_ptr<pt::Identifier> id = parseIdentifier();
        if (!id)
            return nullptr;
        nodes.push_back(move(id));
    }
    else if (look.type() == Token::Type::literal)
    {
        option = pt::PrimExpr::Option::literal;
        unique_ptr<pt::Identifier> lit = parseIdentifier();
        if (!lit)
            return nullptr;
        nodes.push_back(move(lit));
    } else
    {
        option = pt::PrimExpr::Option::additive;
        unique_ptr<pt::Generic> bracket_open = parseGeneric("(");
        if (!bracket_open)
            return nullptr;
        nodes.push_back(move(bracket_open));

        unique_ptr<pt::AddExpr> add_expr = addExpr();
        if (!add_expr)
            return nullptr;
        nodes.push_back(move(add_expr));

        unique_ptr<pt::Generic> bracket_close = parseGeneric(")");
        if (!bracket_close)
            return nullptr;

        nodes.push_back(move(bracket_open));
    }

    Range range = combinedRange(nodes);
    return make_unique<pt::PrimExpr>(range, move(nodes), option);

}
//---------------------------------------------------------------------------
std::unique_ptr<pt::Identifier> Parser::parseIdentifier()
{
    Token token = m_lexer->next();
    if (token.type() == Token::Type::identifier)
    {
        return make_unique<pt::Identifier>(token.range());
    }
    else
    {
        m_error = Error(token.range(), "expected identifier");
        return nullptr;
    }
}
//---------------------------------------------------------------------------
std::unique_ptr<pt::Generic> Parser::parseGeneric(const string& expected)
{
    Token token = m_lexer->next();
    if (token.view() == expected)
    {
        return make_unique<pt::Generic>(token.range());
    }
    else
    {
        m_error = Error(token.range(), "expected \"" + expected + "\"");
        return nullptr;
    }
}
//---------------------------------------------------------------------------
std::unique_ptr<pt::Literal> Parser::parseLiteral()
{
    Token token = m_lexer->next();
    if (token.type() == Token::Type::literal)
    {
        m_lexer->look();
        return make_unique<pt::Literal>(token.range());
    }
    else
    {
        m_error = Error(token.range(), "expected literal");
        return nullptr;
    }
}
//---------------------------------------------------------------------------
code::Range Parser::combinedRange
(const vector<unique_ptr<pt::PTNode>>& nodes) const
{

    if (!nodes.empty())
    {
        size_t startIndex = nodes[0]->range().index();
        size_t endIndex = nodes[nodes.size()-1]->range().index();
        size_t length = endIndex - startIndex + 1;
        return  code::Range(m_lexer->source(), startIndex, length);
    }

    return code::Range(m_lexer->source(), 0, 0);

}
//---------------------------------------------------------------------------
Error Parser::error()
{
    return m_error;
}
//---------------------------------------------------------------------------
} // pljit::parser
//---------------------------------------------------------------------------