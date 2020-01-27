#ifndef ast_parameter_type_list_hpp
#define ast_parameter_type_list_hpp

#include "ast_tree.hpp"
#include <string>


class Parameter_list;

class Parameter_type_list
    : public Tree
{

private:
    Parameter_list* param_list;
    bool ellipsis_present;
public:
    virtual ~Parameter_type_list() {}
    virtual void print_python(std::ostream &dst) const;
    virtual void compile(std::ostream &dst, Context& context) const;

    Parameter_type_list(
        Parameter_list* param_list_in,
        bool ellipsis_present_in) :
        param_list(param_list_in),
        ellipsis_present(ellipsis_present_in) {}
};

#endif
