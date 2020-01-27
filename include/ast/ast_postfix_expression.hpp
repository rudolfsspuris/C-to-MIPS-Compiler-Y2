#ifndef ast_postfix_expression_hpp
#define ast_postfix_expression_hpp

#include "ast_tree.hpp"
#include "ast_primary_expression.hpp"
#include "ast_expression.hpp"
#include "ast_argument_expression_list.hpp"
#include <string>


class Primary_expression;
class Expression;


class Postfix_expression
    : public Tree
{

private:
    Primary_expression* prim_expr;
    Postfix_expression* postf_expr;
    Expression* expr;
    Argument_expression_list* arg_expr_list;
    std::string* oper;
    std::string* identifier;

public:
    virtual ~Postfix_expression() {}
    virtual void print_python(std::ostream &dst) const;
    virtual void compile(std::ostream &dst, Context& context) const;
    int evaluate(Context& context) const;


    Postfix_expression(
        Primary_expression* prim_expr_in,
        Postfix_expression* postf_expr_in,
        Expression* expr_in,
        Argument_expression_list* arg_expr_list_in,
        std::string* operator_in,
        std::string* identifier_in) :
        prim_expr(prim_expr_in),
        postf_expr(postf_expr_in),
        expr(expr_in),
        arg_expr_list(arg_expr_list_in),
        oper(operator_in),
        identifier(identifier_in)
        {}
};

#endif
