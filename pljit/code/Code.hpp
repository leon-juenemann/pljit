#pragma once
//---------------------------------------------------------------------------
#include <string_view>
//---------------------------------------------------------------------------
namespace pljit::code {
//---------------------------------------------------------------------------
class Code {
    public:
    explicit Code(std::string_view code);
    std::string_view view();
    std::string_view view(size_t index, size_t length);

    void print(size_t index, std::string_view context);
    void print(size_t index, size_t length, std::string_view context);

    private:
    std::string_view m_code;

    std::string_view line(size_t index) const;
    void printContext(size_t index, std::string_view context);
    std::pair<size_t, size_t> position(size_t index);

};
//---------------------------------------------------------------------------
} // plijt::code
//---------------------------------------------------------------------------