#ifndef ast_labeled_statement_hpp
#define ast_labeled_statement_hpp

#include "ast_tree.hpp"
#include "ast_statement.hpp"
#include "ast_constant_expression.hpp"
#include <string>


class Statement;

class Labeled_statement
    : public Tree
{

private:
    std::string* identifier;
    Statement* stmnt;
    Constant_expression* cont_expr;

public:
    virtual ~Labeled_statement() {}
    virtual void print_python(std::ostream &dst) const{};
    virtual void compile(std::ostream &dst, Context& context) const{};

    Labeled_statement(
        std::string* identifier_in,
        Statement* stmnt_in,
        Constant_expression* cont_expr_in) :
        identifier(identifier_in),
        stmnt(stmnt_in),
        cont_expr(cont_expr_in) {}
};

#endif
