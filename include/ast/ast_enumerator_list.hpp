#ifndef ast_enumerator_list_hpp
#define ast_enumerator_list_hpp

#include "ast_tree.hpp"
#include "ast_enumerator.hpp"
#include "ast_enumerator_list.hpp"
#include <string>




class Enumerator_list
    : public Tree
{

private:
    Enumerator* enumer;
    Enumerator_list* enum_list;
public:
    virtual ~Enumerator_list() {}
    virtual void print_python(std::ostream &dst) const{};
    virtual void compile(std::ostream &dst, Context& context) const;

    Enumerator_list(
        Enumerator* enumer_in,
        Enumerator_list* enum_list_in) :
        enumer(enumer_in),
        enum_list(enum_list_in){}
};

#endif
