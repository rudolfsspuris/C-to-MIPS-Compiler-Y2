#include "ast.hpp"
#include <iostream>
#include "mips_forward_declaration.hpp"
#include <string>

// "///" for action needed
// "//" for comment to help understanding
static int makeNameUnq=0;

static std::string makeName(std::string base)
{
    return "_"+base+"_"+std::to_string(makeNameUnq++);
}


void Translation_unit::compile(std::ostream &dst, Context& context) const{
    dst<<"#translation_unit\n";

    if(translation_unit!=NULL){
        translation_unit->compile(dst, context);
        external_decl->compile(dst, context);
    }
    else if(translation_unit==NULL){
        external_decl->compile(dst, context);
    }
}


void External_declaration::compile(std::ostream &dst, Context& context) const{
    dst<<"#External_declaration\n";
    if(funct_def!=NULL){
        funct_def->compile(dst, context);
    }
    else if(decl!=NULL){
        context.external_decl = true;
        decl->compile(dst, context);
        context.external_decl = false;

    }
}


void Function_definition::compile(std::ostream &dst, Context& context) const{

    switch(parse_rule_followed){///case 1 and 3 support K&R style, not to be implemented
        case 2: ///function int abc(){sflkdsjf}
            // function = true;
            //Change Context
            if(context.current_scope.size()<context.last_scope.size()){
                std::vector<int> tmp = context.current_scope;
                context.current_scope.push_back(context.last_scope.back() + 1);
                context.last_scope = tmp;
            }
            else{
                std::vector<int> tmp = context.current_scope;
                context.current_scope.push_back(1);
                context.last_scope = tmp;
            }

            context.current_stack_offset = 24;
            //Count minimum number of variables for memory allocation
            context.declaration_count = 0;
            context.stack_counting = true;
            if(compound_stmnt!=NULL){
                compound_stmnt->compile(dst, context);
            }
            decl->compile(dst, context);
            context.stack_counting = false;
            context.first_var_in_stack = true;

            context.function_declaration = true;
            decl_spec->compile(dst, context);
            context.decl_to_reg = 4;
            decl->compile(dst, context);
            context.function_declaration = false;
            //Setup stack
            // context.element_position = context.element_position - (context.declaration_count*context.largest_decl+32);
            // dst<<"\taddiu\t$sp,$sp,-"<<(context.declaration_count*context.largest_decl+32)<<"\n";
            // dst<<"\tsw\t$31,"<<(context.declaration_count*context.largest_decl+28)<<"($sp)\n";
            // dst<<"\tsw\t$fp,"<<(context.declaration_count*context.largest_decl+24)<<"($sp)\n";
            //
            // // context.current_fp = context.element_position;
            // dst<<"\tmove\t$fp,$sp\n";


            context.function_to_compound = true;

            compound_stmnt->compile(dst, context);

            context.function_to_compound = false;
            //Change Context

            context.last_scope = context.current_scope;
            context.current_scope.pop_back();


            //Deallocate stack
            dst<<"\tmove\t$sp,$fp\n";
            dst<<"\tlw\t$31,"<<(context.declaration_count*context.largest_decl+28)<<"($sp)\n";
            dst<<"\tlw\t$fp,"<<(context.declaration_count*context.largest_decl+24)<<"($sp)\n";
            dst<<"\taddiu\t$sp,$sp,"<<(context.declaration_count*context.largest_decl+32)<<"\n";
            dst<<"\tjr\t$31"<<"\n";
            dst<<"\tnop\n";


            break;



        //case 4: ///function abc(){saldkfjsdflk} ///this defaults to return type int!!!!!!!!!
        // function = true;


    }
    // dst<<".end "<<*identifier<<"\n";

}


void Parameter_type_list::compile(std::ostream &dst, Context &context) const{
    if(param_list!=NULL){
        param_list->compile(dst, context);
    }
}

void Parameter_list::compile(std::ostream &dst, Context &context) const{
    if(context.stack_counting){
        if(param_list!=NULL){
            param_list->compile(dst, context);
        }
        context.declaration_count++;

    }
    else{
        if(param_list!=NULL){
            param_list->compile(dst, context);
        }
        if(param_decl!=NULL){
            param_decl->compile(dst, context);
            context.decl_to_reg++;
        }
    }
}

void Parameter_declaration::compile(std::ostream &dst, Context &context) const{
    if(declarator!=NULL){
        declarator->compile(dst, context);
    }
}


void Expression_statement::compile(std::ostream &dst, Context &context) const{
    if(context.stack_counting){;}
    else{

        if(expr!=NULL){
            expr->compile(dst, context);
            //popping one from stack after every expression
            dst<<"#expression_statement\n";
            dst<<"\tlw\t$2,($sp)\n";
            dst<<"\taddiu\t$sp,$sp,"<<context.largest_decl<<"\n";
        }
    }
}

void Compound_statement::compile(std::ostream &dst, Context &context) const{
    //Change Context

    if(context.stack_counting){
        if(decl_list!=NULL){
            decl_list->compile(dst, context);
        }
        if(stmnt_list!=NULL){
            stmnt_list->compile(dst, context);
        }
    }




    else{
        //Change context
        if(!context.function_to_compound){
            if(context.current_scope.size()<context.last_scope.size()){
                std::vector<int> tmp = context.current_scope;
                context.current_scope.push_back(context.last_scope.back() + 1);
                context.last_scope = tmp;
            }
            else{
                std::vector<int> tmp = context.current_scope;
                context.current_scope.push_back(1);
                context.last_scope = tmp;
            }
        }
        bool function_to_compound_for_dealoc = context.function_to_compound;
        context.function_to_compound = false;


        //////////////////////////////////////////
        if(decl_list!=NULL){
            decl_list->compile(dst, context);
        }
        if(stmnt_list!=NULL){
            stmnt_list->compile(dst, context);
        }
        /////////////////////////////////////////////

        //Change Context
        if(!function_to_compound_for_dealoc){
            context.last_scope = context.current_scope;
            context.current_scope.pop_back();
        }

    }

}

void Declaration_list::compile(std::ostream &dst, Context &context) const{



    if(context.stack_counting){
        decl->compile(dst, context);
        if(decl_list!=NULL){
            decl_list->compile(dst, context);
        }
    }



    else{
        if(decl_list!=NULL){
            decl_list->compile(dst, context);
            decl->compile(dst, context);
        }
        else{
            decl->compile(dst, context);
        }
    }
}


void Declaration::compile(std::ostream &dst, Context& context) const{
    dst<<"#Declaration\n";

    if(init_decl_list!=NULL){///only the initilised case
        // decl_spec->compile(dst);///ignore this for now because only int

        init_decl_list->compile(dst, context);
    }
    else if(init_decl_list==NULL){


        // decl_spec->print_python(dst);
        //nothing to do here

    }
}


void Init_declarator_list::compile(std::ostream &dst, Context& context) const{
    if(init_decl_list==NULL && init_decl!=NULL){
        init_decl->compile(dst, context);
    }
    else if(init_decl_list!=NULL){
        init_decl_list->compile(dst, context);
        init_decl->compile(dst, context);
    }
}


void Init_declarator::compile(std::ostream &dst, Context& context) const{



    if(context.stack_counting){
        if(declarator!=NULL){
            declarator->compile(dst, context);
        }
    }

    else{
        if(declarator!=NULL && initializer==NULL){
            if(context.external_decl){
                declarator->compile(dst, context);
                if(!context.got_to_rule_5){
                    dst<<"\t.globl "<<context.tmp.name<<"\n";
                    dst<<"\t.data\n";
                    dst<<"\t.align  2\n";
                    dst<<"\t.type "<<context.tmp.name<<" @object\n";
                    context.tmp.scope = context.current_scope;
                    context.variables.push_back(context.tmp);
                }
                context.got_to_rule_5 = false;

            }
            else{
                declarator->compile(dst, context);
                context.variables.push_back(context.tmp);
            }
        }
        if(declarator!=NULL&&initializer!=NULL){
            if(context.external_decl){
                if(!context.got_to_rule_5){
                    declarator->compile(dst, context);
                    dst<<"\t.globl "<<context.tmp.name<<"\n";
                    dst<<"\t.data\n";
                    dst<<"\t.align  2\n";
                    dst<<"\t.type "<<context.tmp.name<<" @object\n";
                    dst<<context.tmp.name<<": \n\t .word "<<initializer->evaluate(context)<<"\n";
                    context.tmp.scope = context.current_scope;
                    context.variables.push_back(context.tmp);
                }
                context.got_to_rule_5 = false;


            }
            else{
                declarator->compile(dst, context);
                context.variables.push_back(context.tmp);
                initializer->compile(dst, context);

                dst<<"\tlw\t$2,($sp)\n";
                dst<<"\taddiu\t$sp,$sp,"<<context.largest_decl<<"\n";

                dst<<"\tsw\t$2,"<<context.current_stack_offset-8<<"($fp)"<<std::endl;
            }
        }
    }
}


void Declarator::compile(std::ostream &dst, Context& context) const{
    if(pointer==NULL&&dir_decl!=NULL){///pointer not implemented yet
        dir_decl->compile(dst, context);
    }
}




