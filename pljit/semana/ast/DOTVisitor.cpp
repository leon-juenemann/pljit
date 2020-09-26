#include "pljit/semana/ast/DOTVisitor.hpp"
#include <vector>
#include <iostream>
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace pljit::semana::ast {
//---------------------------------------------------------------------------
DOTVisitor::DOTVisitor() : m_id(0)
{

}
//---------------------------------------------------------------------------
void DOTVisitor::visit(const AST& guest)
{
    size_t myId = m_id;
    string label = "Function";


    cout << "digraph {" << endl;


    cout << myId << " [label=\"" << label << "\"];" << endl;

    std::vector<std::unique_ptr<Statement>> const& nodes = guest.getStmts();
    for(auto& n : nodes)
    {
        size_t curId = m_id;
        m_id++;
        n->accept(*this);
        cout << myId << " -> " << curId + 1 << endl;
    }

    cout << "}" << endl;
}
//---------------------------------------------------------------------------
void DOTVisitor::visit(const Identifier& guest)
{
    cout << m_id << " [label=\"" << guest.name() << "\"];" << endl;
}
//---------------------------------------------------------------------------
void DOTVisitor::visit(const Literal& guest)
{
    cout << m_id << " [label=\"" << guest.value() << "\"];" << endl;
}
//---------------------------------------------------------------------------
void DOTVisitor::visit(const Unary& guest)
{
    string label;
    size_t myId = m_id;
    if (guest.op() == Unary::Operator::Plus)
    {
        label = "+";
    }
    else
    {
        label = "-";
    }
    cout << myId << " [label=\"" << label << "\"];" << endl;
    m_id++;
    cout << myId << " -> " << m_id << endl;

    guest.getChild().accept(*this);
}
//---------------------------------------------------------------------------
void DOTVisitor::visit(const Binary& guest)
{
    string label;
    size_t myId = m_id;
    if (guest.op() == Binary::Operator::Plus)
    {
        label = "+";
    }
    else if (guest.op() == Binary::Operator::Minus)
    {
        label = "-";
    }
    else if (guest.op() == Binary::Operator::Mul)
    {
        label = "*";
    }
    else
    {
        label = "/";
    }
    cout << myId << " [label=\"" << label << "\"];" << endl;
    m_id++;
    cout << myId << " -> " << m_id << endl;
    guest.getLeft().accept(*this);
    m_id++;
    cout << myId << " -> " << m_id << endl;
    guest.getRight().accept(*this);

}
//---------------------------------------------------------------------------
void DOTVisitor::visit(const AssignStmt& guest)
{
    string label = ":=";
    size_t myId = m_id;

    cout << myId << " [label=\"" << label << "\"];" << endl;
    m_id++;
    cout << myId << " -> " << m_id << endl;
    guest.getIdentifier().accept(*this);
    m_id++;
    cout << myId << " -> " << m_id << endl;
    guest.getExpression().accept(*this);
}
//---------------------------------------------------------------------------
void DOTVisitor::visit(const RetStmt& guest)
{
    string label = "RETURN";
    size_t myId = m_id;

    cout << myId << " [label=\"" << label << "\"];" << endl;
    m_id++;
    cout << myId << " -> " << m_id << endl;
    guest.getExpression().accept(*this);

}
//---------------------------------------------------------------------------
} // pljit::semana::ast
//---------------------------------------------------------------------------