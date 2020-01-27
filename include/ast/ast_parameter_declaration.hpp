#ifndef ast_parameter_declaration_hpp
#define ast_parameter_declaration_hpp

#include "ast_tree.hpp"
#include "ast_declaration_specifiers.hpp"
#include "ast_declarator.hpp"
#include "ast_abstract_declarator.hpp"
#include <string>




class Parameter_declaration
    : public Tree
{

private:
    Declaration_specifiers* decl_spec;
    Declarator* declarator;
    Abstract_declarator* abstr_decl;

public:
    virtual ~Parameter_declaration() {}
    virtual void print_python(std::ostream &dst) const;
    virtual void compile(std::ostream &dst, Context& context) const;

    Parameter_declaration(
        Declaration_specifiers* decl_spec_in,
        Declarator* declarator_in,
        Abstract_declarator* abstr_decl_in
        ) :
        decl_spec(decl_spec_in),
        declarator(declarator_in),
        abstr_decl(abstr_decl_in) {}
};

#endif