void Initializer::compile(std::ostream &dst, Context& context) const{
    //returns to $2
    if(assignment_expr!=NULL){
        assignment_expr->compile(dst, context);
    }
}


void Declaration_specifiers::compile(std::ostream &dst, Context& context) const{
    if(type_spec!=NULL){///only rule 3 and 4 implemented
        // if((*type_spec)=="int"){
        //     ///do something in MIPS that corresponds to int return type
        // }
        type_spec->compile(dst,context);


    }
    else if(type_qual!=NULL){}///to support const and volatile, probably won't implement
    else if(stor_class_spec!=NULL){}///to suppport static and stuff, probably won't implement

    if(decl_spec!=NULL){

        decl_spec->compile(dst, context);
    }

}


void Statement::compile(std::ostream &dst, Context& context) const{
    if(context.stack_counting){
        if( labeled_stmnt != NULL ) {
            labeled_stmnt->compile(dst, context);
            // std::cout<<"Statement if_1"<<std::endl;
        }

        else if( compound_stmnt != NULL ) {
            // std::cout<<"Statement if_2"<<std::endl;
            compound_stmnt->compile(dst, context);
        }

        else if( expression_stmnt != NULL ) {
            // std::cout<<"Statement if_3"<<std::endl;
            expression_stmnt->compile(dst, context);
        }

        else if( selection_stmnt != NULL ) {
            // std::cout<<"Statement if_4"<<std::endl;
            selection_stmnt->compile(dst, context);
        }

        else if( iteration_stmnt != NULL ) {
            // std::cout<<"Statement if_5"<<std::endl;
            iteration_stmnt->compile(dst, context);
        }

        else if( jump_stmnt != NULL ) {
            // std::cout<<"Statement if_6"<<std::endl;
            jump_stmnt->compile(dst, context);
        }
    }


    else{
        dst<<"#statement\n";
        if( labeled_stmnt != NULL ) {
            dst<<"#labeled_stmnt\n";
            labeled_stmnt->compile(dst, context);
            // std::cout<<"Statement if_1"<<std::endl;
        }

        else if( compound_stmnt != NULL ) {
            dst<<"#compound_stmnt\n";
            // std::cout<<"Statement if_2"<<std::endl;
            compound_stmnt->compile(dst, context);
        }

        else if( expression_stmnt != NULL ) {
            dst<<"#expression_stmnt\n";
            // std::cout<<"Statement if_3"<<std::endl;
            expression_stmnt->compile(dst, context);
        }

        else if( selection_stmnt != NULL ) {
            dst<<"#selection_stmnt\n";
            // std::cout<<"Statement if_4"<<std::endl;
            selection_stmnt->compile(dst, context);
        }

        else if( iteration_stmnt != NULL ) {
            dst<<"#iteration_stmnt\n";
            // std::cout<<"Statement if_5"<<std::endl;
            iteration_stmnt->compile(dst, context);
        }

        else if( jump_stmnt != NULL ) {
            dst<<"#jump_stmnt\n";
            // std::cout<<"Statement if_6"<<std::endl;
            jump_stmnt->compile(dst, context);
        }
    }
}

void Statement_list::compile(std::ostream &dst, Context& context) const{

    if(context.stack_counting){
        if(stmnt_list!=NULL && stmnt!=NULL){
            stmnt_list->compile(dst, context);
            stmnt->compile(dst, context);
        }
        else if(stmnt_list==NULL && stmnt!=NULL){
            stmnt->compile(dst, context);
        }
    }
    else{
        dst<<"#statement_list\n";
        if(stmnt_list!=NULL && stmnt!=NULL){
            stmnt_list->compile(dst, context);
            stmnt->compile(dst, context);
        }
        else if(stmnt_list==NULL && stmnt!=NULL){
            stmnt->compile(dst, context);
        }
    }
}

void Jump_statement::compile(std::ostream &dst, Context& context) const{
    if(context.stack_counting){;}
    else{
        if(*keyword=="return"){

            if(context.stack_counting){
                if(expr!=NULL){
                    expr->compile(dst, context);
                }
            }

            else if(expr!=NULL){


                expr->compile(dst, context);
                dst<<"#jump_statement with return value\n";

                dst<<"\tlw\t$2,($sp)\n";
                dst<<"\taddiu\t$sp,$sp,"<<context.largest_decl<<"\n";

                // //Change Context
                // context.last_scope = context.current_scope;
                // context.current_scope.pop_back();


                //Deallocate stack
                dst<<"\tmove\t$sp,$fp\n";
                dst<<"\tlw\t$fp,"<<(context.declaration_count*context.largest_decl+24)<<"($sp)\n";
                dst<<"\tlw\t$31,"<<(context.declaration_count*context.largest_decl+28)<<"($sp)\n";
                dst<<"\taddiu\t$sp,$sp,"<<(context.declaration_count*context.largest_decl+32)<<"\n";
                dst<<"\tjr\t$31"<<"\n";
                dst<<"\tnop\n";

            }
            else{

                dst<<"#jump_statement without return value\n";

                // // Change Context
                // context.last_scope = context.current_scope;
                // context.current_scope.pop_back();


                //Deallocate stack
                dst<<"\tmove\t$sp,$fp\n";
                dst<<"\tlw\t$fp,"<<(context.declaration_count*context.largest_decl+24)<<"($sp)\n";
                dst<<"\tlw\t$31,"<<(context.declaration_count*context.largest_decl+28)<<"($sp)\n";
                dst<<"\taddiu\t$sp,$sp,"<<(context.declaration_count*context.largest_decl+32)<<"\n";
                dst<<"\tjr\t$31"<<"\n";
                dst<<"\tnop\n";

            }

        }else if (*keyword=="break"){
             dst<<"\tj "<<"$END"<<context.break_scope.back()<<"\n";

        }else if (*keyword=="continue"){
            dst<<"\tj "<<"$CONTINUESTART"<<context.break_scope.back()<<"\n";
        }

    }

}

void Selection_statement::compile(std::ostream &dst, Context& context) const{
    if(context.stack_counting){
        if(if_statement!=NULL){
            if_statement->compile(dst, context);
        }
        if(else_statement!=NULL){
            if_statement->compile(dst, context);
        }
    }
    else{
        if(!switch_stmnt){
            if(else_statement==NULL && expr!=NULL && if_statement!=NULL){
                std::string branch=makeName("if");

                expr->compile(dst, context);
                dst<<"\tlw\t$8,($sp)\n";
                dst<<"\taddiu\t$sp,$sp,"<<"8"<<"\n";


                dst<<"\tbeq\t$8,$0,"<<"$FALSE"<<branch<<"\n";
                dst<<"\tnop\n";

                //Change Context
                if(context.current_scope.size()<context.last_scope.size()){
                    std::vector<int> tmp = context.current_scope;
                    context.current_scope.push_back(context.last_scope.back() + 1);
                    context.last_scope = tmp;
                }
                else{
                    std::vector<int> tmp = context.current_scope;
                    context.current_scope.push_back(1);
                    context.last_scope = tmp;
                }

                if_statement->compile(dst, context);


                //Change Context
                context.last_scope = context.current_scope;
                context.current_scope.pop_back();

                dst<<"$FALSE"<<branch<<":\n";
            }
            else if(else_statement!=NULL && expr!=NULL && if_statement!=NULL){
                std::string branch=makeName("if");

                expr->compile(dst, context);
                dst<<"\tlw\t$8,($sp)\n";
                dst<<"\taddiu\t$sp,$sp,"<<"8"<<"\n";

                dst<<"\tbne\t$8,$0,"<<"$TRUE"<<branch<<"\n";
                dst<<"nop\n";

                //Change Context
                if(context.current_scope.size()<context.last_scope.size()){
                    std::vector<int> tmp = context.current_scope;
                    context.current_scope.push_back(context.last_scope.back() + 1);
                    context.last_scope = tmp;
                }
                else{
                    std::vector<int> tmp = context.current_scope;
                    context.current_scope.push_back(1);
                    context.last_scope = tmp;
                }

                else_statement->compile(dst, context);

                //Change Context
                context.last_scope = context.current_scope;
                context.current_scope.pop_back();


                dst<<"\tj "<<"$END"<<branch<<"\n";
                dst<<"nop\n";
                dst<<"$TRUE"<<branch<<":\n";

                //Change Context
                if(context.current_scope.size()<context.last_scope.size()){
                    std::vector<int> tmp = context.current_scope;
                    context.current_scope.push_back(context.last_scope.back() + 1);
                    context.last_scope = tmp;
                }
                else{
                    std::vector<int> tmp = context.current_scope;
                    context.current_scope.push_back(1);
                    context.last_scope = tmp;
                }

                if_statement->compile(dst, context);

                //Change Context
                context.last_scope = context.current_scope;
                context.current_scope.pop_back();

                dst<<"$END"<<branch<<":\n";
            }
        }
        else{

        }
    }
}


