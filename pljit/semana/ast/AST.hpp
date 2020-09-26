#pragma once
//---------------------------------------------------------------------------
#include "pljit/code/Code.hpp"
#include "pljit/semana/ast/ASTVisitor.hpp"
#include "pljit/semana/ast/Evaluator.hpp"
#include <memory>
#include <pljit/optimizer/DeadCode.hpp>
//---------------------------------------------------------------------------
namespace pljit::semana::ast {
//---------------------------------------------------------------------------
class ASTVisitor;
//---------------------------------------------------------------------------
class ASTNode {
    public:
    enum class Type {
        Function,
        AssignStmt,
        RetStmt,
        Literal,
        Identifier,
        Unary,
        Binary
    };
    protected:
    Type m_type;

    public:
    virtual void accept(ASTVisitor& visitor) const = 0;
    explicit ASTNode(Type type);
    virtual ~ASTNode() = default;

    virtual int64_t evaluate(Evaluator& context) const = 0;
    Type type() const;
};
//---------------------------------------------------------------------------
class Expression : public ASTNode {
    protected:
    explicit Expression(Type type);
};
//---------------------------------------------------------------------------
class Identifier : public Expression {
    public:
    explicit Identifier(std::string_view name);

    void accept(ASTVisitor& visitor) const override;

    int64_t evaluate(Evaluator& context) const override;

    std::string name() const;
    private:
    std::string m_name;
};
//---------------------------------------------------------------------------
class Literal : public Expression {
    public:
    explicit Literal(int64_t value);
    int64_t value() const;
    void accept(ASTVisitor& visitor) const override;

    int64_t evaluate(Evaluator& context) const override;

    private:
    int64_t m_value;
};
//---------------------------------------------------------------------------
class Unary : public Expression {
    public:
    enum class Operator {
        Plus,
        Minus
    };

    Unary(Operator op, std::unique_ptr<Expression> expr);
    Operator op() const;
    const Expression& getChild() const;
    void accept(ASTVisitor& visitor) const override;

    int64_t evaluate(Evaluator& context) const override;

    private:
    Operator m_op;
    std::unique_ptr<Expression> m_expr;
};
//---------------------------------------------------------------------------
class Binary : public Expression {
    public:
    enum class Operator {
        Plus,
        Minus,
        Mul,
        Div
    };

    Binary(Operator op,
            std::unique_ptr<Expression> left,
            std::unique_ptr<Expression> right);

    void accept(ASTVisitor& visitor) const override;

    int64_t evaluate(Evaluator& context) const override;

    Operator op() const;
    const Expression& getLeft() const;
    const Expression& getRight() const;


    private:
    Operator m_operator;
    std::unique_ptr<Expression> m_leftExpr;
    std::unique_ptr<Expression> m_rightExpr;
};
//---------------------------------------------------------------------------
class Statement : public ASTNode {
    protected:
    explicit Statement(Type type);
};
//---------------------------------------------------------------------------
class AssignStmt : public Statement {
    public:
    AssignStmt(std::unique_ptr<Identifier> var,
            std::unique_ptr<Expression> expr);

    void accept(ASTVisitor& visitor) const override;

    int64_t evaluate(Evaluator& context) const override;

    const Identifier& getIdentifier() const;
    const Expression& getExpression() const;
    private:
    std::unique_ptr<Identifier> m_var;
    std::unique_ptr<Expression> m_expr;
};
//---------------------------------------------------------------------------
class RetStmt : public Statement {
    public:
    explicit RetStmt(std::unique_ptr<Expression> ret);

    void accept(ASTVisitor& visitor) const override;

    int64_t evaluate(Evaluator& context) const override;

    const Expression& getExpression() const;

    private:
    std::unique_ptr<Expression> m_retExpr;
};
//---------------------------------------------------------------------------
class AST : public ASTNode {

    public:
    explicit AST(std::vector<std::unique_ptr<Statement>> stmts,
            std::vector<std::pair<std::string, int64_t>> vars);
    std::vector<std::unique_ptr<Statement>> const& getStmts() const;

    void accept(ASTVisitor& visitor) const override;

    int64_t evaluate(Evaluator& context) const override;

    public:
    std::vector<std::unique_ptr<Statement>> m_stmts;
    std::vector<std::pair<std::string, int64_t>> m_vars;
};
//---------------------------------------------------------------------------
} // pljit::semana::ast
//---------------------------------------------------------------------------