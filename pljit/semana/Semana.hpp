#pragma once
//---------------------------------------------------------------------------
#include "pljit/semana/SymbolTable.hpp"
#include "pljit/semana/ast/AST.hpp"
#include "pljit/parser/pt/ParseTree.hpp"
#include "pljit/semana/Error.hpp"

namespace pljit::semana {
class Semana {
    public:
    Semana();

    const Error& error() const;
    std::unique_ptr<ast::AST> analyse(std::unique_ptr<parser::pt::ParseTree>& pt);


    private:
    SymbolTable m_symbolTable;
    std::unique_ptr<Error> m_error;
    bool m_hasReturn;

    std::unique_ptr<ast::Statement> analyseStmt(const std::unique_ptr<parser::pt::PTNode>& n);
    std::unique_ptr<ast::Expression> analyseExpr(const std::unique_ptr<parser::pt::PTNode>& n);
    std::unique_ptr<ast::Expression> analyseAdd(const std::unique_ptr<parser::pt::PTNode>& n);
    std::unique_ptr<ast::Expression> analyseMul(const std::unique_ptr<parser::pt::PTNode>& n);
    std::unique_ptr<ast::Expression> analyseUnary(const std::unique_ptr<parser::pt::PTNode>& n);
    std::unique_ptr<ast::Expression> analysePrim(const std::unique_ptr<parser::pt::PTNode>& n);
    std::unique_ptr<ast::Identifier> analyseIdentifier(const std::unique_ptr<parser::pt::PTNode>& n, bool left);



    bool analyseParamDecl(const parser::pt::ParamDecl& paramDecl);
    bool analyseVarDecl(const parser::pt::VarDecl& varDecl);
    bool analyseConstDecl(const parser::pt::ConstDecl& constDecl);
};
} // pljit::ast