void Iteration_statement::compile(std::ostream &dst, Context& context) const{
    if(context.stack_counting){
        if(statement!=NULL){
            statement->compile(dst, context);
        }
    }

    else{
        if(*keyword=="while"){
            std::string branch=makeName("while");
            context.break_scope.push_back(branch);
            dst<<"$START"<<branch<<":\n";

            expr->compile(dst, context);

            dst<<"\tlw\t$8,($sp)\n";
            dst<<"\taddiu\t$sp,$sp,"<<"8"<<"\n";


            dst<<"\tbeq\t$8,$0,"<<"$END"<<branch<<"\n";
            dst<<"nop\n";
            dst<<"$CONTINUESTART"<<branch<<":\n";

            //Change Context
            if(context.current_scope.size()<context.last_scope.size()){
                std::vector<int> tmp = context.current_scope;
                context.current_scope.push_back(context.last_scope.back() + 1);
                context.last_scope = tmp;
            }
            else{
                std::vector<int> tmp = context.current_scope;
                context.current_scope.push_back(1);
                context.last_scope = tmp;
            }

            statement->compile(dst, context);

            //Change Context
            context.last_scope = context.current_scope;
            context.current_scope.pop_back();
            context.break_scope.pop_back();
            dst<<"\tj "<<"$START"<<branch<<"\n";
            dst<<"nop\n";
            dst<<"$END"<<branch<<":\n";

        }
        else if(*keyword=="do"){
            std::string branch=makeName("do");
            context.break_scope.push_back(branch);

            dst<<"$START"<<branch<<":\n";
            dst<<"$CONTINUESTART"<<branch<<":\n";

            //Change Context
            if(context.current_scope.size()<context.last_scope.size()){
                std::vector<int> tmp = context.current_scope;
                context.current_scope.push_back(context.last_scope.back() + 1);
                context.last_scope = tmp;
            }
            else{
                std::vector<int> tmp = context.current_scope;
                context.current_scope.push_back(1);
                context.last_scope = tmp;
            }

            statement->compile(dst, context);

            //Change Context
            context.last_scope = context.current_scope;
            context.current_scope.pop_back();
            context.break_scope.pop_back();
            expr->compile(dst, context);

            dst<<"\tlw\t$8,($sp)\n";
            dst<<"\taddiu\t$sp,$sp,"<<"8"<<"\n";

            dst<<"\tbeq\t$8,$0,"<<"$END"<<branch<<"\n";
            dst<<"nop\n";

            dst<<"\tj "<<"$START"<<branch<<"\n";
            dst<<"nop\n";

            dst<<"$END"<<branch<<":\n";

        }
        else if(*keyword=="for"){
            expr_stmnt_1->compile(dst, context);

            std::string branch=makeName("for");
            context.break_scope.push_back(branch);

            dst<<"$START"<<branch<<":\n";

            expr_stmnt_2->compile(dst, context);

            dst<<"\tbeq\t$2,$0,"<<"$END"<<branch<<"\n";
            dst<<"nop\n";

            dst<<"$CONTINUESTART"<<branch<<":\n";

            //Change Context
            if(context.current_scope.size()<context.last_scope.size()){
                std::vector<int> tmp = context.current_scope;
                context.current_scope.push_back(context.last_scope.back() + 1);
                context.last_scope = tmp;
            }
            else{
                std::vector<int> tmp = context.current_scope;
                context.current_scope.push_back(1);
                context.last_scope = tmp;
            }

            statement->compile(dst, context);

            if(expr!=NULL){
                expr->compile(dst, context);
            }

            //Change Context
            context.last_scope = context.current_scope;
            context.current_scope.pop_back();
            context.break_scope.pop_back();

            dst<<"\tj "<<"$START"<<branch<<"\n";
            dst<<"nop\n";
            dst<<"$END"<<branch<<":\n";




        }

    }
}


void Direct_declarator::compile(std::ostream &dst, Context& context) const{ //global and other variebles handeled needed aghhh

    if(context.stack_counting){
        if(decl!=NULL){
            decl->compile(dst, context);
        }
        else if(identifier!=NULL){
            context.declaration_count++;
        }
        else if(const_expr!=NULL){
            context.declaration_count += const_expr->evaluate(context);
        }
        else if(param_type_list!=NULL){
            param_type_list->compile(dst, context);
        }
    }

    else{
        switch(parse_rule_followed){///only 1 and 7 implemented, need to expand
            case 1://function name printed as MIPS label
            if(context.stack_counting){;}
            else{
                if(!context.function_declaration){
                    if(identifier!=NULL){//left hand side
                        context.tmp.name=*identifier;
                        if(!context.external_decl){
                            context.tmp.scope = context.current_scope;
                            context.tmp.stack_offset = context.current_stack_offset;
                            context.current_stack_offset = context.current_stack_offset + 8; //Change to address different variable types and padding, this only works for int


                        }

                    }


                }
                else if(context.function_declaration && context.parameter_declaration){
                    if(identifier!=NULL){//left hand side
                        context.tmp.name=*identifier;
                        context.tmp.scope = context.current_scope;
                        context.tmp.stack_offset = context.current_stack_offset;
                        context.current_stack_offset = context.current_stack_offset + 8; //Change to address different variable types and padding, this only works for int
                        if(context.decl_to_reg<8){
                            dst<<"\tsw\t$"<<context.decl_to_reg<<","<<context.current_stack_offset - 8<<"($fp)\n";
                        }


                        context.variables.push_back(context.tmp);
                    }

                }
                else{
                    dst<<".text"<<"\n";
                    dst<<".align 2"<<"\n";
                    dst<<".globl "<<*identifier<<"\n";
                    // dst<<".ent "<<*identifier<<"\n";
                    dst<<".type "<<*identifier<<", @function"<<"\n";

                    dst<<*identifier<<":\n";

                    //Setup stack
                    context.element_position = context.element_position - (context.declaration_count*context.largest_decl+32);
                    dst<<"\taddiu\t$sp,$sp,-"<<(context.declaration_count*context.largest_decl+32)<<"\n";
                    dst<<"\tsw\t$31,"<<(context.declaration_count*context.largest_decl+28)<<"($sp)\n";
                    dst<<"\tsw\t$fp,"<<(context.declaration_count*context.largest_decl+24)<<"($sp)\n";

                    context.current_fp = context.element_position;
                    dst<<"\tmove\t$fp,$sp\n";
                }

            }


            break;
            // case 2:
            //     // std::cout<<"Direct_declarator switch_1"<<std::endl;
            //     dst<<"(";
            //     decl->print_python(dst);
            //     dst<<"):"<<"\n";
            //     function = false;
            //
            //     break;
            case 3://array with number of elements specified
            if(context.stack_counting){
                // int result = const_expr->evaluate(context);



            }
            break;
            case 5:
            if(!context.external_decl){
                if(direct_decl!=NULL){
                    if(context.stack_counting){;}
                    else{
                        direct_decl->compile(dst, context);
                    }
                }
                if(param_type_list!=NULL){
                    context.parameter_declaration = true;
                    param_type_list->compile(dst, context);
                    context.parameter_declaration = false;
                }
            }
            else{
                context.got_to_rule_5 = true;
            }
            break;
            //     // std::cout<<"Direct_declarator switch_5"<<std::endl;
            //     direct_decl->print_python(dst);
            //     dst<<"(";
            //     param_type_list->print_python(dst);
            //     dst<<"):"<<"\n";
            //     function = false;
            //
            //     break;
            // case 6:
            //     // std::cout<<"Direct_declarator switch_6"<<std::endl;
            //     direct_decl->print_python(dst);
            //     dst<<"(";
            //     identif_list->print_python(dst);
            //     dst<<"):"<<"\n";
            //     function = false;
            //
            //     break;
            case 7://funct_abc()
            // std::cout<<"Direct_declarator switch_7"<<std::endl;
            direct_decl->compile(dst, context);
            ///this is a function now
            // function = false;

            break;
        }
    }
}


