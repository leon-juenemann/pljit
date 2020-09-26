#include "Semana.hpp"
#include <cassert>
//---------------------------------------------------------------------------
using namespace std;
using namespace pljit::parser;
using namespace pljit::semana::ast;
//---------------------------------------------------------------------------
namespace pljit::semana {
//---------------------------------------------------------------------------
Semana::Semana() : m_symbolTable(), m_error(nullptr), m_hasReturn(false)
{

}
//---------------------------------------------------------------------------
unique_ptr<AST> Semana::analyse(unique_ptr<pt::ParseTree>& pt)
{
    if (!pt)
        return nullptr;

    if (pt->hasParameters())
    {
        const auto& paramDecl = pt->getParamDecl();
        bool error = analyseParamDecl(paramDecl);
        if (error)
            return nullptr;
    }

    if (pt->hasVariables())
    {
        const auto& varDecl = pt->getVarDecl();

        bool error = analyseVarDecl(varDecl);
        if (error)
            return nullptr;
    }

    if (pt->hasConstants())
    {
        const auto& constDecl = pt->getConstDecl();

        bool error = analyseConstDecl(constDecl);
        if (error)
            return nullptr;
    }

    vector<unique_ptr<Statement>> nodes;

    // for each statement in ParseTree (between  BEGIN and END)
    const pt::StmtList& stmtList = pt->getCompStmt().getStmtList();
    for (const auto& n : stmtList.getChildren())
    {
        // if n is separator -> skip
        if (n->type() == pt::ParseTree::Type::generic)
            continue;

        assert(n->type() == pt::ParseTree::Type::stmt);

        unique_ptr<ast::Statement> stmt = analyseStmt(n);
        if (!stmt)
            return nullptr;

        nodes.push_back(move(stmt));
    }

    if (!m_hasReturn)
    {
        m_error = make_unique<MissingRetStmt>
                (stmtList.getChildren().back().get()->range());

        return nullptr;
    }

    return make_unique<AST>(move(nodes),  m_symbolTable.variableState());
}
//---------------------------------------------------------------------------
bool Semana::analyseParamDecl(const pt::ParamDecl& paramDecl)
{
    const auto& declList = paramDecl.getDeclList();

    for (const auto& p : declList.getChildren())
    {
        if (p->code() == ",")
            continue;

        Symbol s(Symbol::Type::parameter, p->range(), 0);

        pair<bool, Symbol> insertInfo = m_symbolTable.insert(s);

        if (!insertInfo.first)
        {

            m_error = make_unique<IdRedeclaration>(p->range(),
                    insertInfo.second.range());

            return true;
        }
    }
    return false;
}
//---------------------------------------------------------------------------
bool Semana::analyseVarDecl(const pt::VarDecl& varDecl)
{
    const auto& declList = varDecl.getDeclList();
    for (const auto& v : declList.getChildren())
    {
        if (v->code() == ",")
            continue;

        Symbol s(Symbol::Type::variable, v->range(), 0);

        pair<bool, Symbol> insertInfo = m_symbolTable.insert(s);

        if (!insertInfo.first)
        {
            m_error = make_unique<IdRedeclaration>(v->range(),
                    insertInfo.second.range());
            return true;
        }

    }
    return false;
}
//---------------------------------------------------------------------------
bool Semana::analyseConstDecl(const pt::ConstDecl& constDecl)
{
    const auto& initDeclList = constDecl.getInitDeclList();
    for (const auto& decl : initDeclList.getChildren())
    {
        if (decl->code() == ",")
            continue;


        assert(decl->type() == pt::PTNode::Type::init_decl);


        const auto& initDecl = *static_cast<pt::InitDecl*>(decl.get());

        code::Range r = initDecl.getIdentifier().range();
        Symbol s(Symbol::Type::constant, r, initDecl.getLiteral().value());
        pair<bool, Symbol> insertInfo = m_symbolTable.insert(s);

        if (!insertInfo.first)
        {
            m_error = make_unique<IdRedeclaration>(decl->range(), insertInfo.second.range());
            return true;
        }

    }
    return false;
}
//---------------------------------------------------------------------------
std::unique_ptr<ast::Statement> Semana::analyseStmt
(const unique_ptr<pt::PTNode>& n)
{
    const auto& stmt = *static_cast<pt::Stmt*>(n.get());

    if (stmt.isAssign())
    {
        const auto& assignExpr = *static_cast<pt::AssignExpr*>(stmt.getChildren()[0].get());

        unique_ptr<Identifier> id = analyseIdentifier(assignExpr.getChildren()[0], true);
        if (!id)
            return nullptr;

        unique_ptr<Expression> expr = analyseExpr(assignExpr.getChildren()[2]);
        if (!expr)
            return nullptr;

        return make_unique<ast::AssignStmt>(move(id), move(expr));
    }
    else
    {
        // it's a return stmt
        unique_ptr<ast::Expression> expr = analyseExpr(stmt.getChildren()[1]);

        if (!expr)
            return nullptr;

        m_hasReturn = true;
        return make_unique<ast::RetStmt>(move(expr));
    }
}
//---------------------------------------------------------------------------
std::unique_ptr<ast::Expression> Semana::analyseExpr
(const std::unique_ptr<parser::pt::PTNode>& n)
{
    if (n->type() == pt::ParseTree::Type::add_expr)
        return analyseAdd(n);
    if (n->type() == pt::ParseTree::Type::mul_expr)
        return analyseMul(n);
    if (n->type() == pt::ParseTree::Type::un_expr)
        return analyseUnary(n);
    if (n->type() == pt::ParseTree::Type::prim_expr)
        return analysePrim(n);


    return analyseAdd(n);
}
//---------------------------------------------------------------------------
std::unique_ptr<ast::Expression> Semana::analyseAdd
(const std::unique_ptr<parser::pt::PTNode>& n)
{
    const auto& add = *static_cast<pt::AddExpr*>(n.get());

    unique_ptr<ast::Expression> left = analyseMul(add.getChildren()[0]);

    if (!left)
        return nullptr;

    if (add.option() == pt::AddExpr::Option::no_option)
    {
        return left;
    }
    unique_ptr<ast::Expression> right = analyseMul(n);

    if (!right)
        return nullptr;

    Binary::Operator op;
    if (add.option() == pt::AddExpr::Option::plus)
        op = Binary::Operator::Plus;
    else
        op = Binary::Operator::Minus;

    return make_unique<Binary>(op, move(left), move(right));

}
//---------------------------------------------------------------------------
std::unique_ptr<ast::Expression> Semana::analyseMul
(const std::unique_ptr<parser::pt::PTNode>& n)
{
    const auto& mul = *static_cast<pt::MulExpr*>(n.get());

    unique_ptr<ast::Expression> left = analyseUnary(mul.getChildren()[0]);

    if (!left)
        return nullptr;

    if (mul.option() == pt::MulExpr::Option::no_option)
    {
        return left;
    }
    unique_ptr<ast::Expression> right = analyseMul(mul.getChildren()[2]);

    if (!right)
        return nullptr;

    Binary::Operator op;
    if (mul.option() == pt::MulExpr::Option::mul)
        op = Binary::Operator::Mul;
    else
        op = Binary::Operator::Div;

    return make_unique<Binary>(op, move(left), move(right));
}
//---------------------------------------------------------------------------
unique_ptr<ast::Expression> Semana::analyseUnary(const unique_ptr<pt::PTNode>& n)
{
    const auto& unary = *static_cast<pt::UnExpr*>(n.get());

    if (unary.option() == pt::UnExpr::Option::no_option)
    {
        return analysePrim(unary.getChildren()[0]);
    }

    unique_ptr<ast::Expression> prim = analysePrim(unary.getChildren()[1]);

    if (!prim)
        return nullptr;

    Unary::Operator op;

    if (unary.option() == pt::UnExpr::Option::plus)
        op = Unary::Operator::Plus;
    else
        op = Unary::Operator::Minus;

    return make_unique<Unary>(op, move(prim));
}
//---------------------------------------------------------------------------
unique_ptr<ast::Expression> Semana::analysePrim(const unique_ptr<pt::PTNode>& n)
{
    const auto& prim = *static_cast<pt::PrimExpr*>(n.get());

    if (prim.option() == pt::PrimExpr::Option::identifier)
    {
        return analyseIdentifier(prim.getChildren()[0], false);
    }

    if (prim.option() == pt::PrimExpr::Option::literal)
    {
        const auto& literal = *static_cast<pt::Literal*>(prim.getChildren()[0].get());
        return make_unique<Literal>(literal.value());
    }


    return analyseAdd(prim.getChildren()[1]);
}
//---------------------------------------------------------------------------
unique_ptr<ast::Identifier> Semana::analyseIdentifier
(const unique_ptr<pt::PTNode>& n, bool left)
{
    const auto& id = *static_cast<pt::Identifier*>(n.get());
    Symbol* s = m_symbolTable.find(id.name());
    if (s == nullptr)
    {
        m_error = make_unique<UndeclaredId>(id.range());
        return nullptr;
    }
    if (left)
    {
        if (s->type() == Symbol::Type::variable)
        {
            s->setToInitialized();
        }
        if (s->type() == Symbol::Type::constant)
        {
            m_error = make_unique<AssignToConst>(id.range(), s->range());
            return nullptr;
        }
    }
    else
    {
        if (!s->initialized())
        {
            m_error = make_unique<UninitializedVar>(id.range(), s->range());
            return nullptr;
        }
    }
    return make_unique<Identifier>(id.code());
}
//---------------------------------------------------------------------------
const Error& Semana::error() const
{
    return *m_error;
}
//---------------------------------------------------------------------------
} // pljit::semana
//---------------------------------------------------------------------------