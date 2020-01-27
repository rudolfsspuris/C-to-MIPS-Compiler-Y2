#ifndef ast_additive_expression_hpp
#define ast_additive_expression_hpp

#include "ast_tree.hpp"
#include "ast_multiplicative_expression.hpp"
#include <string>




class Additive_expression
    : public Tree
{

private:
    Multiplicative_expression* mult_expr;
    Additive_expression* additive_expr;
    std::string* op;

public:
    virtual ~Additive_expression() {}
    virtual void print_python(std::ostream &dst) const;
    virtual void compile(std::ostream &dst, Context& context) const;
    int evaluate(Context& context) const;

    Additive_expression(
        Multiplicative_expression* mult_expr_in,
        Additive_expression* additive_expr_in,
        std::string* op_in):
        mult_expr(mult_expr_in),
        additive_expr(additive_expr_in),
        op(op_in) {}
};

#endif
