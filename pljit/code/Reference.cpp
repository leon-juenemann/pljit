#include "pljit/code/Reference.hpp"
//---------------------------------------------------------------------------
namespace pljit::code {
//---------------------------------------------------------------------------
Location::Location(Code* src, size_t index) : m_code(src), m_index(index)
{

}
//---------------------------------------------------------------------------
size_t Location::index() const
{
    return m_index;
}
//---------------------------------------------------------------------------
std::string_view Location::view() const
{
    return m_code->view(m_index, 1);
}
//---------------------------------------------------------------------------
void Location::print(std::string_view context) const
{
    m_code->print(m_index, context);
}
//---------------------------------------------------------------------------
Range::Range(Code* src, size_t index, size_t length)
: m_code(src), m_index(index), m_length(length)
{

}
//---------------------------------------------------------------------------
size_t Range::index() const
{
    return m_index;
}
//---------------------------------------------------------------------------
size_t Range::length() const
{
    return m_length;
}
//---------------------------------------------------------------------------
std::string_view Range::view() const
{
    return m_code->view(m_index, m_length);
}
//---------------------------------------------------------------------------
void Range::print(std::string_view context) const
{
    m_code->print(m_index, m_length, context);
}
//---------------------------------------------------------------------------
} // pljit::code
//---------------------------------------------------------------------------