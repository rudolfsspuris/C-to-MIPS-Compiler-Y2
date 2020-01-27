#ifndef ast_function_definition_hpp
#define ast_function_definition_hpp

#include "ast_tree.hpp"
#include "ast_declarator.hpp"
#include "ast_declaration_specifiers.hpp"
#include "ast_compound_statement.hpp"
#include "ast_declaration_list.hpp"
#include <string>




class Function_definition
    : public Tree
{

private:
    int parse_rule_followed; //Rule parser follows to create this node
    Declaration_specifiers* decl_spec;
    Declarator* decl;
    Compound_statement* compound_stmnt;
    Declaration_list* decl_list;
public:
    virtual ~Function_definition() {}
    virtual void print_python(std::ostream &dst) const;
    virtual void compile(std::ostream &dst, Context& context) const;

    Function_definition(
        int parse_rule_followed_in, //Rule parser follows to create this node
        Declaration_specifiers* decl_spec_in,
        Declarator* decl_in,
        Compound_statement* compound_stmnt_in,
        Declaration_list* decl_list_in) :
        parse_rule_followed(parse_rule_followed_in),
        decl_spec(decl_spec_in),
        decl(decl_in),
        compound_stmnt(compound_stmnt_in),
        decl_list(decl_list_in) {}
};

#endif
