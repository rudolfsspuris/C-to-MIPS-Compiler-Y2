#ifndef ast_selection_statement_hpp
#define ast_selection_statement_hpp

#include "ast_tree.hpp"
#include "ast_expression.hpp"
#include "ast_statement.hpp"
#include <string>




class Selection_statement
    : public Tree
{

private:
    Expression* expr;
    Statement* if_statement;
    Statement* else_statement;
    bool switch_stmnt;


public:
    virtual ~Selection_statement() {}
    virtual void print_python(std::ostream &dst) const;
    virtual void compile(std::ostream &dst, Context& context) const;

    Selection_statement(
        Expression* expr_in,
        Statement* if_statement_in,
        Statement* else_statement_in,
        bool switch_stmnt_in) :
        expr(expr_in),
        if_statement(if_statement_in),
        else_statement(else_statement_in),
        switch_stmnt(switch_stmnt_in) {}
};

#endif
