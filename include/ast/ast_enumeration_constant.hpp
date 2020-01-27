#ifndef ast_enumeration_constant_hpp
#define ast_enumeration_constant_hpp

#include "ast_tree.hpp"
#include <string>




class Enumeration_constant
    : public Tree
{

private:
public:
    std::string* identifier;
    virtual ~Enumeration_constant() {}
    virtual void print_python(std::ostream &dst) const{};
    virtual void compile(std::ostream &dst, Context& context) const{};

    Enumeration_constant(
        std::string* identifier_in) :
        identifier(identifier_in){}
};

#endif
