#ifndef ast_tree_hpp
#define ast_tree_hpp
#include <iostream>
#include "mips_forward_declaration.hpp"

class Tree;

typedef const Tree *TreePtr;

class Tree
{
public:
    virtual ~Tree() { };
    virtual void print_python(std::ostream &dst) const{ };
    virtual void compile(std::ostream &dst, Context& context) const{};
};


#endif
