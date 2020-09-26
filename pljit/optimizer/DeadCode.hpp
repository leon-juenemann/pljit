#pragma once
//---------------------------------------------------------------------------
#include "pljit/semana/ast/AST.hpp"
#include "pljit/optimizer/Optimization.hpp"
//---------------------------------------------------------------------------
namespace pljit::optimizer {
class DeadCode : public Optimization {
    public:
    DeadCode() = default;
    void optimize(pljit::semana::ast::AST& ast) override ;

};
} // namespace ast
//---------------------------------------------------------------------------
