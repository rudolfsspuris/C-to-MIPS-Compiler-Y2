#ifndef ast_argument_expression_list_hpp
#define ast_argument_expression_list_hpp

#include "ast_tree.hpp"
#include "ast_assignment_expression.hpp"
#include <string>


class Assignment_expression; //forward declaration

class Argument_expression_list
    : public Tree
{

private:
    Assignment_expression* assign_expr;
    Argument_expression_list* arg_expr_list;
public:
    virtual ~Argument_expression_list() { };
    virtual void print_python(std::ostream &dst) const;
    virtual void compile(std::ostream &dst, Context& context) const;

    Argument_expression_list(
        Assignment_expression* assign_expr_in,
        Argument_expression_list* arg_expr_list_in) :
        assign_expr(assign_expr_in),
        arg_expr_list(arg_expr_list_in) {}
};

#endif
