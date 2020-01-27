#ifndef ast_statement_hpp
#define ast_statement_hpp

#include "ast_tree.hpp"
#include "ast_labeled_statement.hpp"
#include "ast_compound_statement.hpp"
#include "ast_expression_statement.hpp"
#include "ast_selection_statement.hpp"
#include "ast_iteration_statement.hpp"
#include "ast_jump_statement.hpp"
#include <string>


class Compound_statement;

class Statement
    : public Tree
{

private:
    Labeled_statement* labeled_stmnt;
    Compound_statement* compound_stmnt;
    Expression_statement* expression_stmnt;
    Selection_statement* selection_stmnt;
    Iteration_statement* iteration_stmnt;
    Jump_statement* jump_stmnt;

public:
    virtual ~Statement() {}
    virtual void print_python(std::ostream &dst) const;
    virtual void compile(std::ostream &dst, Context& context) const;

    Statement(
        Labeled_statement* labeled_stmnt_in,
        Compound_statement* compound_stmnt_in,
        Expression_statement* expression_stmnt_in,
        Selection_statement* selection_stmnt_in,
        Iteration_statement* iteration_stmnt_in,
        Jump_statement* jump_stmnt_in) :
        labeled_stmnt(labeled_stmnt_in),
        compound_stmnt(compound_stmnt_in),
        expression_stmnt(expression_stmnt_in),
        selection_stmnt(selection_stmnt_in),
        iteration_stmnt(iteration_stmnt_in),
        jump_stmnt(jump_stmnt_in) {}
};

#endif
