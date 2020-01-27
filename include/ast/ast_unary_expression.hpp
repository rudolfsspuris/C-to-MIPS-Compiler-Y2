#ifndef ast_unary_expression_hpp
#define ast_unary_expression_hpp

#include "ast_tree.hpp"
#include "ast_postfix_expression.hpp"
#include "ast_unary_expression.hpp"
#include "ast_cast_expression.hpp"
#include "ast_type_name.hpp"
#include <string>

class Cast_expression;


class Unary_expression
    : public Tree
{

private:
    Postfix_expression* postf_expr;
    std::string* oper;
    Unary_expression* unary_expr;
    std::string* unary_op;
    Cast_expression* cast_expr;
    Type_name* type_name;

public:
    virtual ~Unary_expression() {}
    virtual void print_python(std::ostream &dst) const;
    virtual void compile(std::ostream &dst, Context& context) const;
    int evaluate(Context& context) const;

    Unary_expression(
        Postfix_expression* postf_expr_in,
        std::string* oper_in,
        Unary_expression* unary_expr_in,
        std::string* unary_op_in,
        Cast_expression* cast_expr_in,
        Type_name* type_name_in) :
        postf_expr(postf_expr_in),
        oper(oper_in),
        unary_expr(unary_expr_in),
        unary_op(unary_op_in),
        cast_expr(cast_expr_in),
        type_name(type_name_in)
        {}
};

#endif