void Assignment_expression::compile(std::ostream &dst, Context& context) const{
    // context.assignment_expression_lvl++; //not needed, instead always pop from stack at the end of expression_statement;

    if(context.stack_counting){
        ;
    }


    if(cond_expr!=NULL){
        cond_expr->compile(dst, context);

        //IF PASSING VALUE TO A FUNCTION
        if(context.in_argument_expression_list){
            dst<<"\tlw\t$2,($sp)\n";//rhs value loaded into register 2
            dst<<"\taddiu\t$sp,$sp,"<<context.largest_decl<<"\n";
            dst<<"\taddiu\t$"<<context.val_to_reg<<",$2,0\n";
        }
    }





    else{
        if(assign_expr!=NULL){
            assign_expr->compile(dst, context);
        }

        if(unary_expr!=NULL){
            context.in_assignment_expression = true;
            unary_expr->compile(dst,context);
            context.in_assignment_expression = false;
        }

        dst<<"\tlw\t$2,($sp)\n";//rhs value loaded into register 2
        dst<<"\taddiu\t$sp,$sp,"<<context.largest_decl<<"\n";


        if(context.variable_found&&assign_op!=NULL){//lefthandside variable, rhs is already evaluated and stored in register 2
            dst<<"\tlw\t$t0,"<<context.variables[context.variable_position].stack_offset<<"($fp)\n";//lhs value loaded into t0

            if(*assign_op=="="){
                //do nothing
            }
            else if(*assign_op=="*="){

                dst<<"\tmult\t$t0,$2\n";
                dst<<"\tmflo\t$2\n";
            }
            else if(*assign_op=="/="){
                dst<<"\tdiv\t$t0,$2\n";
                dst<<"\tmflo\t$2\n";
            }
            else if(*assign_op=="%="){
                dst<<"\tdiv\t$t0,$2\n";
                dst<<"\tmfhi\t$2\n";
            }
            else if(*assign_op=="+="){
                dst<<"\taddu\t$2,$t0,$2\n";
            }
            else if(*assign_op=="-="){
                dst<<"\tsubu\t$2,$t0,$2\n";
            }
            else if(*assign_op=="<<="){
                dst<<"\tsll\t$2,$t0,$2\n";
            }
            else if(*assign_op==">>="){
                dst<<"\tsrl\t$2,$t0,$2\n";
            }
            else if(*assign_op=="&="){
                dst<<"\tand\t$2,$t0,$2\n";
            }
            else if(*assign_op=="^="){
                dst<<"\txor\t$2,$t0,$2\n";
            }
            else if(*assign_op=="|="){
                dst<<"\tor\t$2,$t0,$2\n";
            }

            if(!context.enum_found){
                dst<<"\tsw\t$2,"<<context.variables[context.variable_position].stack_offset<<"($fp)\n";
            }
            // if(context.assignment_expression_lvl!=1){
            dst<<"\taddiu\t$sp,$sp,-"<<context.largest_decl<<"\n";
            dst<<"\tsw\t$2,($sp)\n";
            // }

            //IF PASSING VALUE TO A FUNCTION
            if(context.in_argument_expression_list){
                dst<<"\taddiu\t$"<<context.val_to_reg<<",$2,0\n";
            }

            context.variable_found=false;
        }



        if(context.global_found && assign_op!=NULL){//lefthandside variable, rhs is already evaluated and stored in register 2

            dst<<"\tlui\t$t0,%hi("<<context.variables[context.variable_position].name<<")\n";
            dst<<"\tlw\t$t0,%lo("<<context.variables[context.variable_position].name<<")($t0)\n"; //lhs value loaded into t0

            if(*assign_op=="="){
                //do nothing
            }
            else if(*assign_op=="*="){

                dst<<"\tmult\t$t0,$2\n";
                dst<<"\tmflo\t$2\n";
            }
            else if(*assign_op=="/="){
                dst<<"\tdiv\t$t0,$2\n";
                dst<<"\tmflo\t$2\n";
            }
            else if(*assign_op=="%="){
                dst<<"\tdiv\t$t0,$2\n";
                dst<<"\tmfhi\t$2\n";
            }
            else if(*assign_op=="+="){
                dst<<"\taddu\t$2,$t0,$2\n";
            }
            else if(*assign_op=="-="){
                dst<<"\tsubu\t$2,$t0,$2\n";
            }
            else if(*assign_op=="<<="){
                dst<<"\tsll\t$2,$t0,$2\n";
            }
            else if(*assign_op==">>="){
                dst<<"\tsrl\t$2,$t0,$2\n";
            }
            else if(*assign_op=="&="){
                dst<<"\tand\t$2,$t0,$2\n";
            }
            else if(*assign_op=="^="){
                dst<<"\txor\t$2,$t0,$2\n";
            }
            else if(*assign_op=="|="){
                dst<<"\tor\t$2,$t0,$2\n";
            }

            dst<<"\tlui\t$t0,%hi("<<context.variables[context.variable_position].name<<")\n";
            dst<<"\tsw\t$2,%lo("<<context.variables[context.variable_position].name<<")($t0)\n";

            // if(context.assignment_expression_lvl!=1){
            dst<<"\taddiu\t$sp,$sp,-"<<context.largest_decl<<"\n";
            dst<<"\tsw\t$2,($sp)\n";
            // }

            //IF PASSING VALUE TO A FUNCTION
            if(context.in_argument_expression_list){
                dst<<"\taddiu\t$"<<context.val_to_reg<<",$2,0\n";
            }

            context.global_found=false;
        }

    }
    // context.assignment_expression_lvl--;
}

void Conditional_expression::compile(std::ostream &dst, Context& context) const{
    if(logic_or_expr!=NULL&&expr==NULL&&cond_expr==NULL){
        logic_or_expr->compile(dst,context);
    }
    else{

        std::string branch=makeName("conditional");

        logic_or_expr->compile(dst,context);
        dst<<"\tlw\t$8,($sp)\n";
        dst<<"\taddiu\t$sp,$sp,"<<"8"<<"\n";

        dst<<"\tbne\t$8,$0,"<<"$TRUE"<<branch<<"\n";
        dst<<"nop\n";

        cond_expr->compile(dst,context);

        dst<<"\tj "<<"$END"<<branch<<"\n";
        dst<<"nop\n";
        dst<<"$TRUE"<<branch<<":\n";
        expr->compile(dst,context);
        dst<<"$END"<<branch<<":\n";
    }
}


void Logical_or_expression::compile(std::ostream &dst, Context& context) const{
    if(logical_or_expr==NULL&&logical_and_expr!=NULL){
        logical_and_expr->compile(dst,context);
    }
    else if(logical_or_expr!=NULL&&logical_and_expr!=NULL){
        logical_or_expr->compile(dst,context);
        logical_and_expr->compile(dst,context);
        dst<<"#Logical_or_expression_if_2_1\n";
        std::string branch1 = makeName("L_O_E");
        std::string branch2 = makeName("L_O_E");
        std::string branch3 = makeName("L_O_E");

        dst<<"#Logical_and_expression_if_2_1\n";

        dst<<"\tlw\t$t0,($sp)\n";
        dst<<"\taddiu\t$sp,$sp,"<<context.largest_decl<<"\n";
        // context.element_position-=context.largest_decl;
        dst<<"\tlw\t$2,($sp)\n";
        dst<<"\taddiu\t$sp,$sp,"<<context.largest_decl<<"\n";
        // context.element_position-=context.largest_decl;
        dst<<"\tbeq\t$2,$0,$"<<branch1<<"\n";
        dst<<"\tnop\n";
        dst<<"\tbeq\t$t0,$0,$"<<branch2<<"\n";
        dst<<"\tnop\n";

        dst<<"$"<<branch1<<":\n";
        dst<<"\tli\t$2,1\n";
        dst<<"\tb\t$"<<branch3<<"\n";

        dst<<"$"<<branch2<<":\n";
        dst<<"\tmove\t$2,$0\n";

        dst<<"$"<<branch3<<":\n";
        dst<<"\taddiu\t$sp,$sp,-"<<context.largest_decl<<"\n"; ///might need to start pushing at 0 first instead of -4
        // context.element_position+=context.largest_decl;
        dst<<"\tsw\t$2,($sp)\n";
    }
}

void Logical_and_expression::compile(std::ostream &dst, Context& context) const{
    if(logical_and_expr==NULL&&inclusive_or_expr!=NULL){
        inclusive_or_expr->compile(dst,context);
    }
    else if(logical_and_expr!=NULL&&inclusive_or_expr!=NULL){
        logical_and_expr->compile(dst,context);
        inclusive_or_expr->compile(dst,context);
        dst<<"#Logical_and_expression_if_2_1\n";
        std::string branch1 = makeName("L_A_E");
        std::string branch2 = makeName("L_A_E");
        dst<<"#Logical_and_expression_if_2_1\n";

        dst<<"\tlw\t$t0,($sp)\n";//later one in stack
        dst<<"\taddiu\t$sp,$sp,"<<context.largest_decl<<"\n";
        // context.element_position-=context.largest_decl;
        dst<<"\tlw\t$2,($sp)\n";//earlier one in stack
        dst<<"\taddiu\t$sp,$sp,"<<context.largest_decl<<"\n";
        // context.element_position-=context.largest_decl;
        dst<<"\tbeq\t$2,$0,$"<<branch1<<"\n";
        dst<<"\tnop\n";
        dst<<"\tbeq\t$t0,$0,$"<<branch1<<"\n";
        dst<<"\tnop\n";

        dst<<"\tli\t$2,1\n";
        dst<<"\tb\t$"<<branch2<<"\n";

        dst<<"$"<<branch1<<":\n";
        dst<<"\tmove\t$2,$0\n";

        dst<<"$"<<branch2<<":\n";
        dst<<"\taddiu\t$sp,$sp,-"<<context.largest_decl<<"\n"; ///might need to start pushing at 0 first instead of -4
        // context.element_position+=context.largest_decl;
        dst<<"\tsw\t$2,($sp)\n";
    }
}

