#ifndef ast_and_expression_hpp
#define ast_and_expression_hpp

#include "ast_tree.hpp"
#include "ast_equality_expression.hpp"
#include <string>




class And_expression
    : public Tree
{

private:
    Equality_expression* equality_expr;
    And_expression* and_expr;
public:
    virtual ~And_expression() {}
    virtual void print_python(std::ostream &dst) const;
    virtual void compile(std::ostream &dst, Context& context) const;
    int evaluate(Context& context) const;


    And_expression(
        Equality_expression* equality_expr_in,
        And_expression* and_expr_in) :
        equality_expr(equality_expr_in),
        and_expr(and_expr_in) {}
};

#endif
