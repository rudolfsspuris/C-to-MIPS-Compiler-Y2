#ifndef ast_enum_specifier_hpp
#define ast_enum_specifier_hpp

#include "ast_tree.hpp"
#include "ast_enumerator_list.hpp"
#include <string>




class Enum_specifier
    : public Tree
{

private:
    Enumerator_list* enum_list;
    std::string* identifier;
public:
    virtual ~Enum_specifier() {}
    virtual void print_python(std::ostream &dst) const{};
    virtual void compile(std::ostream &dst, Context& context) const;

    Enum_specifier(
        Enumerator_list* enum_list_in,
        std::string* identifier_in) :
        enum_list(enum_list_in),
        identifier(identifier_in){}
};

#endif
