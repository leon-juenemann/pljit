#include "pljit/lexer/Lexer.hpp"
#include "pljit/parser/Parser.hpp"
#include "pljit/parser/pt/ParseTree.hpp"
#include "pljit/parser/pt/DOTVisitor.hpp"
#include "pljit/semana/ast/DOTVisitor.hpp"
#include "pljit/optimizer/DeadCode.hpp"

#include "pljit/code/Code.hpp"
#include "pljit/semana/Semana.hpp"
#include <fstream>
#include <iostream>
//---------------------------------------------------------------------------
using namespace std;
using namespace pljit::code;
using namespace pljit::lexer;
using namespace pljit::parser;
using namespace pljit::semana;
using namespace pljit::optimizer;
//---------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    string_view input =
            "PARAM width, height, depth;\n"
            "VAR volume, test;\n"
            "CONST density = 2400;\n"
            "BEGIN\n"
            "volume := width * volume * depth;\n"
            "RETURN density * volume\n"
            "END.";

    Code code(input);

    Lexer lexer(&code);

    Parser parser(&lexer);

    unique_ptr<pt::ParseTree> pt = parser.parse();

    if (!pt)
    {
        parser.error().print();
        return 0;
    }

    //DOTVisitor visitor;
    //pt->accept(visitor);

    Semana semana;
    unique_ptr<ast::AST> ast = semana.analyse(pt);
    if (!ast)
    {
        semana.error().print();
        return 0;
    }

    //ast::DOTVisitor astVisitor;
    //ast->accept(astVisitor);

    DeadCode deadCode;
    deadCode.optimize(*ast);

    //ast::DOTVisitor astVisitor;
    //ast->accept(astVisitor);

    pljit::semana::ast::Evaluator eval;
    ast->evaluate(eval);

    cout << eval.returnValue();

    return 0;
}
//---------------------------------------------------------------------------
