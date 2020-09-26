#include "pljit/parser/pt/DOTVisitor.hpp"
#include <iostream>
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace pljit::parser::pt {
//---------------------------------------------------------------------------
void DOTVisitor::visit(const Generic& guest)
{
    cout << id << " [label=\"" << guest.code() << "\"];" << endl;

}
//---------------------------------------------------------------------------
void pt::DOTVisitor::visit(const Literal& guest)
{
    cout << id << " [label=\"" << guest.code() << "\"];" << endl;

}
//---------------------------------------------------------------------------
void DOTVisitor::visit(const Identifier& guest)
{
    cout << id << " [label=\"" << guest.code() << "\"];" << endl;
}
//---------------------------------------------------------------------------
void pt::DOTVisitor::visit(const ParseTree& guest)
{
    size_t myId = id;
    string label = "function-definition";

    cout << "digraph {" << endl;

    cout << myId << " [label=\"" << label << "\"];" << endl;
    std::vector<std::unique_ptr<PTNode>> const& nodes = guest.getChildren();
    for(auto& n : nodes)
    {
        size_t curId = id;
        id++;
        n->accept(*this);
        cout << myId << " -> " << curId + 1 << endl;
    }

    cout << "}" << endl;
}
//---------------------------------------------------------------------------
void pt::DOTVisitor::visit(const ParamDecl& guest)
{
    size_t myId = id;
    string label = "parameter-declaration";

    cout << myId << " [label=\"" << label << "\"];" << endl;

    std::vector<std::unique_ptr<PTNode>> const& nodes = guest.getChildren();
    for(auto& n : nodes)
    {
        size_t curId = id;
        id++;
        n->accept(*this);
        cout << myId << " -> " << curId + 1 << endl;

    }
}
//---------------------------------------------------------------------------
void pt::DOTVisitor::visit(const VarDecl& guest)
{
    size_t myId = id;
    string label = "variable-declaration";


    cout << myId << " [label=\"" << label << "\"];" << endl;

    std::vector<std::unique_ptr<PTNode>> const& nodes = guest.getChildren();
    for(auto& n : nodes)
    {
        size_t curId = id;
        id++;
        n->accept(*this);
        cout << myId << " -> " << curId + 1 << endl;
    }
}
//---------------------------------------------------------------------------
void pt::DOTVisitor::visit(const ConstDecl& guest)
{
    size_t myId = id;
    string label = "constant-declaration";

    cout << myId << " [label=\"" << label << "\"];" << endl;

    std::vector<std::unique_ptr<PTNode>> const& nodes = guest.getChildren();
    for(auto& n : nodes)
    {
        size_t curId = id;
        id++;
        n->accept(*this);
        cout << myId << " -> " << curId + 1 << endl;
    }
}
//---------------------------------------------------------------------------
void pt::DOTVisitor::visit(const DeclList& guest)
{
    size_t myId = id;
    string label = "declaration-list";


    cout << myId << " [label=\"" << label << "\"];" << endl;

    std::vector<std::unique_ptr<PTNode>> const& nodes = guest.getChildren();
    for(auto& n : nodes)
    {
        size_t curId = id;
        id++;
        n->accept(*this);
        cout << myId << " -> " << curId + 1 << endl;
    }
}
//---------------------------------------------------------------------------
void pt::DOTVisitor::visit(const InitDeclList& guest)
{
    size_t myId = id;
    string label = "init-declarator-list";

    cout << myId << " [label=\"" << label << "\"];" << endl;

    std::vector<std::unique_ptr<PTNode>> const& nodes = guest.getChildren();
    for(auto& n : nodes)
    {
        size_t curId = id;
        id++;
        n->accept(*this);
        cout << myId << " -> " << curId + 1 << endl;
    }
}
//---------------------------------------------------------------------------
void pt::DOTVisitor::visit(const InitDecl& guest)
{
    size_t myId = id;
    string label = "init-declarator";

    cout << myId << " [label=\"" << label << "\"];" << endl;

    std::vector<std::unique_ptr<PTNode>> const& nodes = guest.getChildren();
    for(auto& n : nodes)
    {
        size_t curId = id;
        id++;
        n->accept(*this);
        cout << myId << " -> " << curId + 1 << endl;
    }
}
//---------------------------------------------------------------------------
void pt::DOTVisitor::visit(const CompStmt& guest)
{
    size_t myId = id;
    string label = "compound-statement";


    cout << myId << " [label=\"" << label << "\"];" << endl;

    std::vector<std::unique_ptr<PTNode>> const& nodes = guest.getChildren();
    for(auto& n : nodes)
    {
        size_t curId = id;
        id++;
        n->accept(*this);
        cout << myId << " -> " << curId + 1 << endl;
    }
}
//---------------------------------------------------------------------------
void pt::DOTVisitor::visit(const StmtList& guest)
{
    size_t myId = id;
    string label = "statement-list";

    cout << myId << " [label=\"" << label << "\"];" << endl;

    std::vector<std::unique_ptr<PTNode>> const& nodes = guest.getChildren();
    for(auto& n : nodes)
    {
        size_t curId = id;
        id++;
        n->accept(*this);
        cout << myId << " -> " << curId + 1 << endl;
    }
}
//---------------------------------------------------------------------------
void pt::DOTVisitor::visit(const Stmt& guest)
{
    size_t myId = id;
    string label = "statement";

    cout << myId << " [label=\"" << label << "\"];" << endl;

    std::vector<std::unique_ptr<PTNode>> const& nodes = guest.getChildren();
    for(auto& n : nodes)
    {
        size_t curId = id;
        id++;
        n->accept(*this);
        cout << myId << " -> " << curId + 1 << endl;
    }
}
//---------------------------------------------------------------------------
void pt::DOTVisitor::visit(const AssignExpr& guest)
{
    size_t myId = id;
    string label = "assignment-expression";

    cout << myId << " [label=\"" << label << "\"];" << endl;

    std::vector<std::unique_ptr<PTNode>> const& nodes = guest.getChildren();
    for(auto& n : nodes)
    {
        size_t curId = id;
        id++;
        n->accept(*this);
        cout << myId << " -> " << curId + 1 << endl;
    }
}
//---------------------------------------------------------------------------
void pt::DOTVisitor::visit(const AddExpr& guest)
{
    size_t myId = id;
    string label = "additive-expression";


    cout << myId << " [label=\"" << label << "\"];" << endl;

    std::vector<std::unique_ptr<PTNode>> const& nodes = guest.getChildren();
    for(auto& n : nodes)
    {
        size_t curId = id;
        id++;
        n->accept(*this);
        cout << myId << " -> " << curId + 1 << endl;
    }
}
//---------------------------------------------------------------------------
void pt::DOTVisitor::visit(const MulExpr& guest)
{
    size_t myId = id;
    string label = "multiplicative-expression";

    cout << myId << " [label=\"" << label << "\"];" << endl;

    std::vector<std::unique_ptr<PTNode>> const& nodes = guest.getChildren();
    for(auto& n : nodes)
    {
        size_t curId = id;
        id++;
        n->accept(*this);
        cout << myId << " -> " << curId + 1 << endl;
    }
}
//---------------------------------------------------------------------------
void pt::DOTVisitor::visit(const UnExpr& guest)
{
    size_t myId = id;
    string label = "unary-expression";

    cout << myId << " [label=\"" << label << "\"];" << endl;

    std::vector<std::unique_ptr<PTNode>> const& nodes = guest.getChildren();
    for(auto& n : nodes)
    {
        size_t curId = id;
        id++;
        n->accept(*this);
        cout << myId << " -> " << curId + 1 << endl;
    }
}
//---------------------------------------------------------------------------
void pt::DOTVisitor::visit(const PrimExpr& guest)
{
    size_t myId = id;
    string label = "primary-expression";


    cout << myId << " [label=\"" << label << "\"];" << endl;

    std::vector<std::unique_ptr<PTNode>> const& nodes = guest.getChildren();
    for(auto& n : nodes)
    {
        size_t curId = id;
        id++;
        n->accept(*this);
        cout << myId << " -> " << curId + 1 << endl;
    }
}
//---------------------------------------------------------------------------
DOTVisitor::DOTVisitor() : id(0)
{

}
//---------------------------------------------------------------------------
} // pljit::parser::pt
//---------------------------------------------------------------------------
