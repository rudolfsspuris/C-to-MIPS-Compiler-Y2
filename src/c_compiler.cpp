#include "ast.hpp"
#include "ast_python.cpp"
#include "ast_compile.cpp"
#include <fstream>

int gdb_or_not=0;

int main(int argc, char *argv[])
{
    if(std::string(argv[0])=="gdb"){
        gdb_or_not=2;
    }
    if(argc < 5){
        fprintf(stderr, "usage : compiler sourceCode\n");
        exit(1);
    }

    // std::ifstream src(argv[2]);

    FILE *src;
    src=fopen(argv[2+gdb_or_not], "r");
    if(!src){
        fprintf(stderr, "Couldn't open '%s'\n", argv[2]);
        exit(1);
    }

    const Translation_unit *ast=parseAST(src);

    if(std::string(argv[1+gdb_or_not])=="--translate"){
        std::ofstream outfile(argv[4+gdb_or_not]);
        ast->print_python(outfile);
        //outfile << "my text here!" << std::endl;
        //pass outfile as an argument to translate function and print python code into it like above
        // run translate function with tree and outfile
        outfile<<"if __name__ == \"__main__\":\n\timport sys\n\tret=main()\n\tsys.exit(ret)"<<std::endl;
        outfile.close();
    }
    else if(std::string(argv[1])=="-S"){
        Context context;

        context.current_scope.push_back(1);
        context.last_scope.push_back(1);
        context.break_scope.push_back(" ");
        std::ofstream outfile (argv[4]);

        ast->compile(outfile, context);
        //pass outfile as an argument to compile function and print MIPS code into it like above
        // Compile(tree, outfile)// run compiler function with tree

        outfile.close();
    }


    return 0;
}
