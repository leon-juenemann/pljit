#include "pljit/parser/Error.hpp"
#include <iostream>
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace pljit::parser {
//---------------------------------------------------------------------------
Error::Error() : m_src(nullptr, 0, 0), m_context()
{

}
//---------------------------------------------------------------------------
Error::Error(code::Range src, std::string_view context)
    : m_src(src), m_context(context)
{

}
//---------------------------------------------------------------------------
code::Range Error::range()
{
    return m_src;
}
//---------------------------------------------------------------------------
std::string_view Error::context()
{
    return m_context;
}
//---------------------------------------------------------------------------
void Error::print()
{
    if (m_src.index() == 0 && m_src.length() == 0)
        cout << "No error: parsing successful";
    else
    m_src.print(m_context);
}
//---------------------------------------------------------------------------
} // pljit::parser
//---------------------------------------------------------------------------