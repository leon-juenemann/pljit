#include "pljit/semana/Error.hpp"
#include <pljit/code/Code.hpp>
#include <string>
#include <sstream>
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace pljit::semana {
//---------------------------------------------------------------------------
Error::Error(code::Range src) : m_src(src)
{

}
//---------------------------------------------------------------------------
IdRedeclaration::IdRedeclaration(code::Range src, code::Range firstDecl)
    : Error(src), m_firstDecl(firstDecl)
{

}
//---------------------------------------------------------------------------
void IdRedeclaration::print() const
{
    std::stringstream output;
    output << "error: redeclaration of \"" << m_src.view() << "\"";
    m_src.print(output.str());
    string note = "note: already declared here";
    m_firstDecl.print(note);
}
//---------------------------------------------------------------------------
void AssignToConst::print() const
{
    std::stringstream output;
    output << "error: assigning to const \"" << m_src.view() << "\"";
    m_src.print(output.str());
    string note = "note: declared as const here";
    m_constDecl.print(note);
}
//---------------------------------------------------------------------------
AssignToConst::AssignToConst(code::Range src, code::Range constDecl)
        : Error(src), m_constDecl(constDecl)
{

}
//---------------------------------------------------------------------------
UninitializedVar::UninitializedVar(code::Range src, code::Range varDecl)
        : Error(src), m_varDecl(varDecl)
{

}
//---------------------------------------------------------------------------
void UninitializedVar::print() const
{
    std::stringstream output;
    output << "error: variable \"" << m_src.view() << "\"" << " is not initialized";
    m_src.print(output.str());
    string note = "note: declared here";
    m_varDecl.print(note);
}
//---------------------------------------------------------------------------
void UndeclaredId::print() const
{
    std::stringstream output;
    output << "error: identifier \"" << m_src.view() << "\"" << " was never declared";
    m_src.print(output.str());

}
//---------------------------------------------------------------------------
UndeclaredId::UndeclaredId(code::Range src) : Error(src)
{

}
//---------------------------------------------------------------------------
MissingRetStmt::MissingRetStmt(code::Range src) : Error(src)
{

}
//---------------------------------------------------------------------------
void MissingRetStmt::print() const
{
    std::stringstream output;
    output << "error: function is missing return statement";
    m_src.print(output.str());
}
//---------------------------------------------------------------------------
} // pljit::semana
//---------------------------------------------------------------------------