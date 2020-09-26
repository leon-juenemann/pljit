#include <utility>
#include <cassert>
#include "ParseTree.hpp"

using namespace pljit::code;
using namespace std;


namespace pljit::parser::pt {
// --------------------------------------------------------------------------
PTNode::PTNode(PTNode::Type type, Range code) : m_type(type), m_code(code)
{

}
// --------------------------------------------------------------------------
PTNode::Type PTNode::type() const
{
    return m_type;
}

Range PTNode::range() const
{
    return m_code;
}

std::string_view PTNode::code() const
{
    return m_code.view();
}

Nonterminal::Nonterminal(Range code, Type t, vector<unique_ptr<PTNode>> n)
        : PTNode(t, code), m_children(move(n))
{

}

vector<unique_ptr<PTNode>> const& Nonterminal::getChildren() const
{
    return m_children;
}

// --------------------------------------------------------------------------
Generic::Generic(Range code) : PTNode(Type::generic, code)
{

}
// --------------------------------------------------------------------------
void Generic::accept(PTVisitor& visitor) const
{
    visitor.visit(*this);
}

// --------------------------------------------------------------------------
Literal::Literal(Range code) : PTNode(Type::literal, code)
{

}
// --------------------------------------------------------------------------
void Literal::accept(PTVisitor& visitor) const
{
    visitor.visit(*this);
}
// --------------------------------------------------------------------------
uint64_t Literal::value() const
{
    return stoi(m_code.view().data());
}
// --------------------------------------------------------------------------
Identifier::Identifier(Range code)
: PTNode(Type::identifier, code), m_name(m_code.view())
{

}
// --------------------------------------------------------------------------
void Identifier::accept(PTVisitor& visitor) const
{
    visitor.visit(*this);
}
// --------------------------------------------------------------------------
std::string Identifier::name() const
{
    return m_name;
}
// --------------------------------------------------------------------------
ParseTree::ParseTree(Range code, vector<unique_ptr<PTNode>> nodes)
 : Nonterminal(code, Type::fun_def, move(nodes)),
 m_hasParams(false), m_hasVars(false), m_hasConsts(false)
{
    for (auto& n : m_children)
    {
        if (n->type() == Type::param_decl)
        {
            m_hasParams = true;
        }
        else if (n->type() == Type::var_decl)
        {
            m_hasVars = true;
        }
        else if (n->type() == Type::const_decl)
        {
            m_hasConsts = true;
        }
    }
}
// --------------------------------------------------------------------------
void ParseTree::accept(PTVisitor& visitor) const
{
    visitor.visit(*this);
}
// --------------------------------------------------------------------------
bool ParseTree::hasParameters()
{
    return m_hasConsts;
}
// --------------------------------------------------------------------------
bool ParseTree::hasVariables()
{
    return m_hasVars;
}
// --------------------------------------------------------------------------
bool ParseTree::hasConstants()
{
    return m_hasConsts;
}
// --------------------------------------------------------------------------
const ParamDecl& ParseTree::getParamDecl() const
{
    assert(m_hasParams);

    return *static_cast<ParamDecl*>(m_children[0].get());
}
// --------------------------------------------------------------------------
const VarDecl& ParseTree::getVarDecl() const
{
    assert(m_hasVars);
    size_t pos = 0;
    if (m_hasParams)
        ++pos;
    return *static_cast<VarDecl*>(m_children[pos].get());
}
// --------------------------------------------------------------------------
const ConstDecl& ParseTree::getConstDecl() const
{
    assert(m_hasConsts);
    size_t pos = 0;
    if (m_hasParams)
        ++pos;
    if (m_hasVars)
        ++pos;

    return *static_cast<ConstDecl*>(m_children[pos].get());
}
// --------------------------------------------------------------------------
const CompStmt& ParseTree::getCompStmt() const
{
    size_t pos = 0;
    if (m_hasParams)
        ++pos;
    if (m_hasVars)
        ++pos;
    if (m_hasConsts)
        ++pos;

    assert(m_children[pos].get()->type() == Type::comp_stmt);

    return *static_cast<CompStmt*>(m_children[pos].get());
}
// --------------------------------------------------------------------------
ParamDecl::ParamDecl(Range code, vector<unique_ptr<PTNode>> n)
 : Nonterminal(code, Type::param_decl, move(n))
{

}
// --------------------------------------------------------------------------
void ParamDecl::accept(PTVisitor& visitor) const
{
    visitor.visit(*this);
}
// --------------------------------------------------------------------------
const DeclList& ParamDecl::getDeclList() const
{
    assert(m_children[1]->type() == Type::decl_list);
    return *static_cast<DeclList*>(m_children[1].get());
}
// --------------------------------------------------------------------------
VarDecl::VarDecl(Range code, vector<unique_ptr<PTNode>> n)
: Nonterminal(code, Type::var_decl, move(n))
{

}
// --------------------------------------------------------------------------
void VarDecl::accept(PTVisitor& visitor) const
{
    visitor.visit(*this);
}
// --------------------------------------------------------------------------
const DeclList& VarDecl::getDeclList() const
{
    assert(m_children[1]->type() == Type::decl_list);
    return *static_cast<DeclList*>(m_children[1].get());

}
// --------------------------------------------------------------------------
ConstDecl::ConstDecl(Range code, vector<unique_ptr<PTNode>> n)
: Nonterminal(code, Type::const_decl, move(n))
{

}
// --------------------------------------------------------------------------
void ConstDecl::accept(PTVisitor& visitor) const
{
    visitor.visit(*this);
}
// --------------------------------------------------------------------------
const InitDeclList& ConstDecl::getInitDeclList() const
{
    assert(m_children[1]->type() == Type::init_decl_list);
    return *static_cast<InitDeclList*>(m_children[1].get());
}
// --------------------------------------------------------------------------
DeclList::DeclList(Range code, vector<unique_ptr<PTNode>> n)
: Nonterminal(code, Type::decl_list, move(n))
{

}
// --------------------------------------------------------------------------
void DeclList::accept(PTVisitor& visitor) const
{
    visitor.visit(*this);
}
// --------------------------------------------------------------------------
InitDeclList::InitDeclList(Range code, vector<unique_ptr<PTNode>> n)
: Nonterminal(code, Type::init_decl_list, move(n))
{

}
// --------------------------------------------------------------------------
void InitDeclList::accept(PTVisitor& visitor) const
{
    visitor.visit(*this);
}
// --------------------------------------------------------------------------
InitDecl::InitDecl(Range code, vector<unique_ptr<PTNode>> n)
: Nonterminal(code, Type::init_decl, move(n))
{

}
// --------------------------------------------------------------------------
const Literal& InitDecl::getLiteral() const
{
    assert(m_children[2]->type() == Type::literal);
    return *static_cast<Literal*>(m_children[2].get());
}
// --------------------------------------------------------------------------
const Identifier& InitDecl::getIdentifier() const
{
    assert(m_children[0]->type() == Type::identifier);
    return *static_cast<Identifier*>(m_children[0].get());
}
// --------------------------------------------------------------------------
void InitDecl::accept(PTVisitor& visitor) const
{
    visitor.visit(*this);
}
// --------------------------------------------------------------------------
CompStmt::CompStmt(Range code, vector<unique_ptr<PTNode>> n)
        : Nonterminal(code, Type::comp_stmt, move(n))
{

}
// --------------------------------------------------------------------------
void CompStmt::accept(PTVisitor& visitor) const
{
    visitor.visit(*this);
}
// --------------------------------------------------------------------------
const StmtList& CompStmt::getStmtList() const
{
    assert(m_children[1]->type() == Type::stmt_list);
    return *static_cast<StmtList*>(m_children[1].get());
}
// --------------------------------------------------------------------------
StmtList::StmtList(Range code, vector<unique_ptr<PTNode>> n)
        : Nonterminal(code, Type::stmt_list, move(n))
{

}
// --------------------------------------------------------------------------
void StmtList::accept(PTVisitor& visitor) const
{
    visitor.visit(*this);
}
// --------------------------------------------------------------------------
Stmt::Stmt(Range code, vector<unique_ptr<PTNode>> n, bool assign)
        : Nonterminal(code, Type::stmt, move(n)), m_isAssign(assign)
{

}
// --------------------------------------------------------------------------
void Stmt::accept(PTVisitor& visitor) const
{
    visitor.visit(*this);
}
// --------------------------------------------------------------------------
bool Stmt::isAssign() const
{
    return m_isAssign;
}
// --------------------------------------------------------------------------
AssignExpr::AssignExpr(Range code, vector<unique_ptr<PTNode>> n)
: Nonterminal(code, Type::assign_expr, move(n))
{

}
// --------------------------------------------------------------------------
void AssignExpr::accept(PTVisitor& visitor) const
{
    visitor.visit(*this);
}
// --------------------------------------------------------------------------
AddExpr::AddExpr(Range code, vector<unique_ptr<PTNode>> n, Option o)
: Nonterminal(code, Type::add_expr, move(n)), m_option(o)
{

}
// --------------------------------------------------------------------------
void AddExpr::accept(PTVisitor& visitor) const
{
    visitor.visit(*this);
}
// --------------------------------------------------------------------------
AddExpr::Option AddExpr::option() const
{
    return m_option;
}
// --------------------------------------------------------------------------
MulExpr::MulExpr(Range code, vector<unique_ptr<PTNode>> n, Option o)
        : Nonterminal(code, Type::mul_expr, move(n)), m_option(o)
{

}
// --------------------------------------------------------------------------
void MulExpr::accept(PTVisitor& visitor) const
{
    visitor.visit(*this);
}
// --------------------------------------------------------------------------
MulExpr::Option MulExpr::option() const
{
    return m_option;
}
// --------------------------------------------------------------------------
UnExpr::UnExpr(Range code, vector<unique_ptr<PTNode>> n, Option o)
        : Nonterminal(code, Type::un_expr, move(n)), m_option(o)
{

}
// --------------------------------------------------------------------------
void UnExpr::accept(PTVisitor& visitor) const
{
    visitor.visit(*this);
}
// --------------------------------------------------------------------------
UnExpr::Option UnExpr::option() const
{
    return m_option;
}
// --------------------------------------------------------------------------
PrimExpr::PrimExpr(Range code, vector<unique_ptr<PTNode>> n, Option o)
: Nonterminal(code, Type::prim_expr, move(n)), m_option(o)
{

}
// --------------------------------------------------------------------------
void PrimExpr::accept(PTVisitor& visitor) const
{
    visitor.visit(*this);
}
// --------------------------------------------------------------------------
PrimExpr::Option PrimExpr::option() const
{
    return m_option;
}
// --------------------------------------------------------------------------
} // pljit::parser::pt
// --------------------------------------------------------------------------