void Inclusive_or_expression::compile(std::ostream &dst, Context& context) const{
    if(inclusive_or_expr==NULL&&exclusive_or_expr!=NULL){
        exclusive_or_expr->compile(dst,context);
    }
    else if(inclusive_or_expr!=NULL&&exclusive_or_expr!=NULL){
        inclusive_or_expr->compile(dst,context);
        exclusive_or_expr->compile(dst,context);
        dst<<"#Inclusive_or_expression_if_2_1\n";

        dst<<"\tlw\t$t0,($sp)\n";
        dst<<"\taddiu\t$sp,$sp,"<<context.largest_decl<<"\n";
        // context.element_position-=context.largest_decl;
        dst<<"\tlw\t$2,($sp)\n";
        dst<<"\taddiu\t$sp,$sp,"<<context.largest_decl<<"\n";
        // context.element_position-=context.largest_decl;
        dst<<"\tor\t$2,$2,$t0\n";

        dst<<"\taddiu\t$sp,$sp,-"<<context.largest_decl<<"\n"; ///might need to start pushing at 0 first instead of -4
        // context.element_position+=context.largest_decl;
        dst<<"\tsw\t$2,($sp)\n";
    }
}

void Exclusive_or_expression::compile(std::ostream &dst, Context& context) const{
    if(exclusive_or_expr==NULL&&and_expr!=NULL){
        and_expr->compile(dst,context);
    }
    else if(exclusive_or_expr!=NULL&&and_expr!=NULL){
        exclusive_or_expr->compile(dst,context);
        and_expr->compile(dst,context);
        dst<<"#Exclusive_or_expression_if_2_1\n";

        dst<<"\tlw\t$t0,($sp)\n";
        dst<<"\taddiu\t$sp,$sp,"<<context.largest_decl<<"\n";
        // context.element_position-=context.largest_decl;
        dst<<"\tlw\t$2,($sp)\n";
        dst<<"\taddiu\t$sp,$sp,"<<context.largest_decl<<"\n";
        // context.element_position-=context.largest_decl;
        dst<<"\txor\t$2,$2,$t0\n";

        dst<<"\taddiu\t$sp,$sp,-"<<context.largest_decl<<"\n"; ///might need to start pushing at 0 first instead of -4
        // context.element_position+=context.largest_decl;
        dst<<"\tsw\t$2,($sp)\n";
    }
}

void And_expression::compile(std::ostream &dst, Context& context) const{
    if(and_expr==NULL&&equality_expr!=NULL){
        equality_expr->compile(dst,context);
    }
    else if(and_expr!=NULL&&equality_expr!=NULL){
        and_expr->compile(dst,context);
        equality_expr->compile(dst,context);
        dst<<"#And_expr_if_2_1\n";

        dst<<"\tlw\t$t0,($sp)\n";
        dst<<"\taddiu\t$sp,$sp,"<<context.largest_decl<<"\n";
        // context.element_position-=context.largest_decl;
        dst<<"\tlw\t$2,($sp)\n";
        dst<<"\taddiu\t$sp,$sp,"<<context.largest_decl<<"\n";
        // context.element_position-=context.largest_decl;
        dst<<"\tand\t$2,$2,$t0\n";

        dst<<"\taddiu\t$sp,$sp,-"<<context.largest_decl<<"\n"; ///might need to start pushing at 0 first instead of -4
        // context.element_position+=context.largest_decl;
        dst<<"\tsw\t$2,($sp)\n";
    }
}

void Equality_expression::compile(std::ostream &dst, Context& context) const{
    if(equality_expr==NULL&&relat_expr!=NULL){
        relat_expr->compile(dst,context);
    }
    else if(equality_expr!=NULL&&relat_expr!=NULL){
        equality_expr->compile(dst,context);
        relat_expr->compile(dst,context);
        if(*op=="=="){
            dst<<"#Equality_expr_if_2_1\n";

            dst<<"\tlw\t$t0,($sp)\n";
            dst<<"\taddiu\t$sp,$sp,"<<context.largest_decl<<"\n";
            // context.element_position-=context.largest_decl;
            dst<<"\tlw\t$2,($sp)\n";
            dst<<"\taddiu\t$sp,$sp,"<<context.largest_decl<<"\n";
            // context.element_position-=context.largest_decl;
            dst<<"\txor\t$2,$2,$t0\n";
            dst<<"\tsltu\t$2,$2,1\n";

            dst<<"\tandi\t$2,$2,0x00ff\n";
            dst<<"\taddiu\t$sp,$sp,-"<<context.largest_decl<<"\n"; ///might need to start pushing at 0 first instead of -4
            // context.element_position+=context.largest_decl;
            dst<<"\tsw\t$2,($sp)\n";
        }
        else if(*op=="!="){
            dst<<"#Equality_expr_if_2_2\n";

            dst<<"\tlw\t$t0,($sp)\n";
            dst<<"\taddiu\t$sp,$sp,"<<context.largest_decl<<"\n";
            // context.element_position-=context.largest_decl;
            dst<<"\tlw\t$2,($sp)\n";
            dst<<"\taddiu\t$sp,$sp,"<<context.largest_decl<<"\n";
            // context.element_position-=context.largest_decl;
            dst<<"\txor\t$2,$2,$t0\n";
            dst<<"\tsltu\t$2,$0,$2\n";

            dst<<"\tandi\t$2,$2,0x00ff\n";
            dst<<"\taddiu\t$sp,$sp,-"<<context.largest_decl<<"\n"; ///might need to start pushing at 0 first instead of -4
            // context.element_position+=context.largest_decl;
            dst<<"\tsw\t$2,($sp)\n";
        }
    }
}
void Relational_expression::compile(std::ostream &dst, Context& context) const{
    if(relat_expr==NULL&&shift_expr!=NULL){
        shift_expr->compile(dst,context);
    }
    else if(relat_expr!=NULL&&shift_expr!=NULL){
        relat_expr->compile(dst,context);
        shift_expr->compile(dst,context);
        if(*op=="<"){
            dst<<"#Relat_expr_if_2_1\n";

            dst<<"\tlw\t$t0,($sp)\n";
            dst<<"\taddiu\t$sp,$sp,"<<context.largest_decl<<"\n";
            // context.element_position-=context.largest_decl;
            dst<<"\tlw\t$2,($sp)\n";
            dst<<"\taddiu\t$sp,$sp,"<<context.largest_decl<<"\n";
            // context.element_position-=context.largest_decl;
            dst<<"\tslt\t$2,$2,$t0\n";
            dst<<"\tandi\t$2,$2,0x00ff\n";
            dst<<"\taddiu\t$sp,$sp,-"<<context.largest_decl<<"\n"; ///might need to start pushing at 0 first instead of -4
            // context.element_position+=context.largest_decl;
            dst<<"\tsw\t$2,($sp)\n";
        }
        else if(*op==">"){
            dst<<"#Relat_expr_if_2_2\n";

            dst<<"\tlw\t$t0,($sp)\n";
            dst<<"\taddiu\t$sp,$sp,"<<context.largest_decl<<"\n";
            // context.element_position-=context.largest_decl;
            dst<<"\tlw\t$2,($sp)\n";
            dst<<"\taddiu\t$sp,$sp,"<<context.largest_decl<<"\n";
            // context.element_position-=context.largest_decl;
            dst<<"\tslt\t$2,$t0,$2\n";
            dst<<"\tandi\t$2,$2,0x00ff\n";
            dst<<"\taddiu\t$sp,$sp,-"<<context.largest_decl<<"\n"; ///might need to start pushing at 0 first instead of -4
            // context.element_position+=context.largest_decl;
            dst<<"\tsw\t$2,($sp)\n";
        }
        else if(*op=="<="){
            dst<<"#Relat_expr_if_2_3\n";

            dst<<"\tlw\t$t0,($sp)\n";
            dst<<"\taddiu\t$sp,$sp,"<<context.largest_decl<<"\n";
            // context.element_position-=context.largest_decl;
            dst<<"\tlw\t$2,($sp)\n";
            dst<<"\taddiu\t$sp,$sp,"<<context.largest_decl<<"\n";
            // context.element_position-=context.largest_decl;
            dst<<"\taddiu\t$t0,$t0,1\n";
            dst<<"\tslt\t$2,$2,$t0\n";
            dst<<"\tandi\t$2,$2,0x00ff\n";
            dst<<"\taddiu\t$sp,$sp,-"<<context.largest_decl<<"\n"; ///might need to start pushing at 0 first instead of -4
            // context.element_position+=context.largest_decl;
            dst<<"\tsw\t$2,($sp)\n";
        }
        else if(*op==">="){
            dst<<"#Relat_expr_if_2_4\n";

            dst<<"\tlw\t$t0,($sp)\n";
            dst<<"\taddiu\t$sp,$sp,"<<context.largest_decl<<"\n";
            // context.element_position-=context.largest_decl;
            dst<<"\tlw\t$2,($sp)\n";
            dst<<"\taddiu\t$sp,$sp,"<<context.largest_decl<<"\n";
            // context.element_position-=context.largest_decl;
            dst<<"\tslt\t$2,$2,$t0\n";
            dst<<"\txori\t$2,$2,0x1\n";
            dst<<"\tandi\t$2,$2,0x00ff\n";
            dst<<"\taddiu\t$sp,$sp,-"<<context.largest_decl<<"\n"; ///might need to start pushing at 0 first instead of -4
            // context.element_position+=context.largest_decl;
            dst<<"\tsw\t$2,($sp)\n";
        }
    }

}

