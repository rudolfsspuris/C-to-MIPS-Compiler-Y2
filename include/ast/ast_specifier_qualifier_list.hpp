#ifndef ast_specifier_qualifier_list_hpp
#define ast_specifier_qualifier_list_hpp

#include "ast_tree.hpp"
#include "ast_type_specifier.hpp"
#include "ast_type_qualifier.hpp"
#include <string>




class Specifier_qualifier_list
    : public Tree
{

private:
    Type_specifier* type_spec;
    Specifier_qualifier_list* spec_qual_list;
    Type_qualifier* type_qual;
public:
    virtual ~Specifier_qualifier_list() {}
    virtual void print_python(std::ostream &dst) const{};
    virtual void compile(std::ostream &dst, Context& context) const{};

    Specifier_qualifier_list(
        Type_specifier* type_spec_in,
        Specifier_qualifier_list* spec_qual_list_in,
        Type_qualifier* type_qual_in) :
        type_spec(type_spec_in),
        spec_qual_list(spec_qual_list_in),
        type_qual(type_qual_in) {}
};

#endif
