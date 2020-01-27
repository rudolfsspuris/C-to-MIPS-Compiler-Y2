#ifndef ast_init_declarator_list_hpp
#define ast_init_declarator_list_hpp

#include "ast_tree.hpp"
#include "ast_init_declarator.hpp"
#include <string>




class Init_declarator_list
    : public Tree
{

private:
    Init_declarator* init_decl;
    Init_declarator_list* init_decl_list;
public:
    virtual ~Init_declarator_list() {}
    virtual void print_python(std::ostream &dst) const;
    virtual void compile(std::ostream &dst, Context& context) const;

    Init_declarator_list(Init_declarator* init_decl_in, Init_declarator_list* init_decl_list_in) : init_decl(init_decl_in), init_decl_list(init_decl_list_in) {}
};

#endif
