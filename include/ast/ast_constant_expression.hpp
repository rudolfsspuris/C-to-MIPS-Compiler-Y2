#ifndef ast_constant_expression_hpp
#define ast_constant_expression_hpp

#include "ast_tree.hpp"
#include "ast_conditional_expression.hpp"
#include <string>


class Conditional_expression;

class Constant_expression
    : public Tree
{

private:
    Conditional_expression* cond_expr;
public:
    virtual ~Constant_expression() {}
    virtual void print_python(std::ostream &dst) const{};
    virtual void compile(std::ostream &dst, Context& context) const{};
    int evaluate(Context& context) const;

    Constant_expression(Conditional_expression* cond_expr_in) : cond_expr(cond_expr_in){}
};

#endif
