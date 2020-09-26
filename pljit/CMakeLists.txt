set(PLJIT_SOURCES
        code/Code.cpp
        code/Reference.cpp

        lexer/Token.cpp
        lexer/Lexer.cpp

        parser/Parser.cpp
        parser/Error.cpp
        parser/pt/ParseTree.cpp
        parser/pt/DOTVisitor.cpp

        semana/Semana.cpp
        semana/Error.cpp
        semana/SymbolTable.cpp
        semana/ast/AST.cpp
        semana/ast/DOTVisitor.cpp
        semana/ast/Evaluator.cpp

        optimizer/DeadCode.cpp
        )


add_library(pljit_core STATIC ${PLJIT_SOURCES})
target_include_directories(pljit_core PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(pljit_core PUBLIC Threads::Threads)

add_clang_tidy_target(lint_pljit_core ${PLJIT_SOURCES})
add_dependencies(lint lint_pljit_core)

add_executable(pljit main.cpp)
target_link_libraries(pljit PUBLIC pljit_core)
