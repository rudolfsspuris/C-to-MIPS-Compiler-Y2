#ifndef ast_statement_list_hpp
#define ast_statement_list_hpp

#include "ast_tree.hpp"
#include "ast_statement.hpp"
#include <string>




class Statement_list
    : public Tree
{

private:
    Statement_list* stmnt_list;
    Statement* stmnt;
public:
    virtual ~Statement_list() {}
    virtual void print_python(std::ostream &dst) const;
    virtual void compile(std::ostream &dst, Context& context) const;

    Statement_list(
        Statement_list* stmnt_list_in,
        Statement* stmnt_in) :
        stmnt_list(stmnt_list_in),
        stmnt(stmnt_in) {}
};

#endif
