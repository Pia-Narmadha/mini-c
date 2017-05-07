#ifndef CODEGEN_H
#define CODEGEN_H

typedef struct treenode node; 
extern FILE *fp;
enum nodeTypes {
    PROGRAM, DECLLIST, DECL, VARDECL, 
    TYPESPEC,  
    FUNDECL, FORMALDL, FORMALDECL, FUNBODY, LOCALDL, LOCALVARDECL, 
    STMLIST, STM, COMSTM, ASTM, ASGN, 
    CONSTM, IF_TYPE, ELSE_TYPE, LSTM, WHILE_TYPE,
    RSTM, RETURN_TYPE, 
    VAR, IDENTIFIER, 
    EXPR, 
    RELOP, 
    ADDEXPR, ADDOP, ADD, SUB, 
    TERM, MULOP, MUL, DIV, 
    FACTOR, ARR,
    FUNCALLEXPR, ARGL, 
    EMPTY
  };
struct registerArray
{
  int sp; // set it as 0 if the variable is global else set it to 1
  char* var_name;
}reg[20];
void emit(char* opcode,int r1,int r2,int r3,int offset);//it creates the .asm file and opens it
void assignmentStm(node *n);
void expr(node *n);
void conditionalStm(node *n);
void loopStm(node *n);
void functionDecl(node *n);
void functionCall(node *n);
void codegen(node *n);//this the main codegen function that calls other function to build the .asm code
void printAsm();
int registerAlloc(char *var,int scope);
int memorySpill();
int findRegister(char * var_name)
/*
returns the register number if the variable is in the register else returns -1;
var_name is the name of the varable 
*/
#endif