#include "pljit/optimizer/DeadCode.hpp"
//---------------------------------------------------------------------------
using namespace std;
using namespace pljit::semana;
namespace pljit::optimizer {
//---------------------------------------------------------------------------
void DeadCode::optimize(pljit::semana::ast::AST& ast)
{
    size_t i = 0;
    while (i < ast.m_stmts.size())
    {
        if (ast.m_stmts[i].get()->type() == ast::AST::Type::RetStmt)
            break;

        i++;
    }

    size_t deadStmts = ast.m_stmts.size() - i - 1;

    for (size_t j = 0; j < deadStmts; j++)
    {
        ast.m_stmts.pop_back();
    }

}
} // namespace ast
//---------------------------------------------------------------------------
