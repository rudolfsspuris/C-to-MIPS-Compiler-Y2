#include "ast.hpp"
#include "python_forward_declaration.hpp"
#include <iostream>



void Primary_expression::print_python(std::ostream &dst) const{
    if(constant != NULL){//second rule
        // std::cout<<"Primary_expression_expression if_1"<<std::endl;
        dst<<*constant;
        // std::cout<<"Primary_expression_expression if_1_END"<<std::endl;

    }
    else if(identifier!=NULL){
        // std::cout<<"Primary_expression_expression if_2"<<std::endl;
        dst<<*identifier;
    }
    else if(string_literal!=NULL){
        // std::cout<<"Primary_expression_expression if_3"<<std::endl;
        dst<<" \"" << *string_literal << "\" ";
    }
    else if(expression!=NULL){
        // std::cout<<"Primary_expression_expression if_4"<<std::endl;
        primary_parantheses++;
        dst<<"(";
        expression->print_python(dst);
        dst<<")";
        primary_parantheses--;
    }
}


void Postfix_expression::print_python(std::ostream &dst) const{
    if(prim_expr != NULL){///first rule
        // std::cout<<"Postfix_expression if_1"<<std::endl;

        prim_expr->print_python(dst);
    }
    //rule 2 not applicable
    else if(postf_expr!=NULL){//rule 3 and 4
        // std::cout<<"Postfix_expression if_2"<<std::endl;
        for( int i = 0; i<indent_count; i++) { dst << "\t"; }
        postf_expr->print_python(dst);
        dst<<"(";
        in_function=true;
        if(arg_expr_list!=NULL){
            arg_expr_list->print_python(dst);
        }
        dst<<")";
        in_function=false;
    }
    //other rules not applicable to python?
}


void Unary_expression::print_python(std::ostream &dst) const{
    if(postf_expr != NULL){//rule 1
        // std::cout<<"Unary_expression if_1"<<std::endl;
        postf_expr->print_python(dst);
    }
    else if(unary_op==NULL){//rule 3
        // std::cout<<"Unary_expression if_2"<<std::endl;
        std::cout<<"Should there be a unary operator linked?"<<std::endl;
    }
    else{
        // std::cout<<"Unary_expression if_3"<<std::endl;
        dst<<" "<<*unary_op << " ";
        if(cast_expr!=NULL){
            cast_expr->print_python(dst);
        }
    }
    //should not need any more rules for python?
}


void Cast_expression::print_python(std::ostream &dst) const{
    if(unary_expr !=NULL){///first rule
        unary_expr->print_python(dst);
    }
    //no casting in python translator needed
}


void Multiplicative_expression::print_python(std::ostream &dst) const{
    if(cast_expr!=NULL && mult_expr==NULL){//first rule
        cast_expr->print_python(dst);
    }
    else if(cast_expr!=NULL && mult_expr!=NULL){//first rule
        mult_expr->print_python(dst);
        dst<< *op;
        cast_expr->print_python(dst);
    }
}


void Additive_expression::print_python(std::ostream &dst) const{
    if(mult_expr!=NULL && additive_expr==NULL){///first rule
        mult_expr->print_python(dst);
    }
    else if(mult_expr!=NULL && additive_expr!=NULL && op!=NULL){///first rule
        additive_expr->print_python(dst);
        dst<<" " << *op << " ";
        mult_expr->print_python(dst);
    }
}


void Shift_expression::print_python(std::ostream &dst) const{
    if(additive_expr!=NULL && shift_expr==NULL){///only first rule needed
        additive_expr->print_python(dst);
    }
}


void Relational_expression::print_python(std::ostream &dst) const{
    if(shift_expr!=NULL && relat_expr==NULL){///first rule
        shift_expr->print_python(dst);
    }
    else if(shift_expr!=NULL && relat_expr!=NULL && op!=NULL){///rule 2 & 3
        relat_expr->print_python(dst);
        dst<<" " << *op << " ";
        shift_expr->print_python(dst);
    }
}


void Equality_expression::print_python(std::ostream &dst) const{
    if(relat_expr!=NULL && equality_expr==NULL){///only first rule needed
        relat_expr->print_python(dst);
    }
    else if(relat_expr!=NULL && equality_expr!=NULL && op!=NULL){///only first rule needed
        equality_expr->print_python(dst);
        dst<<" " << *op << " ";
        relat_expr->print_python(dst);
    }
}

void And_expression::print_python(std::ostream &dst) const{
    if(equality_expr!=NULL && and_expr==NULL){///only first rule needed, no & operator
        equality_expr->print_python(dst);
    }
}


