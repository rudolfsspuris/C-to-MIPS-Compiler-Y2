#ifndef ast_exclusive_or_expression_hpp
#define ast_exclusive_or_expression_hpp

#include "ast_tree.hpp"
#include "ast_and_expression.hpp"
#include <string>




class Exclusive_or_expression
    : public Tree
{

private:
    And_expression* and_expr;
    Exclusive_or_expression* exclusive_or_expr;
public:
    virtual ~Exclusive_or_expression() {}
    virtual void print_python(std::ostream &dst) const;
    virtual void compile(std::ostream &dst, Context& context) const;
    int evaluate(Context& context) const;


    Exclusive_or_expression(
        And_expression* and_expr_in,
        Exclusive_or_expression* exclusive_or_expr_in) :
        and_expr(and_expr_in),
        exclusive_or_expr(exclusive_or_expr_in) {}
};

#endif
