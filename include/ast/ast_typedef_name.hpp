#ifndef ast_typedef_name_hpp
#define ast_typedef_name_hpp

#include "ast_tree.hpp"
#include <string>




class Typedef_name
    : public Tree
{

private:
    std::string* typedef_name;
public:
    virtual ~Typedef_name() {}
    virtual void print_python(std::ostream &dst) const{}
    virtual void compile(std::ostream &dst, Context& context) const{};

    Typedef_name(std::string* typedef_name_in) : typedef_name(typedef_name_in) {}
};

#endif
