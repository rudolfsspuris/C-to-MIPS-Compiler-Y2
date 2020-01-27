#ifndef ast_struct_declaration_hpp
#define ast_struct_declaration_hpp

#include "ast_tree.hpp"
#include "ast_specifier_qualifier_list.hpp"
#include "ast_struct_declarator_list.hpp"
#include <string>


class Specifier_qualifier_list;

class Struct_declaration
    : public Tree
{

private:
    Specifier_qualifier_list* spec_qual_list;
    Struct_declarator_list* struct_declarator_list;
public:
    virtual ~Struct_declaration() {}
    virtual void print_python(std::ostream &dst) const{}
    virtual void compile(std::ostream &dst, Context& context) const{};

    Struct_declaration(
        Specifier_qualifier_list* spec_qual_list_in,
        Struct_declarator_list* struct_declarator_list_in) :
        spec_qual_list(spec_qual_list_in),
        struct_declarator_list(struct_declarator_list_in) {}
};

#endif