void Shift_expression::compile(std::ostream &dst, Context& context) const{
    if(shift_expr==NULL&&additive_expr!=NULL){
        additive_expr->compile(dst,context);
    }
    else if(shift_expr!=NULL&&additive_expr!=NULL){
        shift_expr->compile(dst,context);
        additive_expr->compile(dst,context);
        if(*op=="<<"){
            dst<<"#shift_expr_if_2_1\n";

            dst<<"\tlw\t$t0,($sp)\n";
            dst<<"\taddiu\t$sp,$sp,"<<context.largest_decl<<"\n";
            // context.element_position-=context.largest_decl;
            dst<<"\tlw\t$2,($sp)\n";
            dst<<"\taddiu\t$sp,$sp,"<<context.largest_decl<<"\n";
            // context.element_position-=context.largest_decl;
            dst<<"\tsll\t$2,$2,$t0\n";
            dst<<"\taddiu\t$sp,$sp,-"<<context.largest_decl<<"\n"; ///might need to start pushing at 0 first instead of -4
            // context.element_position+=context.largest_decl;
            dst<<"\tsw\t$2,($sp)\n";
        }
        else if(*op==">>"){
            dst<<"#shift_expr_if_2_2\n";

            dst<<"\tlw\t$t0,($sp)\n";
            dst<<"\taddiu\t$sp,$sp,"<<context.largest_decl<<"\n";
            // context.element_position-=context.largest_decl;
            dst<<"\tlw\t$2,($sp)\n";
            dst<<"\taddiu\t$sp,$sp,"<<context.largest_decl<<"\n";
            // context.element_position-=context.largest_decl;
            dst<<"\tsrl\t$2,$2,$t0\n";
            dst<<"\taddiu\t$sp,$sp,-"<<context.largest_decl<<"\n"; ///might need to start pushing at 0 first instead of -4
            // context.element_position+=context.largest_decl;
            dst<<"\tsw\t$2,($sp)\n";
        }

    }
}

void Expression::compile(std::ostream &dst, Context& context) const{
    if(expr==NULL && assign_expr!=NULL){
        dst<<"#expression\n";
        assign_expr->compile(dst,context);
        // context.variable_found=false;
    }
    else if(expr!=NULL&&assign_expr!=NULL){
        // context.variable_found=false;
    }///to be implemented for multiple
}

void Additive_expression::compile(std::ostream &dst, Context& context) const{
    if(additive_expr==NULL&&mult_expr!=NULL){// rule 1
        mult_expr->compile(dst,context);
        dst<<"#add_expr_if_1\n";

    }
    else if(additive_expr!=NULL&&mult_expr!=NULL){//rule 2 and 3
        additive_expr->compile(dst,context);
        mult_expr->compile(dst,context);
        if(*op=="+"){
            dst<<"#add_expr_if_2_1\n";

            dst<<"\tlw\t$t0,($sp)\n";
            dst<<"\taddiu\t$sp,$sp,"<<context.largest_decl<<"\n";
            // context.element_position-=context.largest_decl;
            dst<<"\tlw\t$2,($sp)\n";
            dst<<"\taddiu\t$sp,$sp,"<<context.largest_decl<<"\n";
            // context.element_position-=context.largest_decl;
            dst<<"\taddu\t$2,$2,$t0\n";
            dst<<"\taddiu\t$sp,$sp,-"<<context.largest_decl<<"\n"; ///might need to start pushing at 0 first instead of -4
            // context.element_position+=context.largest_decl;
            dst<<"\tsw\t$2,($sp)\n";
        }
        else if(*op=="-"){
            dst<<"#add_expr_if_2_2\n";

            dst<<"\tlw\t$t0,($sp)\n";//this is taking out right oprand
            dst<<"\taddiu\t$sp,$sp,"<<context.largest_decl<<"\n";
            // context.element_position-=context.largest_decl;
            dst<<"\tlw\t$2,($sp)\n";//this is taking out left oprand
            dst<<"\taddiu\t$sp,$sp,"<<context.largest_decl<<"\n";
            // context.element_position-=context.largest_decl;
            dst<<"\tsubu\t$2,$2,$t0\n";
            dst<<"\taddiu\t$sp,$sp,-"<<context.largest_decl<<"\n"; ///might need to start pushing at 0 first instead of -4
            // context.element_position+=context.largest_decl;
            dst<<"\tsw\t$2,($sp)\n";

        }

    }

}

void Multiplicative_expression::compile(std::ostream &dst, Context& context) const{
    if(mult_expr==NULL&&cast_expr!=NULL){//value stored in $2
        cast_expr->compile(dst,context);
        dst<<"#mult_expr_if_1\n";

        //cast_expr will store to stack
    }
    else if(mult_expr!=NULL&&cast_expr!=NULL){
        mult_expr->compile(dst,context);//result will be in the second top of stack
        cast_expr->compile(dst,context);//result top of stack

        dst<<"\tlw\t$t0,($sp)\n";
        dst<<"\taddiu\t$sp,$sp,"<<context.largest_decl<<"\n";
        // context.element_position-=context.largest_decl;
        dst<<"\tlw\t$2,($sp)\n";
        dst<<"\taddiu\t$sp,$sp,"<<context.largest_decl<<"\n";

        if(*op=="/"){
            dst<<"#add_expr_if_2_1\n";


            dst<<"\tdiv\t$2,$t0\n";
            dst<<"\tmflo\t$2\n";

        }
        else if(*op=="*"){
            dst<<"#add_expr_if_2_2\n";

            dst<<"\tmult\t$2,$t0\n";
            dst<<"\tmflo\t$2\n";
            dst<<"\tmfhi\t$3\n";
        }
        else if(*op=="%"){
            dst<<"#add_expr_if_2_3\n";

            dst<<"\tdiv\t$2,$t0\n";
            dst<<"\tmfhi\t$2\n";
        }
        dst<<"\taddiu\t$sp,$sp,-"<<context.largest_decl<<"\n"; ///might need to start pushing at 0 first instead of -4
        // context.element_position+=context.largest_decl;
        dst<<"\tsw\t$2,($sp)\n";
    }

}

void Cast_expression::compile(std::ostream &dst, Context& context) const{
    if(type_name==NULL&&unary_expr!=NULL){
        unary_expr->compile(dst,context);
        //unary_expr will store to stack
    }
    else if(type_name!=NULL&&unary_expr!=NULL){
        ///to implement type cast
    }

}

void Unary_expression::compile(std::ostream &dst, Context& context) const{
    if(postf_expr!=NULL){
        postf_expr->compile(dst,context);
        //postf_expr will store to stack
    }
    else if(unary_expr!=NULL&&oper!=NULL){
        if(*oper=="++"){
            //Increment variable
            unary_expr->compile(dst,context);
            //FOUND?
            // if(context.variable_found){
            if(!context.enum_found){
                dst<<"\tlw\t$2,"<<context.variables[context.variable_position].stack_offset<<"($fp)\n";
                dst<<"\taddi\t$2,$2,1\n";
                dst<<"\tsw\t$2,"<<context.variables[context.variable_position].stack_offset<<"($fp)\n";
                //Increment value returned in the stack
                dst<<"\tlw\t$2,($sp)\n";
                dst<<"\taddiu\t$sp,$sp,"<<context.largest_decl<<"\n";
                dst<<"\taddi\t$2,$2,1\n";

                dst<<"\taddiu\t$sp,$sp,-"<<context.largest_decl<<"\n";
                dst<<"\tsw\t$2,($sp)\n";
                context.variable_found=false;
            }
            // }
        }
        else if(*oper=="--"){
            //Increment variable
            unary_expr->compile(dst,context);

            // if(context.variable_found){
            if(!context.enum_found){
                dst<<"\tlw\t$2,"<<context.variables[context.variable_position].stack_offset<<"($fp)\n";
                dst<<"\taddi\t$2,$2,-1\n";
                dst<<"\tsw\t$2,"<<context.variables[context.variable_position].stack_offset<<"($fp)\n";
                //Increment value returned in the stack
                dst<<"\tlw\t$2,($sp)\n";
                dst<<"\taddiu\t$sp,$sp,"<<context.largest_decl<<"\n";
                dst<<"\taddi\t$2,$2,-1\n";

                dst<<"\taddiu\t$sp,$sp,-"<<context.largest_decl<<"\n";
                dst<<"\tsw\t$2,($sp)\n";
                context.variable_found=false;
            }
            // }
        }
        else if(*oper=="sizeof"){;}

    }
    else if(cast_expr!=NULL&&unary_op!=NULL){
        cast_expr->compile(dst,context);
        if(context.variable_found){
            dst<<"\tlw\t$2,"<<context.variables[context.variable_position].stack_offset<<"($fp)\n";//load variable
            context.variable_found=false;

        }
        else{
            dst<<"\tlw\t$2,($sp)\n";//load from stack a constant when it's not a variable
            dst<<"\taddiu\t$sp,$sp,"<<context.largest_decl<<"\n";
        }


        if(*unary_op=="+"){///value of variable unchanged
            //nothing to do
        }
        else if(*unary_op=="-"){///value of variable negated
            dst<<"\tsubu\t$2,$0,$2\n";
        }
        else if(*unary_op=="!"){///value of logical value flipped
            dst<<"\tsltu\t$2,$2,1\n";
            dst<<"\tandi\t$2,$2,0x00ff\n";
        }
        else if(*unary_op=="~"){///all bits inverted
            dst<<"\tnor\t$2,$0,$2\n";
        }
        else if(*unary_op=="&"){///return address of variable
            ///to be implemented later
        }
        else if(*unary_op=="*"){///operates on pointer and return the value of the memory space that the pointer is pointing to
            ///to be implemented later
        }


        if(context.variable_found){
            dst<<"\tsw\t$2,"<<context.variables[context.variable_position].stack_offset<<"($fp)\n";
            context.variable_found=false;
        }

        dst<<"\taddiu\t$sp,$sp,-"<<context.largest_decl<<"\n";
        // context.element_position+=context.largest_decl;
        dst<<"\tsw\t$2,($sp)\n";
    }///to do other rules
}

