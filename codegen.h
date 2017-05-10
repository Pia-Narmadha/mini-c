#ifndef CODEGEN_H
#define CODEGEN_H

typedef struct treenode node; 
enum nodeTypes {
    PROGRAM, DECLLIST, DECL, VARDECL, 
    TYPESPEC,  
    FUNDECL, FORMALDL, FORMALDECL, FUNBODY, LOCALDL, LOCALVARDECL, 
    STMLIST, STM, COMSTM, ASTM, ASGN, 
    CONSTM, IF_TYPE, IF_ELSE_TYPE, LSTM, WHILE_TYPE,
    RSTM, RETURN_TYPE, 
    VAR, IDENTIFIER, 
    EXPR, 
    RELOP, 
    ADDEXPR, ADDOP, ADD, SUB, 
    TERM, MULOP, MUL, DIV, 
    FACTOR,INTEGER, ARR,
    FUNCALLEXPR, ARGL, 
    EMPTY
  };
struct registerArray
{
  int sp; // set it as 0 if the variable is global else set it to 1
  char* var_name;
}reg[20];
struct var_list
{
  char *var_name;
  struct var_list *next;
};
typedef struct var_list var_list;
var_list *gvl,*fvl;//global one and is temp

int label;

void assignmentStm(node *n);
int expr(node *n);
void conditionalStm(node *n);
void loopStm(node *n);
void functionDecl(node *n);
int functionCall(node *n);
void codegen(node *n);//this the main codegen function that calls other function to build the .asm code
void printAsm();
int registerAlloc(char *var,int scope);
int memorySpill();
int findRegister(char * var_name);
void functionBody(node *funbody);
void statement(node *tmp);
int getLabel();

void emit(int opcode,int r1,int r2,int r3,int offset);
void emit_comment(char *str);
void emit_i(char *str,int r1,int num);
void emit_r2_label(char *str,int r1,int r2,char *str2);
void emit_r3(char *str,int r1,int r2,int r3);
void emit_label(char *str,char *str2);
void emit_offset(char *str,int r1,int offset,char* pointer);
void emit_r2(char *str,int r1,int r2);
/*
returns the register number if the variable is in the register else returns -1;
var_name is the name of the varable 
*/
#endif
