#ifndef ast_jump_statement_hpp
#define ast_jump_statement_hpp

#include "ast_tree.hpp"
#include "ast_expression.hpp"
#include <string>




class Jump_statement
    : public Tree
{

private:
    Expression* expr;
    std::string* keyword;

public:
    virtual ~Jump_statement() {}
    virtual void print_python(std::ostream &dst) const;
    virtual void compile(std::ostream &dst, Context& context) const;

    Jump_statement(
        Expression* expr_in,
        std::string* keyword_in) :
        expr(expr_in),
        keyword(keyword_in) {}
};

#endif
