#ifndef ast_equality_expression_hpp
#define ast_equality_expression_hpp

#include "ast_tree.hpp"
#include "ast_relational_expression.hpp"
#include "ast_equality_expression.hpp"
#include <string>




class Equality_expression
    : public Tree
{

private:
    Relational_expression* relat_expr;
    Equality_expression* equality_expr;
    std::string* op;
public:
    virtual ~Equality_expression() {}
    virtual void print_python(std::ostream &dst) const;
    virtual void compile(std::ostream &dst, Context& context) const;
    int evaluate(Context& context) const;


    Equality_expression(
        Relational_expression* relat_expr_in,
        Equality_expression* equality_expr_in,
        std::string* op_in):
        relat_expr(relat_expr_in),
        equality_expr(equality_expr_in),
        op(op_in) {}
};

#endif
