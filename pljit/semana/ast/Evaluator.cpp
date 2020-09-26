#include "Evaluator.hpp"
#include <cassert>
//---------------------------------------------------------------------------
namespace pljit::semana::ast {
//---------------------------------------------------------------------------
Evaluator::Evaluator() : m_vars(), m_retValue(0)
{

}
//---------------------------------------------------------------------------
void Evaluator::initialize(const std::vector<std::pair<std::string, int64_t>>& vars)
{
    for (const auto& v : vars)
    {
        m_vars.insert(v);
    }
}
//---------------------------------------------------------------------------
uint64_t Evaluator::getValue(const std::string& name) const
{
    assert(m_vars.find(name) != m_vars.end());
    return m_vars.find(name)->second;
}
//---------------------------------------------------------------------------
void Evaluator::setValue(const std::string& name, int64_t value)
{
    m_vars[name] = value;

}
//---------------------------------------------------------------------------
int64_t Evaluator::returnValue() const
{
    return m_retValue;
}
//---------------------------------------------------------------------------
} // pljit::semana::ast
//---------------------------------------------------------------------------
