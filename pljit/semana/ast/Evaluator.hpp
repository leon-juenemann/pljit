#pragma once
//---------------------------------------------------------------------------
#include <map>
#include <vector>
//---------------------------------------------------------------------------
namespace pljit::semana::ast {
//---------------------------------------------------------------------------
class Evaluator {
    friend class RetStmt;
    public:
    Evaluator();

    void initialize(const std::vector<std::pair<std::string, int64_t>>& vars);
    uint64_t getValue(const std::string& name) const;
    void setValue(const std::string& name, int64_t value);
    int64_t returnValue() const;
    private:
    std::map<std::string, int64_t> m_vars;
    uint64_t m_retValue;

};
//---------------------------------------------------------------------------
} // pljit::semana::ast
//---------------------------------------------------------------------------