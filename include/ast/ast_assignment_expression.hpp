#ifndef ast_assignment_expression_hpp
#define ast_assignment_expression_hpp

#include "ast_tree.hpp"
#include "ast_conditional_expression.hpp"
#include "ast_unary_expression.hpp"
#include "ast_assignment_expression.hpp"
#include <string>




class Assignment_expression
    : public Tree
{

private:
    Conditional_expression* cond_expr;
    Unary_expression* unary_expr;
    std::string* assign_op;
    Assignment_expression* assign_expr;
public:
    virtual ~Assignment_expression() {}
    virtual void print_python(std::ostream &dst) const;
    virtual void compile(std::ostream &dst, Context& context) const;
    int evaluate(Context& context) const;

    Assignment_expression(Conditional_expression* cond_expr_in, Unary_expression* unary_expr_in, std::string* assign_op_in, Assignment_expression* assign_expr_in) : cond_expr(cond_expr_in), unary_expr(unary_expr_in), assign_op(assign_op_in), assign_expr(assign_expr_in){}
};

#endif
