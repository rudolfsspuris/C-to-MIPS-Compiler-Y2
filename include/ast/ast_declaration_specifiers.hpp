#ifndef ast_declaration_specifiers_hpp
#define ast_declaration_specifiers_hpp

#include "ast_tree.hpp"
#include "ast_storage_class_specifier.hpp"
#include "ast_type_specifier.hpp"
#include "ast_type_qualifier.hpp"
#include "ast_declaration_specifiers.hpp"

class Type_specifier;

class Declaration_specifiers
    : public Tree
{

protected:
    Storage_class_specifier* stor_class_spec;
    Type_specifier* type_spec;
    Type_qualifier* type_qual;
	Declaration_specifiers* decl_spec;
public:
    virtual ~Declaration_specifiers() {}
    virtual void print_python(std::ostream &dst) const;
    virtual void compile(std::ostream &dst, Context& context) const;

    Declaration_specifiers(
        Storage_class_specifier* stor_class_spec_in,
        Type_specifier* type_spec_in,
        Type_qualifier* type_qual_in,
        Declaration_specifiers* decl_spec_in) :
        stor_class_spec(stor_class_spec_in),
        type_spec(type_spec_in),
        type_qual(type_qual_in),
        decl_spec(decl_spec_in) {}
};

#endif