void Postfix_expression::compile(std::ostream &dst, Context& context) const{
    if(prim_expr!=NULL){///rule 3,4,5 to be implemented(function call with argument, function field and pointer)
        prim_expr->compile(dst,context);
    }
    else if(postf_expr!=NULL&&oper!=NULL&&identifier==NULL){
        ///need to retrieve variable value from stack and update its value

        if(*oper=="++"){
            postf_expr->compile(dst,context);
            //Increment variable
            // if(context.variable_found){
            if(!context.enum_found){
                dst<<"\tlw\t$2,"<<context.variables[context.variable_position].stack_offset<<"($fp)\n";
                dst<<"\taddi\t$2,$2,1\n";
                dst<<"\tsw\t$2,"<<context.variables[context.variable_position].stack_offset<<"($fp)\n";
                context.variable_found=false;
            }
            // }

        }
        else if(*oper=="--"){
            postf_expr->compile(dst,context);
            // if(context.variable_found){
            if(!context.enum_found){
                dst<<"\tlw\t$2,"<<context.variables[context.variable_position].stack_offset<<"($fp)\n";
                dst<<"\taddi\t$2,$2,-1\n";
                dst<<"\tsw\t$2,"<<context.variables[context.variable_position].stack_offset<<"($fp)\n";
                context.variable_found=false;
            }
            // }
        }

    }
    else if(postf_expr!=NULL&&expr!=NULL){
        ///rule 2, array
    }
    else if(postf_expr!=NULL && expr==NULL && arg_expr_list==NULL && oper==NULL && identifier==NULL){ //f()
        context.print_function_identifier = true;
        postf_expr->compile(dst,context);
        context.print_function_identifier = false;

    }
    else if(postf_expr!=NULL && expr==NULL && arg_expr_list!=NULL && oper==NULL && identifier==NULL){ //f(77, b)
        context.in_argument_expression_list = true;
        context.val_to_reg = 4;
        arg_expr_list->compile(dst,context);
        context.in_argument_expression_list = false;
        context.print_function_identifier = true;
        postf_expr->compile(dst,context);
        context.print_function_identifier = false;

    }
    else if(postf_expr!=NULL && expr!=NULL && arg_expr_list==NULL && oper==NULL && identifier==NULL){ //f(a,b,c)
        ;
    }

}

void Argument_expression_list::compile(std::ostream &dst, Context& context) const{
    if(arg_expr_list!=NULL){
        arg_expr_list->compile(dst,context);
    }
    if(assign_expr!=NULL){
        assign_expr->compile(dst,context);
    }
    if(context.val_to_reg<8){
        context.val_to_reg++;
    }
}


void Primary_expression::compile(std::ostream &dst, Context& context) const{


    if(context.stack_counting){
        ;
    }


    else{

        if(constant!=NULL){
            dst<<"\tli\t$2,"<<*constant<<"\n";
            dst<<"#primary_expression_if_1\n";
            // dst<<"\tmove\t,$sp,$fp\n"
            dst<<"\taddiu\t$sp,$sp,-"<<context.largest_decl<<"\n"; ///might need to start pushing at 0 first instead of -4
            // context.element_position+=context.largest_decl;
            dst<<"\tsw\t$2,($sp)\n";
        }

        else if(identifier!=NULL){
            if(context.in_assignment_expression){

                context.variable_found = false;
                context.enum_found=false;
                std::vector<int> vect_decr= context.current_scope;
                for(int size = vect_decr.size(); size>1 && !context.variable_found; size--){
                    for(int i = 0; i < context.variables.size() && !context.variable_found; i++){
                        if(context.variables[i].name == *identifier && context.variables[i].scope == vect_decr&&!context.variables[i].is_enum){
                            context.variable_position = i;
                            context.variable_found = true;
                            return;
                        }
                        // else if(context.variables[i].name == *identifier && context.variables[i].scope == vect_decr&&context.variables[i].is_enum){
                        //     context.variable_position = i;
                        //     context.enum_found = true;
                        //     return;
                        // } should not do anything because enum can't be assigned
                    }
                    vect_decr.pop_back();
                }
                //look for global ones
                for(int i = 0; i < context.variables.size() && !context.variable_found; i++){
                    if(context.variables[i].name == *identifier && context.variables[i].scope == vect_decr){
                        context.variable_position = i;
                        context.global_found = true;
                        return;
                    }
                }

                if(!context.variable_found && context.global_found){
                    ;
                    std::cout<<"havent found "<< *identifier <<std::endl;

                }

                context.variable_found=false;
                context.global_found = false;
                context.enum_found=false;
            }

            else if(context.print_function_identifier){
                dst<<"\tjal\t"<<*identifier<<"\nnop\n";
                dst<<"\taddiu\t$sp,$sp,-"<<context.largest_decl<<"\n"; ///might need to start pushing at 0 first instead of -4
                // context.element_position+=context.largest_decl;
                dst<<"\tsw\t$2,($sp)\n";

            }


            else{
                //getting the value
                dst<<"#getting the value of "<<*identifier<<"\n";
                context.variable_found = false;
                context.enum_found=false;
                context.global_found = false;
                std::vector<int> vect_decr= context.current_scope;
                for(int size = vect_decr.size(); size>1 && !context.variable_found; size--){
                    for(int i = 0; i < context.variables.size() && !context.variable_found; i++){
                        if(context.variables[i].name == *identifier && context.variables[i].scope == vect_decr&&!context.variables[i].is_enum){
                            context.variable_position = i;
                            context.variable_found = true;
                        }
                        else if(context.variables[i].name == *identifier && context.variables[i].scope == vect_decr&&context.variables[i].is_enum){
                            context.variable_position=i;
                            context.enum_found=true;

                        }
                    }
                    vect_decr.pop_back();
                }
                if(context.variable_found){
                    dst<<"\tlw\t$2,"<<context.variables[context.variable_position].stack_offset<<"($fp)\n";

                    dst<<"\taddiu\t$sp,$sp,-"<<context.largest_decl<<"\n";
                    dst<<"\tsw\t$2,($sp)\n";
                    context.variable_found=false;
                }
                else if(context.enum_found){
                    dst<<"\taddiu\t$2,$0,"<<context.variables[context.variable_position].enumerator_value<<"\n";
                    dst<<"\taddiu\t$sp,$sp,-"<<context.largest_decl<<"\n";
                    dst<<"\tsw\t$2,($sp)\n";
                    context.enum_found=false;
                }
                else{
                    //look for global ones
                    for(int i = 0; i < context.variables.size() && !context.variable_found; i++){
                        if(context.variables[i].name == *identifier && context.variables[i].scope == vect_decr){
                            context.variable_position = i;
                            context.variable_found = true;

                            dst<<"\tlui\t$2,%hi("<<context.variables[context.variable_position].name<<")\n";
                            dst<<"\tlw\t$2,%lo("<<context.variables[context.variable_position].name<<")($2)\n";
                            dst<<"\taddiu\t$sp,$sp,-"<<context.largest_decl<<"\n";
                            dst<<"\tsw\t$2,($sp)\n";
                        }
                    }

                    if(!context.variable_found){
                        ;
                        std::cout<<"havent found "<< *identifier <<std::endl;

                        //Shift through global variables
                    }
                    if(!context.enum_found){
                        std::cout<<"havent found enum"<<*identifier<<std::endl;
                    }
                    context.enum_found=false;

                    context.variable_found=false;

                }
            }

        }
        else if(expression!=NULL){
            expression->compile(dst,context);
        }
    }
}










void Type_specifier::compile(std::ostream &dst, Context& context) const{
    if(enum_spec!=NULL){
        enum_spec->compile(dst,context);
    }
}


void Enum_specifier::compile(std::ostream &dst, Context& context) const{
    if(identifier==NULL&&enum_list!=NULL){
        enum_list->compile(dst,context);
    }
}


