#ifndef ast_struct_declarator_hpp
#define ast_struct_declarator_hpp

#include "ast_tree.hpp"
#include "ast_declarator.hpp"
#include "ast_constant_expression.hpp"
#include <string>




class Struct_declarator
    : public Tree
{

private:
    Declarator* decl;
    Constant_expression* const_expr;
public:
    virtual ~Struct_declarator() {}
    virtual void print_python(std::ostream &dst) const{}
    virtual void compile(std::ostream &dst, Context& context) const{};

    Struct_declarator(
        Declarator* decl_in,
        Constant_expression* const_expr_in) :
        decl(decl_in),
        const_expr(const_expr_in) {}
};

#endif