void Exclusive_or_expression::print_python(std::ostream &dst) const{
    if(and_expr!=NULL && exclusive_or_expr==NULL){///only first rule needed, no ^ in python
        and_expr->print_python(dst);
    }
}


void Inclusive_or_expression::print_python(std::ostream &dst) const{
    if(exclusive_or_expr!=NULL && inclusive_or_expr==NULL){///only first rule needed for python
        exclusive_or_expr->print_python(dst);
    }
}


void Logical_and_expression::print_python(std::ostream &dst) const{
    if(inclusive_or_expr!=NULL && logical_and_expr == NULL){///first rule
        // std::cout<<"Logical_and_expression if_1"<<std::endl;
        inclusive_or_expr->print_python(dst);
    }
    else if(inclusive_or_expr!=NULL && logical_and_expr != NULL) {///first rule
        logical_and_expr->print_python(dst);
        dst<<" and ";
        inclusive_or_expr->print_python(dst);
    }
}


void Logical_or_expression::print_python(std::ostream &dst) const{
    if(logical_and_expr!=NULL && logical_or_expr==NULL){///first rule
        // std::cout<<"Logical_or_expression if_1"<<std::endl;
        logical_and_expr->print_python(dst);
    }
    else if(logical_and_expr!=NULL && logical_or_expr!=NULL){ //second rule
        // std::cout<<"Logical_or_expression if_2"<<std::endl;
        logical_or_expr->print_python(dst);
        dst<<" or ";
        logical_and_expr->print_python(dst);
    }
}

void Conditional_expression::print_python(std::ostream &dst) const{
    if(logic_or_expr!=NULL){///only first rule needed for python
        // std::cout<<"Conditional_expression if_1"<<std::endl;
        logic_or_expr->print_python(dst);
    }
}


void Initializer::print_python(std::ostream &dst) const{
    if(assignment_expr!=NULL){///only first rule
        assignment_expr->print_python(dst);
    }
}



void Assignment_expression::print_python(std::ostream &dst) const{

    if(cond_expr!=NULL){
        // std::cout<<"Assignment_expression if_1"<<std::endl;
        cond_expr->print_python(dst);
    }
    else if(unary_expr!=NULL){
        // std::cout<<"Assignment_expression if_2"<<std::endl;
        for(int i = 0; i<indent_count; i++){dst << "\t";}
        unary_expr->print_python(dst);

        if(assign_op == NULL){
            // std::cout<<"Assignment_expression if_21"<<std::endl;
            std::cout<<"Assignment operators are not linked to the Assignment_expression node, RIP"<<std::endl;
        }
        else{
            // std::cout<<"Assignment_expression if_22, ASSIGN_OP = "<<*assign_op<<std::endl;
            dst<<" "<<*assign_op<< " ";
        }
        if(assign_expr!=NULL){
            // std::cout<<"Assignment_expression if_23"<<std::endl;
            assign_expr->print_python(dst);

        }
    }
    if(!in_iteration && !in_function && !newline_selective && primary_parantheses==0){
        dst<<std::endl;
    }


}


void Init_declarator::print_python(std::ostream &dst) const{
    for( int i = 0; i<indent_count; i++) { dst << "\t"; }
    if(declarator!=NULL && initializer!=NULL){/// to be finished with declarator

        declarator->print_python(dst);
        dst<<" = ";
        initializer->print_python(dst);
        dst<<std::endl;
    }
    else if(declarator!=NULL && initializer==NULL){
        declarator->print_python(dst);
        dst<<" = 0";
        dst<<std::endl;
    }
}


void Init_declarator_list::print_python(std::ostream &dst) const{
    if(init_decl_list==NULL && init_decl!=NULL){
        init_decl->print_python(dst);
    }
    else if(init_decl_list!=NULL){
        init_decl_list->print_python(dst);
        init_decl->print_python(dst);
    }
}


void Direct_declarator::print_python(std::ostream &dst) const{ //global and other variebles handeled needed aghhh
    switch(parse_rule_followed){
        case 1:
            // std::cout<<"Direct_declarator switch_1"<<std::endl;
            // std::cout<<"HEEEERE: " <<*identifier<<std::endl;
            if(indent_count==0&&function==false){
                global_variables.push_back(*identifier);
            }
            dst<<*identifier;
            break;
        case 2:
            // std::cout<<"Direct_declarator switch_1"<<std::endl;
            dst<<"(";
            decl->print_python(dst);
            dst<<"):"<<"\n";
            function = false;

            break;
        case 5:
            // std::cout<<"Direct_declarator switch_5"<<std::endl;
            direct_decl->print_python(dst);
            dst<<"(";
            param_type_list->print_python(dst);
            dst<<"):"<<"\n";
            function = false;

            break;
        case 6:
            // std::cout<<"Direct_declarator switch_6"<<std::endl;
            direct_decl->print_python(dst);
            dst<<"(";
            identif_list->print_python(dst);
            dst<<"):"<<"\n";
            function = false;

            break;
        case 7:
            // std::cout<<"Direct_declarator switch_7"<<std::endl;
            direct_decl->print_python(dst);
            dst<<"():"<<"\n";
            function = false;

            break;
    }
}


