#include"symtab.h"
#include<stdio.h>
#include<stdlib.h>
#include "tree.h"
#include "codegen.h"

extern tree *ast;                    
extern symbol *ST;
extern int error;
extern int warning;

int main() {
  ST = malloc(SIZE * sizeof(symbol));
  // set the initial values of hash table's symbols
  for (int i = 0; i < SIZE; ++i) {
      ST[i].name = NULL;
      ST[i].type = NULL;
      ST[i].scope = -1;
      ST[i].arraySize = -1;
      ST[i].head = NULL;
  }
  if (yyparse())
    printf("\nREJECT\n");
  else
  {
    printf("\n%d Errors and %d Warnings",error,warning);
    if(error == 0)
    {
      printf("\nACCEPT\n\t\tPrinting Syntax tree\n");
      printAst(ast, 1);
      printf("\n\tPrinting symbol table\n");
      print_ST(ST);
      printf("\n\tGenerating assembly code\n");
      codegen(ast);
      printf("\nAssemble code is genereted");
      printAsm();
    }
    printf("\n");
  }
  return 0;
}

