#pragma once
//---------------------------------------------------------------------------
#include "pljit/code/Reference.hpp"
#include <string>
//---------------------------------------------------------------------------
namespace pljit::parser {
//---------------------------------------------------------------------------
class Error {
    private:
    code::Range m_src;
    std::string m_context;

    public:
    Error();
    Error(code::Range src, std::string_view context);
    Error(const Error& other) = default;

    void print();
    Error& operator=(const Error& other) = default;
    std::string_view context();
    code::Range range();


};
//---------------------------------------------------------------------------
} // pljit::parser
//---------------------------------------------------------------------------