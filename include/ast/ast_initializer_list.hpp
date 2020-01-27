#ifndef ast_initializer_list_hpp
#define ast_initializer_list_hpp

#include "ast_tree.hpp"
#include "ast_initializer.hpp"
#include <string>

class Initializer;

class Initializer_list
    : public Tree
{

private:
    Initializer* init;
    Initializer_list* init_list;
public:
    virtual ~Initializer_list() {}
    virtual void print_python(std::ostream &dst) const{};
    virtual void compile(std::ostream &dst, Context& context) const{};


    Initializer_list(
        Initializer* init_in,
        Initializer_list* init_list_in) :
        init(init_in),
        init_list(init_list_in){}
};

#endif
