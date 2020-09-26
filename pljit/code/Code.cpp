#include "pljit/code/Code.hpp"
#include <cassert>
#include <iostream>
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace pljit::code {
//---------------------------------------------------------------------------
pljit::code::Code::Code(std::string_view code) : m_code(code)
{

}
//---------------------------------------------------------------------------
std::string_view Code::view()
{
    return m_code;
}
//---------------------------------------------------------------------------
std::string_view Code::view(size_t index, size_t length)
{
    return m_code.substr(index, length);
}
//---------------------------------------------------------------------------
std::string_view Code::line(size_t index) const
{
    size_t from = index;
    size_t to = index;
    while (from > 0)
    {
        if (m_code.at(from) == '\n')
        {
            ++from;
            break;
        }
        --from;
    }
    while (to < m_code.size())
    {
        if (m_code.at(to) == '\n')
        {
            --to;
            break;
        }
        ++to;
    }
    assert(from < m_code.size());
    return m_code.substr(from, to - from + 1);
}
//---------------------------------------------------------------------------
void Code::print(size_t index, std::string_view context)
{
    printContext(index, context);
    cout << '^' << endl;
}
//---------------------------------------------------------------------------
void Code::print(size_t index, size_t length, std::string_view context)
{
    printContext(index, context);
    if (length > 0)
        cout << '^';
    else if (length == 0)
        cout << " ^";
    if (length > 0)
    {
        for (size_t i = 0; i < length - 1; i++)
            cout << '~';
    }
    cout << endl;
}
//---------------------------------------------------------------------------
void Code::printContext(size_t index, std::string_view context)
{
    pair<size_t, size_t> pos = position(index);
    cout << pos.first << ":" << pos.second << ": " << context << endl;
    cout << line(index) << endl;

    for (size_t i = 0; i < pos.second; i++)
    {
        cout << " ";
    }
}
//---------------------------------------------------------------------------
std::pair<size_t, size_t> Code::position(size_t index)
{
    size_t lines = 0;
    size_t chars = 0;

    for (size_t i = 0; i < index; i++)
    {
        chars++;
        if (m_code.at(i) == '\n')
        {
            lines++;
            chars = 0;
        }
    }
    return {lines, chars};
}
//---------------------------------------------------------------------------
} // pljit::code
//---------------------------------------------------------------------------