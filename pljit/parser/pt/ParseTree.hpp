#pragma once
// --------------------------------------------------------------------------
#include "pljit/code/Code.hpp"
#include "pljit/code/Reference.hpp"
#include "pljit/parser/pt/PTVisitor.hpp"
#include <vector>
#include <memory>
// --------------------------------------------------------------------------
namespace pljit::parser::pt {
// --------------------------------------------------------------------------
class PTVisitor;
class DeclList;
class CompStmt;
class InitDecl;
class InitDeclList;
class StmtList;
class AssignExpr;
class AddExpr;
class MulExpr;
class UnExpr;
class PrimExpr;
class ParamDecl;
class VarDecl;
class ConstDecl;
// --------------------------------------------------------------------------
class PTNode {
    public:
    enum class Type {

        // non-terminals
        fun_def,
        param_decl,
        var_decl,
        const_decl,
        comp_stmt,
        decl_list,
        init_decl_list,
        init_decl,
        stmt_list,
        stmt,
        assign_expr,
        add_expr,
        mul_expr,
        un_expr,
        prim_expr,

        // terminals
        identifier,
        literal,
        generic
    };

    protected:
    Type m_type;
    code::Range m_code;


    public:
    PTNode(Type type, code::Range code);
    virtual ~PTNode() = default;
    virtual void accept(PTVisitor& visitor) const = 0;
    virtual Type type() const;

    code::Range range() const;
    std::string_view code() const;
};
// --------------------------------------------------------------------------
class Nonterminal : public PTNode {
    public:
    std::vector<std::unique_ptr<PTNode>> const& getChildren() const;
    protected:
    Nonterminal(code::Range code, Type t, std::vector<std::unique_ptr<PTNode>> n);

    std::vector<std::unique_ptr<PTNode>> m_children;
};
// --------------------------------------------------------------------------
class Generic : public PTNode {
    public:
    explicit Generic(code::Range code);

    void accept(PTVisitor& visitor) const override;
};
// --------------------------------------------------------------------------
class Literal : public PTNode {
    public:
    explicit Literal(code::Range code);

    void accept(PTVisitor& visitor) const override;
    uint64_t value() const;
};
// --------------------------------------------------------------------------
class Identifier : public PTNode {
    public:
    explicit Identifier(code::Range code);
    std::string name() const;
    void accept(PTVisitor& visitor) const override;
    private:
    std::string m_name;

};
// --------------------------------------------------------------------------
class ParseTree : public Nonterminal {
    public:
    ParseTree(code::Range code,
              std::vector<std::unique_ptr<PTNode>> n);

    void accept(PTVisitor& visitor) const override;
    bool hasParameters();
    bool hasVariables();
    bool hasConstants();

    const ParamDecl& getParamDecl() const;
    const VarDecl& getVarDecl() const;
    const ConstDecl& getConstDecl() const;
    const CompStmt& getCompStmt() const;


    private:
    bool m_hasParams;
    bool m_hasVars;
    bool m_hasConsts;

};
// --------------------------------------------------------------------------
class ParamDecl : public Nonterminal {
    public:
    ParamDecl(code::Range code, std::vector<std::unique_ptr<PTNode>> n);
    void accept(PTVisitor& visitor) const override;
    const DeclList& getDeclList() const;
};
// --------------------------------------------------------------------------
class VarDecl : public Nonterminal {
    public:
    VarDecl(code::Range code, std::vector<std::unique_ptr<PTNode>> n);
    void accept(PTVisitor& visitor) const override;
    const DeclList& getDeclList() const;
};
// --------------------------------------------------------------------------
class ConstDecl : public Nonterminal {
    public:
    ConstDecl(code::Range code, std::vector<std::unique_ptr<PTNode>> n);
    void accept(PTVisitor& visitor) const override;
    const InitDeclList& getInitDeclList() const;


};
// --------------------------------------------------------------------------
class DeclList : public Nonterminal {
    public:
    DeclList(code::Range code, std::vector<std::unique_ptr<PTNode>> n);
    void accept(PTVisitor& visitor) const override;

};
// --------------------------------------------------------------------------
class InitDeclList : public Nonterminal {
    public:
    InitDeclList(code::Range code, std::vector<std::unique_ptr<PTNode>> n);
    void accept(PTVisitor& visitor) const override;

};
// --------------------------------------------------------------------------
class InitDecl : public Nonterminal {
    public:
    InitDecl(code::Range code, std::vector<std::unique_ptr<PTNode>> n);
    void accept(PTVisitor& visitor) const override;
    const Literal& getLiteral() const;
    const Identifier& getIdentifier() const;


};
// --------------------------------------------------------------------------
class CompStmt : public Nonterminal {
    public:
    CompStmt(code::Range code, std::vector<std::unique_ptr<PTNode>> n);
    void accept(PTVisitor& visitor) const override;

    const StmtList& getStmtList() const;
};
// --------------------------------------------------------------------------
class StmtList : public Nonterminal {
    public:
    StmtList(code::Range code, std::vector<std::unique_ptr<PTNode>> n);
    void accept(PTVisitor& visitor) const override;

};
// --------------------------------------------------------------------------
class Stmt : public Nonterminal {
    public:
    Stmt(code::Range code, std::vector<std::unique_ptr<PTNode>> n, bool assign);
    void accept(PTVisitor& visitor) const override;

    bool isAssign() const;
    private:
    bool m_isAssign;
};
// --------------------------------------------------------------------------
class AssignExpr : public Nonterminal {
    public:
    AssignExpr(code::Range code, std::vector<std::unique_ptr<PTNode>> n);
    void accept(PTVisitor& visitor) const override;

};
// --------------------------------------------------------------------------
class AddExpr : public Nonterminal {
    public:
    enum class Option {
        no_option,
        plus,
        minus
    };

    AddExpr(code::Range code, std::vector<std::unique_ptr<PTNode>> n, Option o);
    void accept(PTVisitor& visitor) const override;
    Option option() const;

    private:
    Option m_option;
};
// --------------------------------------------------------------------------
class MulExpr : public Nonterminal {
    public:
    enum class Option {
        no_option,
        mul,
        div
    };

    MulExpr(code::Range code, std::vector<std::unique_ptr<PTNode>> n, Option o);
    void accept(PTVisitor& visitor) const override;
    Option option() const;

    private:
    Option m_option;
};
// --------------------------------------------------------------------------
class UnExpr : public Nonterminal {
    public:
    enum class Option {
        no_option,
        plus,
        minus
    };
    UnExpr(code::Range code, std::vector<std::unique_ptr<PTNode>> n, Option o);
    void accept(PTVisitor& visitor) const override;
    Option option() const;

    private:
    Option m_option;
};
// --------------------------------------------------------------------------
class PrimExpr : public Nonterminal {
    public:
    enum class Option {
        identifier,
        literal,
        additive
    };

    PrimExpr(code::Range code, std::vector<std::unique_ptr<PTNode>> n, Option o);
    void accept(PTVisitor& visitor) const override;
    Option option() const;

    private:
    Option m_option;
};
// --------------------------------------------------------------------------
} // pljit::parser::pt
// --------------------------------------------------------------------------