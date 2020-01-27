#ifndef ast_direct_declarator_hpp
#define ast_direct_declarator_hpp

#include "ast_tree.hpp"
#include "ast_declarator.hpp"
#include "ast_direct_declarator.hpp"
#include "ast_constant_expression.hpp"
#include "ast_parameter_type_list.hpp"
#include "ast_identifier_list.hpp"
#include <string>


class Declarator;

class Direct_declarator
    : public Tree
{

private:
    std::string* identifier;
    Declarator* decl;
    Direct_declarator* direct_decl;
    Constant_expression* const_expr;
    Parameter_type_list* param_type_list;
    Identifier_list* identif_list;
    int parse_rule_followed; //Rule parser follows to create the node

public:
    virtual ~Direct_declarator() {}
    virtual void print_python(std::ostream &dst) const;
    virtual void compile(std::ostream &dst, Context& context) const;

    Direct_declarator(
        std::string* identifier_in,
        Declarator* decl_in,
        Direct_declarator* direct_decl_in,
        Constant_expression* const_expr_in,
        Parameter_type_list* param_type_list_in,
        Identifier_list* identif_list_in,
        int parse_rule_followed_in) :
        identifier(identifier_in),
        decl(decl_in),
        direct_decl(direct_decl_in),
        const_expr(const_expr_in),
        param_type_list(param_type_list_in),
        identif_list(identif_list_in),
        parse_rule_followed(parse_rule_followed_in) {}

};

#endif
