#pragma once
//---------------------------------------------------------------------------
#include "pljit/semana/ast/AST.hpp"
//---------------------------------------------------------------------------
namespace pljit::semana::ast {
//---------------------------------------------------------------------------
class Identifier;
class Literal;
class Unary;
class Binary;
class AssignStmt;
class RetStmt;
class AST;
//---------------------------------------------------------------------------
class ASTVisitor {
    public:
    virtual ~ASTVisitor() = default;

    virtual void visit(const Identifier& guest) = 0;
    virtual void visit(const Literal& guest) = 0;
    virtual void visit(const Unary& guest) = 0;
    virtual void visit(const Binary& guest) = 0;
    virtual void visit(const AssignStmt& guest) = 0;
    virtual void visit(const RetStmt& guest) = 0;
    virtual void visit(const AST& guest) = 0;

};
//---------------------------------------------------------------------------
} // pljit::semana::ast
//---------------------------------------------------------------------------
