#ifndef ast_type_specifier_hpp
#define ast_type_specifier_hpp

#include "ast_tree.hpp"
#include "ast_struct_or_union_specifier.hpp"
#include "ast_enum_specifier.hpp"
#include "ast_typedef_name.hpp"
#include <string>




class Type_specifier
    : public Tree
{

private:
    std::string* type_spec;
    Struct_or_union_specifier* s_or_u_spec;
    Enum_specifier* enum_spec;
    Typedef_name* typedef_name;
public:
    virtual ~Type_specifier() {}
    virtual void print_python(std::ostream &dst) const;
    virtual void compile(std::ostream &dst, Context& context) const;

    Type_specifier(std::string* type_spec_in,
        Struct_or_union_specifier* s_or_u_spec_in,
        Enum_specifier* enum_spec_in,
        Typedef_name* typedef_name_in) :
        type_spec(type_spec_in),
        s_or_u_spec(s_or_u_spec_in),
        enum_spec(enum_spec_in),
        typedef_name(typedef_name_in) {}
};

#endif
