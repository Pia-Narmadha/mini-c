#include "semchk.h"
#include<stdio.h>
#include<stdlib.h>
#include "tree.h"
#include "symtab.h"
#include<string.h>

void insertInCallTable(int num ,char* name ,node* arg)
{
  calltable *ptr=ct;
  calltable *tmp=(calltable*) malloc(sizeof(struct calltable));
  tmp->line_no=num;
  tmp->name=name;
  tmp->arglist=arg;
  tmp->next=NULL;
  if(ct==NULL)
    ct=tmp;
  else
  {
    while(ptr->next !=NULL)
    {
      ptr=ptr->next;
    }
    ptr->next=tmp;
  }
}
void idDeclChk(char *name,int index,int scope) //checks if the variable is declared or not
{
  symbol *tmp=&ST[index];
  if(index==-1)
  {
    error++;
    printf("\nError at line %d:Use of undeclared Variable %s",yylineno,name);
    return;
  }
  while(tmp)
  {
    if(tmp->head == NULL && tmp->scope == scope)
      return;
    tmp=tmp->next;
  }
  tmp=&ST[index];
  while(tmp)
  {
    if(tmp->head == NULL && tmp->scope == 0)
      return;
    tmp=tmp->next;
  }
  error++;
  printf("\nError at line %d:Use of undeclared Variable %s",yylineno,name);
}
void funCallMatch()
{
  int index=0;
  calltable *tmp=ct;
  while(ct)
  {
    yylineno=ct->line_no;
    index=fundeclCheck(ct->name);
    funSignChk(index,ct->arglist);
    free(tmp);
    ct=ct->next;
    tmp=ct;
  }
}
int fundeclCheck(char *name)//returns the index of the symbol table if found, else returns -1
{
  int index=lookup_fx_in_ST(name);
  if( index == -1)
  {
    error++;
    printf("\nError at line %d:Calling a undeclared function %s",yylineno,name);
  }
  return index;
}

int funSignChk(int index, node *node1)
{
  //input is index of the symbol table and the arglist node.
  //return 0 if the signature didnot match else returns 1.
  int pos = 0;
  int count=0;
  tree *tmp;
  signature* sign = getfunctionSignature(index, pos);
  if(node1 == NULL)//if the call has no arguments
  {
    while(sign != NULL)//loop in case of overloaded function
    {
      if(!strcmp(sign->type, "--"))
          return 1; 
      sign = getfunctionSignature(index, ++pos);//gets the next function signature for function with same name else NULL
    }
  }
  else//if the call has arguments
  {
    while(sign != NULL)//loop in case of overloaded function
    {
      tmp=node1;
      while(tmp != NULL && sign !=NULL)//loop to traverse the sign list and the node
      {
        if(strcmp(types[tmp->children[tmp->numChildren-1]->val],sign->type))
        {
          count++;
          break;
        }
        sign = sign ->next;
        tmp=tmp->children[0];
      }
      if(count == 0)
          return 1; 
      sign = getfunctionSignature(index, ++pos);//gets the next function signature for function with same name else NULL
      count =0;
    }
  }
  error++;
  printf("\nError :Function call didnot match with function declaration %s",ST[index].name);
  return 0;
}
void multiDeclIdChk(int index) 
{
      symbol *tmp1=&ST[index];
      symbol *tmp2;
      while(tmp1->next != NULL)
      {
        if(tmp1->head == NULL)
        {
          break;
        }
        tmp1=tmp1->next;
      }
      if(tmp1->next == NULL)
        return;
      tmp2= tmp1->next;
      while(tmp2)
      {
        if(tmp2->head == NULL && tmp2->scope ==  tmp1->scope)
        {
          error++;
          printf("\nError at line %d: Multiple declarations for variable %s",yylineno,ST[index].name);
        }
        tmp2=tmp2->next;
      }
}

void multiDeclFunChk(int index)
{
  symbol *sym=&ST[index];
  int loop=0,check=0,count=0;
  if(ST[index].next ==NULL)
    return;
  signature *s1=getfunctionSignature(index,1);
  signature *s2;
  while(sym)
    {
        s2=sym->head;
        while(s1!=NULL && s2 !=NULL)
        {
            loop++;
            if(strcmp(s2->type,s1->type)==0)
            {
              check++;
            }
            s1=s1->next;
            s2=s2->next;
        }
        if(check == loop)
        {
          count++;
        }
        sym=sym->next;
      }
      if(count > 0)
      {
            error++;
            printf("\nError : Multiple declarations for function %s",ST[index].name);
        }
}


void indexArrChk(node *addExpr)
{
  if (addExpr->val != 0)
  {
    error++;
    printf("\nError at line %d: non-int subscriptType: %s",yylineno, types[addExpr->val]);
  }
}
int loopaddexpr(node* n)
{
  int c=0;
  if(n == NULL)
    return -1;
  if(n->nodeKind == 35)//node is a factor
  {
    return n->val;
  }
  while(c<n->numChildren-1)
  {
    return loopaddexpr(n->children[c]);
  }
}
int oobArrChk(int index, node *addExpr) 
{
  int v=loopaddexpr(addExpr);
  int as=-1;
  symbol *tmp = &ST[index];
  while(tmp)
  {
    if(tmp->head == NULL)
      break;
    tmp =tmp->next;
  }
  if(tmp != NULL)
  {
    as= tmp->arraySize;
  }
  if(v >= as)
  {
    error++;
    printf("\nError :Array index outofbounds\n");
    return 5;
  }
  return 0;
}

void typeChk(node *var, node *expr)  
{
  node *rhs = (node *)malloc(sizeof(node));
  rhs = expr;
  while (rhs->numChildren > 0)
  {
    if (rhs->numChildren > 1)
    {
      warning++;
      printf("\nWarning : non-Simple expr assgn\n");
      return;
    } 
    rhs = getChild(rhs, 0);
  }
  if (!strcmp(nodeNames[rhs->nodeKind],"identifier"))
  {
    if (var->val != rhs->val)
    {
      error++;
      printf("\nError : ID mismatch types: %s != %s, ", types[var->val], ST[rhs->val].type);
      return;
    }
  }
  else if (!strcmp(nodeNames[rhs->nodeKind], "factor"))
  {
    if (rhs->val < 0|| rhs->val > 3){}
      //printf("HEYYYYYYYYYYYYY!");
    else if (var->val)
    // else if (strcmp(ST[identifier->val].type, types[rhs->val]))
    {
      error++;
      printf("\nError: ID mismatch types: %s != %s, ",types[var->val], ST[rhs->val].type);
      return;
    }
  }
}
/* trying to assgin a char to a void
causes an error, etc. You only need to perform this check when the right
hand side is a simple expression (i.e, a variable or literal) or a function
call.*/