#pragma once
//---------------------------------------------------------------------------
#include "pljit/code/Code.hpp"
#include <cstdlib>
//---------------------------------------------------------------------------
namespace pljit::code {
//---------------------------------------------------------------------------
class Location {

    public:
    Location(Code* src, size_t index);

    size_t index() const;
    std::string_view view() const;
    void print(std::string_view context) const;

    private:
    Code* m_code;
    size_t m_index;
};
//---------------------------------------------------------------------------
class Range {
    public:
    Range(Code* src, size_t index, size_t length);

    size_t index() const;
    size_t length() const;
    std::string_view view() const;

    void print(std::string_view context) const;

    private:
    Code* m_code;
    size_t m_index;
    size_t m_length;
};
//---------------------------------------------------------------------------
} // plijt::code
//---------------------------------------------------------------------------
