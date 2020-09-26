#pragma once
//---------------------------------------------------------------------------
#include <pljit/code/Code.hpp>
#include <pljit/code/Reference.hpp>
#include <memory>
//---------------------------------------------------------------------------
namespace pljit::semana {
//---------------------------------------------------------------------------
class Error {
    protected:
    code::Range m_src;

    public:
    explicit Error(code::Range src);
    virtual ~Error() = default;
    virtual void print() const = 0;

};
//---------------------------------------------------------------------------
class MissingRetStmt : public Error {
    public:
    explicit MissingRetStmt(code::Range src);

    void print() const override;

};
//---------------------------------------------------------------------------
class IdRedeclaration : public Error {
    public:
    IdRedeclaration(code::Range src, code::Range firstDecl);

    void print() const override;

    private:
    code::Range m_firstDecl;
};
//---------------------------------------------------------------------------
class UndeclaredId : public Error {
    public:
    explicit UndeclaredId(code::Range src);

    void print() const override;

};
//---------------------------------------------------------------------------
class AssignToConst : public Error {
    public:
    AssignToConst(code::Range src, code::Range constDecl);

    void print() const override;

    private:
    code::Range m_constDecl;
};
//---------------------------------------------------------------------------
class UninitializedVar : public Error {
    public:
    UninitializedVar(code::Range src, code::Range varDecl);

    void print() const override;

    private:
    code::Range m_varDecl;
};
//---------------------------------------------------------------------------
} // pljit::semana
//---------------------------------------------------------------------------
