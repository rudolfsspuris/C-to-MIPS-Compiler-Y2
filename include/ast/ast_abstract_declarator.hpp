#ifndef ast_abstract_declarator_hpp
#define ast_abstract_declarator_hpp

#include "ast_tree.hpp"
#include "ast_pointer.hpp"
#include "ast_direct_abstract_declarator.hpp"
#include <string>




class Abstract_declarator
    : public Tree
{

private:
    Pointer* ptr;
    Direct_abstract_declarator* direct_abstr_decl;
public:
    virtual ~Abstract_declarator() {}
    virtual void print_python(std::ostream &dst) const{};
    virtual void compile(std::ostream &dst, Context& context) const{};

    Abstract_declarator(
        Pointer* ptr_in,
        Direct_abstract_declarator* direct_abstr_decl_in) :
        ptr(ptr_in),
        direct_abstr_decl(direct_abstr_decl_in){}
};

#endif
