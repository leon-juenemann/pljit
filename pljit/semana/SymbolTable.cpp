
#include "SymbolTable.hpp"
using namespace std;
namespace pljit::semana {


Symbol::Symbol(Symbol::Type type, code::Range src, int64_t value)
    : m_type(type), m_src(src), m_value(value), m_initialized(true)
{
    if (m_type == Type::variable)
    {
        m_initialized = false;
    }
}

Symbol::Type Symbol::type() const
{
    return m_type;
}

code::Range Symbol::range() const
{
    return  m_src;
}

int64_t Symbol::value() const
{
    return m_value;
}

bool Symbol::initialized() const
{
    return m_initialized;
}

void Symbol::setToInitialized()
{
    m_initialized = true;
}

SymbolTable::SymbolTable() : m_symbols()
{

}

pair<bool, Symbol> SymbolTable::insert(Symbol& symbol)
{
    string key = static_cast<string>(symbol.range().view());
    auto p = m_symbols.insert(pair<string, Symbol>(key, symbol));
    return pair(p.second, p.first->second);
}

Symbol* SymbolTable::find(const std::string& key)
{
    auto s = m_symbols.find(key);

    if(s == m_symbols.end())
    {
        return nullptr;
    }
    return  &s->second;
}

std::vector<std::pair<std::string, int64_t>> SymbolTable::variableState()
{
    std::vector<std::pair<std::string, int64_t>> vars;

    for (const auto& s : m_symbols)
    {
        vars.emplace_back(pair(s.first, s.second.value()));
    }
    return vars;
}

} // pljit::ast