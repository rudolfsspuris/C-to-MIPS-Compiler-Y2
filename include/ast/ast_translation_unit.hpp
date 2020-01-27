#ifndef ast_translation_unit_hpp
#define ast_translation_unit_hpp

#include "ast_tree.hpp"
#include "ast_external_declaration.hpp"




class Translation_unit
    : public Tree
{

protected:
    External_declaration* external_decl;
	Translation_unit* translation_unit;
public:
    virtual ~Translation_unit() {}
    virtual void print_python(std::ostream &dst) const;
    virtual void compile(std::ostream &dst, Context& context) const;

	Translation_unit(
        External_declaration* ext_decl_ptr,
        Translation_unit* transl_unit_ptr) :
        external_decl(ext_decl_ptr) ,
        translation_unit(transl_unit_ptr){}
};

#endif
