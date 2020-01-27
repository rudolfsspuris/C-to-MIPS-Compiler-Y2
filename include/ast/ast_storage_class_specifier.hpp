#ifndef ast_storage_class_specifier_hpp
#define ast_storage_class_specifier_hpp

#include "ast_tree.hpp"
#include <string>




class Storage_class_specifier
    : public Tree
{

private:
    std::string* stor_class_spec;
public:
    virtual ~Storage_class_specifier() {}
    virtual void print_python(std::ostream &dst) const{};
    virtual void compile(std::ostream &dst, Context& context) const{};

    Storage_class_specifier(std::string* stor_class_spec_in) : stor_class_spec(stor_class_spec_in) {}
};

#endif
