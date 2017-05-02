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
void emit(char* opcode,int r1,int r2,int r3,int offset);//it creates the .asm file and opens it
void assignmentStm(node *n);
void expr(node *n);
void conditionalStm(node *n);
void loopStm(node *n);
void functionDecl(node *n);
void functionCall(node *n);
void codegen(node *n);//this the main codegen function that calls other function to build the .asm code
void printAsm();


#endif