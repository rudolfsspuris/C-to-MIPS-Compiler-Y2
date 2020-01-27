#ifndef ast_struct_declaration_list_hpp
#define ast_struct_declaration_list_hpp

#include "ast_tree.hpp"
#include "ast_struct_declaration.hpp"
#include <string>




class Struct_declaration_list
    : public Tree
{

private:
    Struct_declaration* struct_decl;
    Struct_declaration_list* struct_decl_list;
public:
    virtual ~Struct_declaration_list() {}
    virtual void print_python(std::ostream &dst) const{};
    virtual void compile(std::ostream &dst, Context& context) const{};

    Struct_declaration_list(
        Struct_declaration* struct_decl_in,
        Struct_declaration_list* struct_decl_list_in) :
        struct_decl(struct_decl_in),
        struct_decl_list(struct_decl_list_in) {}
};

#endif
