#ifndef ast_cast_expression_hpp
#define ast_cast_expression_hpp

#include "ast_tree.hpp"
#include "ast_unary_expression.hpp"
#include "ast_type_name.hpp"
#include "ast_cast_expression.hpp"
#include <string>




class Cast_expression
    : public Tree
{

private:
    Unary_expression* unary_expr;
    Type_name* type_name;
    Cast_expression* cast_expr;

public:
    virtual ~Cast_expression() {}
    virtual void print_python(std::ostream &dst) const;
    virtual void compile(std::ostream &dst, Context& context) const;
    int evaluate(Context& context) const;

    Cast_expression(
        Unary_expression* unary_expr_in,
        Type_name* type_name_in,
        Cast_expression* cast_expr_in):
        unary_expr(unary_expr_in),
        type_name(type_name_in),
        cast_expr(cast_expr_in) {}
};

#endif
