#ifndef ast_shift_expression_hpp
#define ast_shift_expression_hpp

#include "ast_tree.hpp"
#include "ast_additive_expression.hpp"
#include <string>




class Shift_expression
    : public Tree
{

private:
    Additive_expression* additive_expr;
    Shift_expression* shift_expr;
    std::string* op;

public:
    virtual ~Shift_expression() {}
    virtual void print_python(std::ostream &dst) const;
    virtual void compile(std::ostream &dst, Context& context) const;
    int evaluate(Context& context) const;

    Shift_expression(
        Additive_expression* additive_expr_in,
        Shift_expression* shift_expr_in,
        std::string* op_in):
        additive_expr(additive_expr_in),
        shift_expr(shift_expr_in),
        op(op_in) {}
};

#endif
