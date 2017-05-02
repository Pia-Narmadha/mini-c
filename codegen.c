#include <stdio.h>
#include "tree.h"
#include "codegen.h"


FILE *fp;

void emit(char* opcode,int r1,int r2,int r3,int offset)//it creates the .asm file and opens it
{
   fp = fopen("assembly.asm", "w+");// creteas a new file , if the file already exists then truncates its length to zero
   //write code for printing 
}
void assignmentStm(node *n)
{

}
void expr(node *n)
{

}
void conditionalStm(node *n)
{

}
void loopStm(node *n)
{

}
void functionDecl(node *n)
{

}
void functionCall(node *n)
{

}
void codegen(node *n)
{
    int i, j;
    for (i = 0; i < n->numChildren; i++)
        codegen(getChild(n, i));
    switch (n->nodeKind)
    {
        case FUNDECL:
            functionDecl(n);
        break;
        case FUNCALLEXPR:
            functionCall(n);
        break;
        case EXPR:
            expr(n);
        break;
        case ASGN:
            assignmentStm(n);
        break;
        case WHILE_TYPE:
            loopStm(n);
        break;
        case CONSTM:
            conditionalStm(n);
        break;
    }
}
void printAsm()
{
    char c = fgetc(fp);
    while (c != EOF)
    {
        printf ("%c", c);
        c = fgetc(fp);
    }
 
    fclose(fp);
}
