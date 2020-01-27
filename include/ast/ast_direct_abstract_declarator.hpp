#ifndef ast_direct_abstract_declarator_hpp
#define ast_direct_abstract_declarator_hpp

#include "ast_tree.hpp"
#include "ast_abstract_declarator.hpp"
#include "ast_constant_expression.hpp"
#include "ast_direct_abstract_declarator.hpp"
#include "ast_parameter_type_list.hpp"
#include <string>


class Abstract_declarator;

class Direct_abstract_declarator
    : public Tree
{

private:
    int parse_rule_followed; //Rule parser follows to create this node
    Abstract_declarator* abstr_decl;
    Constant_expression* const_expr;
    Direct_abstract_declarator* direct_abstr_decl;
    Parameter_type_list* pparam_type_list;
public:
    virtual ~Direct_abstract_declarator() {}
    virtual void print_python(std::ostream &dst) const{};
    virtual void compile(std::ostream &dst, Context& context) const{};

    Direct_abstract_declarator(
        int parse_rule_followed_in, //Rule parser follows to create this node
        Abstract_declarator* abstr_decl_in,
        Constant_expression* const_expr_in,
        Direct_abstract_declarator* direct_abstr_decl_in,
        Parameter_type_list* pparam_type_list_in) :
        parse_rule_followed(parse_rule_followed_in),
        abstr_decl(abstr_decl_in),
        const_expr(const_expr_in),
        direct_abstr_decl(direct_abstr_decl_in),
        pparam_type_list(pparam_type_list_in){}
};

#endif
