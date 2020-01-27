#ifndef ast_identifier_list_hpp
#define ast_identifier_list_hpp

#include "ast_tree.hpp"
#include "ast_identifier_list.hpp"
#include <string>




class Identifier_list
    : public Tree
{

private:
    std::string* identifier;
    Identifier_list* ident_list;
public:
    virtual ~Identifier_list() {}
    virtual void print_python(std::ostream &dst) const{};
    virtual void compile(std::ostream &dst, Context& context) const{};

    Identifier_list(
        std::string* identifier_in,
        Identifier_list* ident_list_in) :
        identifier(identifier_in),
        ident_list(ident_list_in) {}
};

#endif
