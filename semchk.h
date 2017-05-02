#ifndef SEMCHK_H
#define SEMCHK_H

extern int yylineno; 
extern struct fields *ST;
extern char *nodeNames[];
extern char *types[];

int error;
int warning;

typedef struct treenode node;
typedef struct calltable calltable;
typedef struct constantfold  cf;
struct calltable
{
  int line_no;
  char*name;
  node *arglist;
  struct calltable *next;
};
struct constantfold
{
  node *address;
  int i_val;
  char c_val;
};



calltable *ct;
cf cf_table[1000];
int cf_ctr;
void idDeclChk(char *name,int index,int scope);
void semantic_check(node *node);
void multDeclIdChk(); 
void multDeclFunChk();
void multiDeclFunChk(int index);
void multiDeclIdChk(int index);

int loopaddexpr(node* n);

void funCallMatch(); 

void indexArrChk(node *addExpr);
int oobArrChk(int index, node *addExpr);

void typeChk(node *identifier, node *expr);

int funSignChk(int index, node* node);
int fundeclCheck(char *name);

void insertInCallTable(int ,char*,node*);
#endif