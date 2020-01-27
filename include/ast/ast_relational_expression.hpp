#ifndef ast_relational_expression_hpp
#define ast_relational_expression_hpp

#include "ast_tree.hpp"
#include "ast_shift_expression.hpp"
#include <string>




class Relational_expression
    : public Tree
{

private:
    Shift_expression* shift_expr;
    Relational_expression* relat_expr;
    std::string* op;

public:
    virtual ~Relational_expression() {}
    virtual void print_python(std::ostream &dst) const;
    virtual void compile(std::ostream &dst, Context& context) const;
    int evaluate(Context& context) const;


    Relational_expression(
        Shift_expression* shift_expr_in,
        Relational_expression* relat_expr_in,
        std::string* op_in):
        shift_expr(shift_expr_in),
        relat_expr(relat_expr_in),
        op(op_in) {}
};

#endif
