#pragma once
//---------------------------------------------------------------------------
#include "pljit/parser/pt/ParseTree.hpp"
// --------------------------------------------------------------------------
namespace pljit::parser::pt {
// --------------------------------------------------------------------------
class Generic;
class Identifier;
class Literal;
class ParseTree;
class ParamDecl;
class VarDecl;
class ConstDecl;
class DeclList;
class InitDeclList;
class InitDecl;
class CompStmt;
class StmtList;
class Stmt;
class AssignExpr;
class AddExpr;
class MulExpr;
class UnExpr;
class PrimExpr;
// --------------------------------------------------------------------------
class PTVisitor {
    public:
    virtual ~PTVisitor() = default;

    virtual void visit(const Generic& guest) = 0;
    virtual void visit(const Identifier& guest) = 0;
    virtual void visit(const Literal& guest) = 0;
    virtual void visit(const ParseTree& guest) = 0;
    virtual void visit(const ParamDecl& guest) = 0;
    virtual void visit(const VarDecl& guest) = 0;
    virtual void visit(const ConstDecl& guest) = 0;
    virtual void visit(const DeclList& guest) = 0;
    virtual void visit(const InitDeclList& guest) = 0;
    virtual void visit(const InitDecl& guest) = 0;
    virtual void visit(const CompStmt& guest) = 0;
    virtual void visit(const StmtList& guest) = 0;
    virtual void visit(const Stmt& guest) = 0;
    virtual void visit(const AssignExpr& guest) = 0;
    virtual void visit(const AddExpr& guest) = 0;
    virtual void visit(const MulExpr& guest) = 0;
    virtual void visit(const UnExpr& guest) = 0;
    virtual void visit(const PrimExpr& guest) = 0;
};
// --------------------------------------------------------------------------
} // pljit::parser::pt
//---------------------------------------------------------------------------
