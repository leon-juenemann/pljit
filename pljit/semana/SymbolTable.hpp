#pragma once
//---------------------------------------------------------------------------
#include "pljit/code/Code.hpp"
#include "pljit/parser/pt/ParseTree.hpp"
#include <map>
//---------------------------------------------------------------------------
namespace pljit::semana {
//---------------------------------------------------------------------------
class Symbol {
    public:
    enum class Type {
        constant,
        variable,
        parameter
    };

    Symbol(Type type, code::Range src, int64_t value);

    Type type() const;
    code::Range range() const;
    int64_t value() const;
    void setToInitialized();
    bool initialized() const;

    private:
    Type m_type;
    code::Range m_src;
    int64_t m_value;
    bool m_initialized;
};
//---------------------------------------------------------------------------
class SymbolTable {

    public:
    SymbolTable();
    std::pair<bool, Symbol> insert(Symbol& symbol);
    Symbol* find(const std::string& key);
    std::vector<std::pair<std::string, int64_t>> variableState();

    private:
    std::map<std::string, Symbol> m_symbols;
};
//---------------------------------------------------------------------------
} // pljit::semana
//---------------------------------------------------------------------------
