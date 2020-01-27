#ifndef ast_multiplicative_expression_hpp
#define ast_multiplicative_expression_hpp

#include "ast_tree.hpp"
#include "ast_cast_expression.hpp"
#include <string>




class Multiplicative_expression
    : public Tree
{

private:
    Cast_expression* cast_expr;
    Multiplicative_expression* mult_expr;
    std::string* op;

public:
    virtual ~Multiplicative_expression() {}
    virtual void print_python(std::ostream &dst) const;
    virtual void compile(std::ostream &dst, Context& context) const;
    int evaluate(Context& context) const;


    Multiplicative_expression(
        Cast_expression* cast_expr_in,
        Multiplicative_expression* mult_expr_in,
        std::string* op_in):
        cast_expr(cast_expr_in),
        mult_expr(mult_expr_in),
        op(op_in) {}
};

#endif
