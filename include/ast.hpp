#ifndef ast_hpp
#define ast_hpp



#include "ast/ast_primary_expression.hpp"
#include "ast/ast_argument_expression_list.hpp"
#include "ast/ast_pointer.hpp"
#include "ast/ast_declarator.hpp"
#include "ast/ast_function_definition.hpp"
#include "ast/ast_external_declaration.hpp"
#include "ast/ast_translation_unit.hpp"
#include "ast/ast_tree.hpp"
#include "ast/ast_abstract_declarator.hpp"
#include "ast/ast_additive_expression.hpp"
#include "ast/ast_and_expression.hpp"
#include "ast/ast_assignment_expression.hpp"
#include "ast/ast_cast_expression.hpp"
#include "ast/ast_compound_statement.hpp"
#include "ast/ast_conditional_expression.hpp"
#include "ast/ast_constant_expression.hpp"
#include "ast/ast_declaration_list.hpp"
#include "ast/ast_declaration_specifiers.hpp"
#include "ast/ast_declaration.hpp"
#include "ast/ast_direct_abstract_declarator.hpp"
#include "ast/ast_direct_declarator.hpp"
#include "ast/ast_enum_specifier.hpp"
#include "ast/ast_enumeration_constant.hpp"
#include "ast/ast_enumerator_list.hpp"
#include "ast/ast_enumerator.hpp"
#include "ast/ast_equality_expression.hpp"
#include "ast/ast_exclusive_or_expression.hpp"
#include "ast/ast_expression_statement.hpp"
#include "ast/ast_expression.hpp"
#include "ast/ast_identifier_list.hpp"
#include "ast/ast_inclusive_or_expression.hpp"
#include "ast/ast_init_declarator_list.hpp"
#include "ast/ast_init_declarator.hpp"
#include "ast/ast_initializer_list.hpp"
#include "ast/ast_initializer.hpp"
#include "ast/ast_iteration_statement.hpp"
#include "ast/ast_jump_statement.hpp"
#include "ast/ast_labeled_statement.hpp"
#include "ast/ast_logical_and_expression.hpp"
#include "ast/ast_logical_or_expression.hpp"
#include "ast/ast_multiplicative_expression.hpp"
// #include "ast/ast_operators.hpp"
#include "ast/ast_parameter_declaration.hpp"
#include "ast/ast_parameter_list.hpp"
#include "ast/ast_parameter_type_list.hpp"
#include "ast/ast_postfix_expression.hpp"
#include "ast/ast_relational_expression.hpp"
#include "ast/ast_selection_statement.hpp"
#include "ast/ast_shift_expression.hpp"
#include "ast/ast_specifier_qualifier_list.hpp"
#include "ast/ast_statement_list.hpp"
#include "ast/ast_statement.hpp"
#include "ast/ast_storage_class_specifier.hpp"
#include "ast/ast_struct_declaration_list.hpp"
#include "ast/ast_struct_declaration.hpp"
#include "ast/ast_struct_declarator_list.hpp"
#include "ast/ast_struct_declarator.hpp"
#include "ast/ast_struct_or_union_specifier.hpp"
#include "ast/ast_translation_unit.hpp"
#include "ast/ast_type_name.hpp"
#include "ast/ast_type_qualifier_list.hpp"
#include "ast/ast_type_qualifier.hpp"
#include "ast/ast_type_specifier.hpp"
#include "ast/ast_typedef_name.hpp"
#include "ast/ast_unary_expression.hpp"

extern const Translation_unit *parseAST(FILE* src);



#endif
