#ifndef ast_declarator_hpp
#define ast_declarator_hpp

#include "ast_tree.hpp"
#include "ast_pointer.hpp"
#include "ast_direct_declarator.hpp"
#include <string>




class Declarator
    : public Tree
{

private:
    Pointer* pointer;
    Direct_declarator* dir_decl;
    //Declarator* declarator;
public:
    virtual ~Declarator() {}
    virtual void print_python(std::ostream &dst) const;
    virtual void compile(std::ostream &dst, Context& context) const;

    Declarator(Pointer* pointer_in, Direct_declarator* dir_decl_in) : pointer(pointer_in), dir_decl(dir_decl_in){
        // declarator = NULL;
    }
};

#endif
