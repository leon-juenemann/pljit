#pragma once
//---------------------------------------------------------------------------
#include "pljit/lexer/Lexer.hpp"
#include "pljit/lexer/Token.hpp"
#include "pljit/parser/pt/ParseTree.hpp"
#include "pljit/parser/Error.hpp"
#include "pljit/code/Reference.hpp"
#include "pljit/parser/Error.hpp"
//---------------------------------------------------------------------------
namespace pljit::parser {
class Parser {

    public:
    explicit Parser(lexer::Lexer* l);
    std::unique_ptr<pt::ParseTree> parse();
    Error error();
    private:
    lexer::Lexer* m_lexer;
    Error m_error;

    std::unique_ptr<pt::ParamDecl> paramDecl();
    std::unique_ptr<pt::VarDecl> varDecl();
    std::unique_ptr<pt::ConstDecl> constDecl();
    std::unique_ptr<pt::DeclList> declList();
    std::unique_ptr<pt::InitDeclList> initDeclList();
    std::unique_ptr<pt::InitDecl> initDecl();
    std::unique_ptr<pt::CompStmt> compStmt();
    std::unique_ptr<pt::StmtList> stmtList();
    std::unique_ptr<pt::Stmt> stmt();
    std::unique_ptr<pt::AssignExpr> assignExpr();
    std::unique_ptr<pt::AddExpr> addExpr();
    std::unique_ptr<pt::MulExpr> mulExpr();
    std::unique_ptr<pt::UnExpr> unExpr();
    std::unique_ptr<pt::PrimExpr> primExpr();

    std::unique_ptr<pt::Identifier> parseIdentifier();
    std::unique_ptr<pt::Literal> parseLiteral();
    std::unique_ptr<pt::Generic> parseGeneric(const std::string& expected);


    code::Range combinedRange(const std::vector<std::unique_ptr<pt::PTNode>>& nodes) const;

};
//---------------------------------------------------------------------------
} // pljit::parser
//---------------------------------------------------------------------------