#include <stdio.h>
#include "tree.h"
#include "codegen.h"
#include "symtab.h"


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
void functionDecl(node * fundecl)
{
    int i, j;
    // writing the function name as comment
    char * str = get_name_of(fundecl->children[1]->val);
    emit_comment(str);
    node *n;
    for (i = 0; i < n->numChildren; i++)
    {
        n=fundecl->children[i];
        switch (n->nodeKind)
        {
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
    //pop instruction;
    //emit jump fp;
    
}
void functionCall(node *n)
{
    int param = 0;
    //do nothig for the first children which is formal decllist

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
int registrAlloc(char *var,int scope)// return the next available register
{
    int x = 0;
    while(x < 20)
    {
        if(reg[x].var_name == NULL)
        {
            strcpy(reg[x].var_name,var);
            reg[x].sp=scope;
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

void emit_comment(char *str)
{
    fprintf(fp,"\n#");
    fprintf(fp,"%s",str);
}



void codegen(node *parent)
{
    int i, j;
    node *n;
    printf("codeg");
    for (i = 0; i < parent->numChildren; i++)
    {
        
        n=parent->children[i];printf("loop %d",i);
        switch (n->nodeKind)
        {
            case FUNDECL:
                 printf("\nfunction decl");
                functionDecl(n);
                //write instructions to pop the stack;
                 break;
            case VARDECL:
                printf("\nglobal vardecl");
                break;
            default:
            printf("no match");
        }
    }
}