void Declarator::print_python(std::ostream &dst) const{
    if(pointer==NULL){///second rule
        dir_decl->print_python(dst);
    }
    //first rule not needed
}


void Declaration::print_python(std::ostream &dst) const{
    if(init_decl_list!=NULL){///only the initilised case
        // decl_spec->print_python(dst);

        init_decl_list->print_python(dst);
    }
    else if(init_decl_list==NULL){

        for( int i = 0; i<indent_count; i++) { dst << "\t"; }

        // decl_spec->print_python(dst);
        //nothing to do here

    }
}


void Type_specifier::print_python(std::ostream &dst) const{
//     if(type_spec!=NULL){///last few rules not implemented
//         std::cout<<"4a"<<std::endl;
//
//
//     }
}


void Declaration_specifiers::print_python(std::ostream &dst) const{
//     if(type_spec!=NULL){///only rule 3 and 4 implemented
//         std::cout<<"3a"<<std::endl;//only gets here when variable declared, not when function declared
//         //nothing to do here
//     }
//     if(decl_spec!=NULL){
//         std::cout<<"3b"<<std::endl;
//         //nothing to do here
//         // decl_spec->print_python(dst);
//     }
}


void Function_definition::print_python(std::ostream &dst) const{
    switch(parse_rule_followed){///case 1 and 3 support K&R style, not to be implemented
        // case 1:///decl_list might not need to be implemented in our compiler
        //     decl_spec->print_python(dst);
        //     decl->print_python(dst);
        //     decl_list->print_python(dst);
        //     compound_stmnt->print_python(dst);
        //     break;
        case 2: ///function int abc(){sflkdsjf}

            // decl_spec->print_python(dst);//not needed because function only has return type int and void
            function = true;
            dst<<"def ";
            decl->print_python(dst);

            indent_count++;
            for(int i = 0; i<global_variables.size(); i++){
                for( int i(0); i<indent_count; i++) { dst << "\t"; }
                dst<<"global "<<global_variables[i]<<"\n";
            }
            indent_count--;

            compound_stmnt->print_python(dst);
            break;
        // case 3:///decl_list might not need to be implemented in our compiler
        //     decl->print_python(dst);
        //     decl_list->print_python(dst);
        //     compound_stmnt->print_python(dst);
        //     break;
        case 4: ///function abc(){saldkfjsdflk}
            function = true;
            decl->print_python(dst);

            indent_count++;
            for(int i = 0; i<global_variables.size(); i++){
                for( int i(0); i<indent_count; i++) { dst << "\t"; }
                dst<<"global "<<global_variables[i]<<"\n";
            }
            indent_count--;

            compound_stmnt->print_python(dst);
            break;

    }
}


void External_declaration::print_python(std::ostream &dst) const{
    if(decl!=NULL){

        decl->print_python(dst);
        dst<<std::endl;
    }
    else if(funct_def!=NULL){

        funct_def->print_python(dst);
        dst<<std::endl;
    }
}



void Translation_unit::print_python(std::ostream &dst) const{

    if(external_decl!=NULL&&translation_unit==NULL){
        external_decl->print_python(dst);
    }
    else if(external_decl!=NULL && translation_unit!=NULL){
        translation_unit->print_python(dst);
        external_decl->print_python(dst);
    }
}


void Iteration_statement::print_python(std::ostream& dst) const{
    if(*keyword == "while" ){
        if(statement != NULL && expr != NULL){
            in_iteration = true;
            dst<<std::endl;

            for( int i = 0; i<indent_count; i++) { dst << "\t"; }

			dst << "while(";
			expr->print_python(dst);
			dst << "):" << std::endl;
            in_iteration = false;

            indent_count++;
            dst << std::endl;
            statement->print_python(dst);
            dst << std::endl;
            indent_count--;
	    }
    }
}


void Statement_list::print_python(std::ostream& dst) const{
    if(stmnt_list!=NULL && stmnt!=NULL){
        // std::cout<<"Statement_list if_1"<<std::endl;
        stmnt_list->print_python(dst);
        stmnt->print_python(dst);
    }
    else if(stmnt_list==NULL){
        if(stmnt!=NULL){
            // std::cout<<"Statement_list if_2"<<std::endl;
            stmnt->print_python(dst);
        }
        else{
            // std::cout<<"Statement_list if_3"<<std::endl;
            for(int i = 0; i<indent_count; i++){dst << "\t";}
            dst<<"pass" << std::endl;
        }
    }
}