void Enumerator_list::compile(std::ostream &dst, Context& context) const{
    if(enum_list!=NULL){
        enum_list->compile(dst,context);
    }
    if(enum_list==NULL && enumer!=NULL){
        enumer->compile(dst,context);
    }
}


void Enumerator::compile(std::ostream &dst, Context& context) const{
    if(cont_expr==NULL&&enum_constant!=NULL){
        context.tmp.name = *(enum_constant->identifier);
        context.tmp.is_enum=true;
        if(context.variables.size()>0){
            int last_enum_value=0;
            for(int i=0; i<context.variables.size();i++){
                if(context.variables[i].is_enum){
                    last_enum_value=context.variables[i].enumerator_value;
                }
            }
            context.tmp.value = last_enum_value+1;
        }
        else{
            context.tmp.value = 0;
        }
        context.tmp.scope = context.current_scope;
        context.variables.push_back(context.tmp);
    }
    else if(cont_expr!=NULL&&enum_constant!=NULL){
        int result = cont_expr->evaluate(context);
        context.tmp.is_enum=true;
        context.tmp.name = *(enum_constant->identifier);
        context.tmp.scope = context.current_scope;
        context.tmp.value = result;
        context.variables.push_back(context.tmp);

    }
}








int Initializer::evaluate(Context& context) const{
    if(assignment_expr!=NULL){
        return assignment_expr->evaluate(context);
    }
    else return 0;
}

int Assignment_expression::evaluate(Context& context) const{
    if(cond_expr!=NULL){
        return cond_expr->evaluate(context);
    }
    else return 0;
}

int Conditional_expression::evaluate(Context& context) const{
    //skip for now
    if(logic_or_expr!=NULL){
        return logic_or_expr->evaluate(context);
    }
    else return 0;
}

int Logical_or_expression::evaluate(Context& context) const{
    if(logical_or_expr==NULL && logical_and_expr!=NULL){
        return logical_and_expr->evaluate(context);
    }
    else if(logical_or_expr!=NULL && logical_and_expr!=NULL){
        return (int)(logical_or_expr->evaluate(context)||logical_and_expr->evaluate(context));
    }
    else return 0;
}

int Logical_and_expression::evaluate(Context& context) const{
    if(logical_and_expr==NULL && inclusive_or_expr!=NULL){
        return inclusive_or_expr->evaluate(context);
    }
    else if(logical_and_expr!=NULL && inclusive_or_expr!=NULL){
        return (int)(logical_and_expr->evaluate(context) && inclusive_or_expr->evaluate(context));
    }
    else return 0;
}

int Inclusive_or_expression::evaluate(Context& context) const{
    if(inclusive_or_expr==NULL && exclusive_or_expr!=NULL){
        return exclusive_or_expr->evaluate(context);
    }
    else if(inclusive_or_expr!=NULL && exclusive_or_expr!=NULL){
        return (int)(inclusive_or_expr->evaluate(context)|exclusive_or_expr->evaluate(context));
    }
    else return 0;
}

int Exclusive_or_expression::evaluate(Context& context) const{
    if(exclusive_or_expr==NULL && and_expr!=NULL){
        return and_expr->evaluate(context);
    }
    else if(exclusive_or_expr!=NULL && and_expr!=NULL){
        return (int)(exclusive_or_expr->evaluate(context)^and_expr->evaluate(context));
    }
    else return 0;
}

int And_expression::evaluate(Context& context) const{
    if(and_expr==NULL && equality_expr!=NULL){
        return equality_expr->evaluate(context);
    }
    else if(and_expr!=NULL && equality_expr!=NULL){
        return (int)(and_expr->evaluate(context)&equality_expr->evaluate(context));
    }
    else return 0;
}

int Equality_expression::evaluate(Context& context) const{
    if(equality_expr==NULL && relat_expr!=NULL){
        return relat_expr->evaluate(context);
    }
    else if(equality_expr!=NULL && relat_expr!=NULL && *op == "=="){
        return (int)(equality_expr->evaluate(context)==relat_expr->evaluate(context));
    }
    else if(equality_expr!=NULL && relat_expr!=NULL && *op == "!="){
        return (int)(equality_expr->evaluate(context)!=relat_expr->evaluate(context));
    }
    else return 0;
}

int Relational_expression::evaluate(Context& context) const{
    if(relat_expr==NULL && shift_expr!=NULL){
        return shift_expr->evaluate(context);
    }
    else if(relat_expr!=NULL && shift_expr!=NULL && *op == "<"){
        return (int)(relat_expr->evaluate(context)<shift_expr->evaluate(context));
    }
    else if(relat_expr!=NULL && shift_expr!=NULL && *op == ">"){
        return (int)(relat_expr->evaluate(context)>shift_expr->evaluate(context));
    }
    else if(relat_expr!=NULL && shift_expr!=NULL && *op == "<="){
        return (int)(relat_expr->evaluate(context),+shift_expr->evaluate(context));
    }
    else if(relat_expr!=NULL && shift_expr!=NULL && *op == ">="){
        return (int)(relat_expr->evaluate(context)>=shift_expr->evaluate(context));
    }
    else return 0;
}

int Shift_expression::evaluate(Context& context) const{
    if(shift_expr==NULL && additive_expr!=NULL){
        return additive_expr->evaluate(context);
    }
    else if(shift_expr!=NULL && additive_expr!=NULL && *op == "<<"){
        return (int)(shift_expr->evaluate(context)<<additive_expr->evaluate(context));
    }
    else if(shift_expr!=NULL && additive_expr!=NULL && *op == ">>"){
        return (int)(shift_expr->evaluate(context)>>additive_expr->evaluate(context));
    }
    else return 0;
}

int Additive_expression::evaluate(Context& context) const{
    if(additive_expr==NULL && mult_expr!=NULL){
        return mult_expr->evaluate(context);
    }
    else if(additive_expr!=NULL && mult_expr!=NULL && *op == "+"){
        return (int)(additive_expr->evaluate(context)+mult_expr->evaluate(context));
    }
    else if(additive_expr!=NULL && mult_expr!=NULL && *op == "-"){
        return (int)(additive_expr->evaluate(context)-mult_expr->evaluate(context));
    }
    else return 0;
}

int Multiplicative_expression::evaluate(Context& context) const{
    if(mult_expr==NULL && cast_expr!=NULL){
        return cast_expr->evaluate(context);
    }
    else if(mult_expr!=NULL && cast_expr!=NULL && *op == "*"){
        return (int)(mult_expr->evaluate(context)*cast_expr->evaluate(context));
    }
    else if(mult_expr!=NULL && cast_expr!=NULL && *op == "/"){
        return (int)(mult_expr->evaluate(context)/cast_expr->evaluate(context));
    }
    else if(mult_expr!=NULL && cast_expr!=NULL && *op == "%"){
        return (int)(mult_expr->evaluate(context)%cast_expr->evaluate(context));
    }
    else return 0;
}

int Cast_expression::evaluate(Context& context) const{
    //skip for now
    if(unary_expr!=NULL){
        return unary_expr->evaluate(context);
    }
    else return 0;
}


int Unary_expression::evaluate(Context& context) const{
    if(postf_expr!=NULL){
        return postf_expr->evaluate(context);
    }
    // else if(*oper=="++" && unary_expr!=NULL){
    //     return (int)(++(int)(unary_expr->evaluate(context)));
    // }
    // else if(*oper=="--" && unary_expr!=NULL){
    //     return (int)(--(int)(unary_expr->evaluate(context)));
    // }
    else if(*unary_op=="+" && cast_expr!=NULL){
        return (int)(+cast_expr->evaluate(context));
    }
    else if(*unary_op=="-" && cast_expr!=NULL){
        return (int)(-cast_expr->evaluate(context));
    }
    else if(*unary_op=="~" && cast_expr!=NULL){
        return (int)(~cast_expr->evaluate(context));
    }
    else if(*unary_op=="!" && cast_expr!=NULL){
        return (int)(!cast_expr->evaluate(context));
    }
    else return 0;
}

int Postfix_expression::evaluate(Context& context) const{
    if(postf_expr==NULL && prim_expr!=NULL){
        return prim_expr->evaluate(context);
    }
    else return 0;
    // else if(postf_expr!=NULL && *oper == "++"){
    //     return (int)(postf_expr->evaluate(context)++);
    // }
    // else if(postf_expr!=NULL && *oper == "--"){
    //     return (int)(postf_expr->evaluate(context)--);
    // }
}


int Primary_expression::evaluate(Context& context) const{
    if(constant!=NULL){
        return int(*constant);
    }
    else if(expression!=NULL){
        return expression->evaluate(context);
    }
    else if(identifier!=NULL){
        ;
    }
    else return 0;

}

int Expression::evaluate(Context& context) const{
    //skip for now
    if(assign_expr!=NULL){
        return assign_expr->evaluate(context);
    }
    else return 0;
}

int:: Constant_expression::evaluate(Context& context) const{
    //skip for now
    if(cond_expr!=NULL){
        return cond_expr->evaluate(context);
    }
    else return 0;
}
