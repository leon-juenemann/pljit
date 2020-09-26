#pragma once
//---------------------------------------------------------------------------
#include "pljit/parser/pt/PTVisitor.hpp"
//---------------------------------------------------------------------------
namespace pljit::parser::pt {
//---------------------------------------------------------------------------
class DOTVisitor : public PTVisitor {
    private:
    size_t id;

    public:
    DOTVisitor();
    void visit(const Generic& guest) override;

    void visit(const Identifier& guest) override;

    void visit(const Literal& guest) override;

    void visit(const ParseTree& guest) override;

    void visit(const ParamDecl& guest) override;

    void visit(const VarDecl& guest) override;

    void visit(const ConstDecl& guest) override;

    void visit(const DeclList& guest) override;

    void visit(const InitDeclList& guest) override;

    void visit(const InitDecl& guest) override;

    void visit(const CompStmt& guest) override;

    void visit(const StmtList& guest) override;

    void visit(const Stmt& guest) override;

    void visit(const AssignExpr& guest) override;

    void visit(const AddExpr& guest) override;

    void visit(const MulExpr& guest) override;

    void visit(const UnExpr& guest) override;

    void visit(const PrimExpr& guest) override;

};
//---------------------------------------------------------------------------
} //  pljit::parser::pt
//---------------------------------------------------------------------------
