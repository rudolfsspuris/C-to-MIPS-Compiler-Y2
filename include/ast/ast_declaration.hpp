#ifndef ast_declaration_hpp
#define ast_declaration_hpp

#include "ast_tree.hpp"
#include "ast_declaration_specifiers.hpp"
#include "ast_init_declarator_list.hpp"



class Declaration
    : public Tree
{

protected:
    Declaration_specifiers* decl_spec;
	Init_declarator_list* init_decl_list;
public:
    virtual ~Declaration() {}
    virtual void print_python(std::ostream &dst) const;
    virtual void compile(std::ostream &dst, Context& context) const;

    Declaration(
        Declaration_specifiers* decl_spec_in,
        Init_declarator_list* decl_list_in) :
        decl_spec(decl_spec_in),
        init_decl_list(decl_list_in) {}
};

#endif
