# C to MIPS Compiler

The task we were given was to implement features of a C89 to MIPS compiler. Notable files to take a closer look are: 

  src/ast_compile.cpp - Contanins the functions that traverse and output MIPS code for each class in the parse tree. 
  
  include/mips_forward_declaration.hpp - Contains the header for the context class, a class tha holds data that might be needed further down the parse tree.
  
  src/parser.y - rules we defined for the parser, responsible for the creation of the parse tree structure.
  
  
  
  
