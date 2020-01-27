#ifndef ast_pointer_hpp
#define ast_pointer_hpp

#include "ast_tree.hpp"
#include "ast_type_qualifier_list.hpp"
#include <string>




class Pointer
    : public Tree
{

private:
    Type_qualifier_list* type_qual_list;
    Pointer* ptr;
public:
    virtual ~Pointer() {}
    virtual void print_python(std::ostream &dst) const{};
    virtual void compile(std::ostream &dst, Context& context) const{};

    Pointer(
        Type_qualifier_list* type_qual_list_in,
        Pointer* ptr_in) :
        type_qual_list(type_qual_list_in),
        ptr(ptr_in) {}
};

#endif