void Statement::print_python(std::ostream& dst) const{
    // std::cout<<"Statement"<<std::endl;
    if( labeled_stmnt != NULL ) {
        labeled_stmnt->print_python(dst);
        // std::cout<<"Statement if_1"<<std::endl;
    }

    else if( compound_stmnt != NULL ) {
        // std::cout<<"Statement if_2"<<std::endl;
        compound_stmnt->print_python(dst);
    }

    else if( expression_stmnt != NULL ) {
        // std::cout<<"Statement if_3"<<std::endl;
        expression_stmnt->print_python(dst);
    }

    else if( selection_stmnt != NULL ) {
        // std::cout<<"Statement if_4"<<std::endl;
        selection_stmnt->print_python(dst);
    }

    else if( iteration_stmnt != NULL ) {
        // std::cout<<"Statement if_5"<<std::endl;
        iteration_stmnt->print_python(dst);
    }

    else if( jump_stmnt != NULL ) {
        // std::cout<<"Statement if_6"<<std::endl;
        jump_stmnt->print_python(dst);
    }

}


void Compound_statement::print_python(std::ostream& dst) const{

    indent_count++;

    if(stmnt_list == NULL && decl_list == NULL) {

        for( int i(0); i<indent_count; i++) { dst << "\t"; }
        dst << "pass" << std::endl;
    }
    else if(stmnt_list != NULL && decl_list != NULL) {

        decl_list->print_python(dst);
        stmnt_list->print_python(dst);
    }
    else if(stmnt_list == NULL && decl_list != NULL) {

        decl_list->print_python(dst);
    }
    else if(stmnt_list != NULL && decl_list == NULL) {

        stmnt_list->print_python(dst);
    }
    indent_count--;

}


void Selection_statement::print_python(std::ostream& dst) const{

    if(!switch_stmnt){
        if(else_statement == NULL){

            newline_selective = true;

            for(int i = 0; i<indent_count; i++){dst << "\t";}
            dst<<"if(";
            expr->print_python(dst);
            dst<<"):"<<std::endl;

            newline_selective = false;
            indent_count++;
            dst << std::endl;
            if_statement->print_python(dst);
            indent_count--;
        }
        else{
            newline_selective = true;

            for(int i = 0; i<indent_count; i++){dst << "\t";}
            dst<<"if(";
            expr->print_python(dst);
            dst<<"):"<<std::endl;

            newline_selective = false;

            indent_count++;
            dst << std::endl;
            if_statement->print_python(dst);
            indent_count--;

            for(int i = 0; i<indent_count; i++){dst << "\t";}
            dst<<"else:" <<std::endl;

            indent_count++;
            dst << std::endl;
            else_statement->print_python(dst);
            indent_count--;
        }
    }

}

void Jump_statement::print_python(std::ostream& dst) const{
    for(int i = 0; i<indent_count; i++){dst << "\t";}
    if(*keyword == "return"){
        // std::cout<<"Jump if_1"<<std::endl;
        dst << *keyword << " ";
        if(expr != NULL){
            // std::cout<<"Jump if_11"<<std::endl;
            expr->print_python(dst);
        }
    }

}

void Expression::print_python(std::ostream& dst) const{
    if(assign_expr!=NULL){
        // std::cout<<"Expression if_1"<<std::endl;
        assign_expr->print_python(dst);
    }
}

void Parameter_type_list::print_python(std::ostream& dst) const{
    if(param_list!=NULL){               //rule 1. Rule 2 not needed
        param_list->print_python(dst);
    }
}

void Parameter_list::print_python(std::ostream& dst) const{
    if(param_list!=NULL){
        param_list->print_python(dst);
        dst<<", ";
    }
    if(param_decl!=NULL){
        param_decl->print_python(dst);
    }
}

void Parameter_declaration::print_python(std::ostream& dst) const{
    //dont need anything else
    if(declarator!=NULL){
        declarator->print_python(dst);
    }
}

void Argument_expression_list::print_python(std::ostream& dst) const{
    if(arg_expr_list!=NULL){
        arg_expr_list->print_python(dst);
        dst<<", ";
    }
    if(assign_expr!=NULL){
        assign_expr->print_python(dst);
    }

}

void Expression_statement::print_python(std::ostream& dst) const{
    if(expr!=NULL){
        expr->print_python(dst);
    }
}

void Declaration_list::print_python(std::ostream& dst) const{
    if(decl_list!=NULL){
        decl_list->print_python(dst);
    }
    if(decl!=NULL){
        decl->print_python(dst);
    }

}
