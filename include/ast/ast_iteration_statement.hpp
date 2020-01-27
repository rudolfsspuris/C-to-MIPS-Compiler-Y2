#ifndef ast_iteration_statement_hpp
#define ast_iteration_statement_hpp

#include "ast_tree.hpp"
#include "ast_expression.hpp"
#include "ast_statement.hpp"
#include "ast_expression_statement.hpp"
#include <string>




class Iteration_statement
    : public Tree
{

private:
    Expression* expr;
    Statement* statement;
    Expression_statement* expr_stmnt_1;
    Expression_statement* expr_stmnt_2;
    std::string* keyword;

public:
    virtual ~Iteration_statement() {}
    virtual void print_python(std::ostream &dst) const;
    virtual void compile(std::ostream &dst, Context& context) const;

    Iteration_statement(
        Expression* expr_in,
        Statement* statement_in,
        Expression_statement* expr_stmnt_1_in,
        Expression_statement* expr_stmnt_2_in,
        std::string* keyword_in) :
        expr(expr_in),
        statement(statement_in),
        expr_stmnt_1(expr_stmnt_1_in),
        expr_stmnt_2(expr_stmnt_2_in),
        keyword(keyword_in) {}
};

#endif
