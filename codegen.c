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
    //do nothig for the first children which is formal decllist
    codegen(n->children[1]);
    //pop instruction;
    //emit jump fp;
    
}
void functionCall(node *n)
{
    int param = 0;

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
            //write instructions to pop the stack;
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
int registrAlloc()// return the next available register
{
    int x = 0;
    while(x < 20)
    {
        if(reg[x].var_name == NULL)
        {
            return x;
        }
    }
    return memorySpill();
}
int findRegister(char * var_name)
{
    int x = 20;
    int tmp = -1;
    while(x < 20)
    {
        if(strcmp(reg[x].var_name,var_name) == 0)
        {
            if(reg[x].sp > 0)
                return x;// local value is given higher priority
            else
                tmp = x; // global value
        }
    }
    return tmp;
}
int memorySpill()//store the local values in the stack in case of memory spill
{   
    return -1;// not yet complete

}