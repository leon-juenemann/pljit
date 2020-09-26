#pragma once
//---------------------------------------------------------------------------
namespace pljit::semana::ast {
class AST;
}
namespace pljit::optimizer {
class Optimization {
    public:
    virtual ~Optimization() = default;

    virtual void optimize(pljit::semana::ast::AST& ast) = 0;

};
//---------------------------------------------------------------------------
} // namespace ast
//---------------------------------------------------------------------------
