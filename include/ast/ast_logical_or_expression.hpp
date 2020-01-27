#ifndef ast_logical_or_expression_hpp
#define ast_logical_or_expression_hpp

#include "ast_tree.hpp"
#include "ast_logical_and_expression.hpp"
#include <string>




class Logical_or_expression
    : public Tree
{

private:
    Logical_and_expression* logical_and_expr;
    Logical_or_expression* logical_or_expr;
public:
    virtual ~Logical_or_expression() {}
    virtual void print_python(std::ostream &dst) const;
    virtual void compile(std::ostream &dst, Context& context) const;
    int evaluate(Context& context) const;

    Logical_or_expression(
        Logical_and_expression* logical_and_expr_in,
        Logical_or_expression* logical_or_expr_in) :
        logical_and_expr(logical_and_expr_in),
        logical_or_expr(logical_or_expr_in) {}
};

#endif
