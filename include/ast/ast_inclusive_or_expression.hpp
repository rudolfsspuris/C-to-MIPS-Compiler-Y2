#ifndef ast_inclusive_or_expression_hpp
#define ast_inclusive_or_expression_hpp

#include "ast_tree.hpp"
#include "ast_exclusive_or_expression.hpp"
#include <string>




class Inclusive_or_expression
    : public Tree
{

private:
    Exclusive_or_expression* exclusive_or_expr;
    Inclusive_or_expression* inclusive_or_expr;
public:
    virtual ~Inclusive_or_expression() {}
    virtual void print_python(std::ostream &dst) const;
    virtual void compile(std::ostream &dst, Context& context) const;
    int evaluate(Context& context) const;


    Inclusive_or_expression(
        Exclusive_or_expression* exclusive_or_expr_in,
        Inclusive_or_expression* inclusive_or_expr_in) :
        exclusive_or_expr(exclusive_or_expr_in),
        inclusive_or_expr(inclusive_or_expr_in) {}
};

#endif
