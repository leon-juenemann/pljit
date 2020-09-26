#include "pljit/semana/ast/AST.hpp"
#include <utility>
#include <utility>
#include <vector>
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace pljit::semana::ast {
//---------------------------------------------------------------------------
ASTNode::ASTNode(ASTNode::Type type) : m_type(type)
{
}
//---------------------------------------------------------------------------
ASTNode::Type ASTNode::type() const
{
    return m_type;
}
//---------------------------------------------------------------------------
AST::AST(vector<unique_ptr<Statement>> stmts,
        vector<pair<std::string, int64_t>> vars)
        : ASTNode(Type::Function),
        m_stmts(move(stmts)),
        m_vars(std::move(vars))
{

}
//---------------------------------------------------------------------------
void AST::accept(ASTVisitor& visitor) const
{
    visitor.visit(*this);

}
//---------------------------------------------------------------------------
std::vector<std::unique_ptr<Statement>> const& AST::getStmts() const
{
    return m_stmts;
}
//---------------------------------------------------------------------------
int64_t AST::evaluate(Evaluator& context) const
{
    context.initialize(m_vars);

    for (const auto& s :  m_stmts)
    {
        s->evaluate(context);
        if (s->type() == Type::RetStmt)
            break;
    }

    return context.returnValue();
}
//---------------------------------------------------------------------------
Expression::Expression(Type type) : ASTNode(type)
{

}
//---------------------------------------------------------------------------
Literal::Literal(int64_t value)
    : Expression(Type::Literal),
      m_value(value)
{

}
//---------------------------------------------------------------------------
void Literal::accept(ASTVisitor& visitor) const
{
    visitor.visit(*this);
}
//---------------------------------------------------------------------------
int64_t Literal::value() const
{
    return m_value;
}
//---------------------------------------------------------------------------
int64_t Literal::evaluate(Evaluator& context) const
{
    return m_value;
}
//---------------------------------------------------------------------------
Statement::Statement(ASTNode::Type type) : ASTNode(type)
{

}
//---------------------------------------------------------------------------
Identifier::Identifier(std::string_view name)
    : Expression(Type::Identifier),
      m_name(name)
{

}
//---------------------------------------------------------------------------
void Identifier::accept(ASTVisitor& visitor) const
{
    visitor.visit(*this);
}
//---------------------------------------------------------------------------
std::string Identifier::name() const
{
    return m_name;
}
//---------------------------------------------------------------------------
int64_t Identifier::evaluate(Evaluator& context) const
{
    return context.getValue(m_name);
}
//---------------------------------------------------------------------------
Unary::Unary(Unary::Operator op,
        unique_ptr<Expression> expr)
        : Expression(Type::Unary),
          m_op(op),
          m_expr(move(expr))
{

}
//---------------------------------------------------------------------------
void Unary::accept(ASTVisitor& visitor) const
{
    visitor.visit(*this);
}
//---------------------------------------------------------------------------
Unary::Operator Unary::op() const
{
    return m_op;
}
//---------------------------------------------------------------------------
const Expression& Unary::getChild() const
{
    return *m_expr;
}
//---------------------------------------------------------------------------
int64_t Unary::evaluate(Evaluator& context) const
{
    if (m_op == Operator::Plus)
        return m_expr->evaluate(context);

    return - m_expr->evaluate(context);
}
//---------------------------------------------------------------------------
Binary::Binary(Operator op, unique_ptr<Expression> left,
        std::unique_ptr<Expression> right)
        : Expression(Type::Binary),
          m_operator(op),
          m_leftExpr(move(left)),
          m_rightExpr(move(right))
{

}
//---------------------------------------------------------------------------
void Binary::accept(ASTVisitor& visitor) const
{
    visitor.visit(*this);
}
//---------------------------------------------------------------------------
Binary::Operator Binary::op() const
{
    return m_operator;
}
//---------------------------------------------------------------------------
const Expression& Binary::getRight() const
{
    return *m_rightExpr;
}
//---------------------------------------------------------------------------
const Expression& Binary::getLeft() const
{
    return *m_leftExpr;
}
//---------------------------------------------------------------------------
int64_t Binary::evaluate(Evaluator& context) const
{
    int64_t left = m_leftExpr->evaluate(context);
    int64_t right = m_rightExpr->evaluate(context);

    if (m_operator == Operator::Plus)
        return left + right;
    if (m_operator == Operator::Minus)
        return left - right;
    if (m_operator == Operator::Div)
    {
        if (right == 0)
        {
            // error division by 0
        }
        else
        {
            return left / right;
        }

    }
    return left * right;
}
//---------------------------------------------------------------------------
AssignStmt::AssignStmt(unique_ptr<Identifier> var,
        std::unique_ptr<Expression> expr)
        : Statement(Type::AssignStmt),
        m_var(move(var)),
        m_expr(move(expr))
{

}
//---------------------------------------------------------------------------
void AssignStmt::accept(ASTVisitor& visitor) const
{
    visitor.visit(*this);
}
//---------------------------------------------------------------------------
const Identifier& AssignStmt::getIdentifier() const
{
    return *m_var;
}
//---------------------------------------------------------------------------
const Expression& AssignStmt::getExpression() const
{
    return *m_expr;
}
//---------------------------------------------------------------------------
int64_t AssignStmt::evaluate(Evaluator& context) const
{
    int64_t right = m_expr->evaluate(context);
    context.setValue(m_var->name(), right);
    return 0;
}
//---------------------------------------------------------------------------
RetStmt::RetStmt(unique_ptr<Expression> ret)
: Statement(Type::RetStmt), m_retExpr(move(ret))
{

}
//---------------------------------------------------------------------------
void RetStmt::accept(ASTVisitor& visitor) const
{
    visitor.visit(*this);
}
//---------------------------------------------------------------------------
const Expression& RetStmt::getExpression() const
{
    return *m_retExpr;
}
//---------------------------------------------------------------------------
int64_t RetStmt::evaluate(Evaluator& context) const
{
    context.m_retValue = m_retExpr->evaluate(context);

    return 0;
}
//---------------------------------------------------------------------------
} // pljit::semana::ast
//---------------------------------------------------------------------------