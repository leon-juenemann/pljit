#pragma once
//---------------------------------------------------------------------------
#include "pljit/semana/ast/ASTVisitor.hpp"
//---------------------------------------------------------------------------
namespace pljit::semana::ast {
//---------------------------------------------------------------------------
class DOTVisitor : public ASTVisitor {
    private:
    size_t m_id;

    public:
    DOTVisitor();

    void visit(const Identifier& guest) override;

    void visit(const Literal& guest) override;

    void visit(const Unary& guest) override;

    void visit(const Binary& guest) override;

    void visit(const AssignStmt& guest) override;

    void visit(const RetStmt& guest) override;

    void visit(const AST& guest) override;

};
//---------------------------------------------------------------------------
} // pljit::semana::ast
//---------------------------------------------------------------------------
