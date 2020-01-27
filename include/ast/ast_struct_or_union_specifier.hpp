#ifndef ast_struct_or_union_specifier_hpp
#define ast_struct_or_union_specifier_hpp

#include "ast_tree.hpp"
#include "ast_struct_declaration_list.hpp"
#include <string>




class Struct_or_union_specifier
    : public Tree
{

private:
    std::string* identifier;
    Struct_declaration_list* struct_decl_list;
public:
    virtual ~Struct_or_union_specifier() {}
    virtual void print_python(std::ostream &dst) const{}
    virtual void compile(std::ostream &dst, Context& context) const{};

    Struct_or_union_specifier(
        std::string* identifier_in,
        Struct_declaration_list* struct_decl_list_in) :
        identifier(identifier_in),
        struct_decl_list(struct_decl_list_in) {}
};

#endif
