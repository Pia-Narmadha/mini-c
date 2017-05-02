#include "symtab.h"
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"tree.h"

int insert_into_ST(char *n,int t, int s)// inserts the variable into the symbol table
{
    int index = hash(n);
    int ii; // an index for faster (quadratic) probing. 
    symbol *tmp;
    for (int i = 0; i < SIZE; i++)
    { 
        ii = (index+i*i)%SIZE;
        if(ST[ii].name==NULL) 
        {
            ST[ii].name = (char *) malloc(sizeof(strlen(n)+1));
            strcpy(ST[ii].name, n);
            ST[ii].type=strdup(types[t]);
            ST[ii].scope = s;
            ST[ii].arraySize=-1;
            ST[ii].next=NULL;
            return ii;
        }
        else if (!strcmp(ST[ii].name, n))
        {
            tmp = (symbol*)malloc (sizeof(symbol));
            tmp->name = (char *) malloc(sizeof(strlen(n)+1));
            strcpy(tmp->name, n);
            tmp->type=strdup(types[t]);
            tmp->scope = s;
            tmp->arraySize=-1;
            tmp->next=ST[ii].next;
            ST[ii].next = tmp;
            return ii;
        }
    }
    printf("\n\t\tsymbol table insertion error");
    return -1; 
}
void  add_function_signature_into_symtab(int index,int ret, tree *formalDL)//calls insert_function_signature function with single parameter each time
{
  int pos=0;signature* tmp;
  tmp=getfunctionSignature(index,pos);
  while(strcmp(getfunctionReturnType(index,pos),types[ret]) || tmp!=NULL)
  {
     pos++;
     tmp=getfunctionSignature(index,pos);
  }
  if(formalDL == NULL)
    insert_function_signature(index,pos,-1);
  else
  {
    insert_function_signature(index,pos,getChild(formalDL,0)->val);
    for (int i = 2; i < formalDL->numChildren; i++)
    {
      insert_function_signature(index,pos,getChild(getChild(formalDL,i),0)->val);
    }
  }                    
}
void insert_function_signature(int index,int pos,int type)//called by add_function_signature function to insert a parammeter
{
    signature *tmp,*ptr;
    symbol *sym = &ST[index];
    if(index == -1)
    {
        printf("error while adding the function signature");
        return;
    }
    while(pos > 0)
    {
        sym=sym->next;
        pos--;
    }
    //creating a dummy tmp variable and inserting it into the end of the list
    tmp = (signature *)malloc(sizeof(signature));
    if(type==-1)
        tmp->type="--";
    else
        tmp->type=strdup(types[type]);
    tmp->next=NULL;
    if(sym->head == NULL)
    {
        sym->head=tmp;
    }
    else
    {
        ptr=sym->head;
        while(ptr->next != NULL)
        {
            ptr=ptr->next;
        }
        ptr->next=tmp;
    }
}
char* getfunctionReturnType(int index,int pos)//index is location is the symbol table and pos is the position in the list
{
    symbol *tmp = &ST[index];
    while(pos > 0 && tmp !=NULL)//loop to traverse in the chained list
    {
        tmp = tmp->next;
        pos--;
    }
    if(tmp != NULL)
        return tmp->type;
    return "";
}
signature* getfunctionSignature(int index,int pos)//index is location is the symbol table and pos is the position in the list
{
    symbol *tmp = &ST[index];
    while(pos > 0 && tmp !=NULL)//loop to traverse in the chained list
    {
        tmp = tmp->next;
        pos--;
    }
    if(tmp != NULL)
        return tmp->head;
    return NULL;
}
int lookup_in_ST(char *n)//get the var name and return index if found else returns -1;
{
    int index = hash(n);
    int ii; // an index for faster (quadratic) probing.
    for (int i = 0; i < SIZE; i++) 
    {        
        ii = (index+i*i)%SIZE;
        if ( ST[ii].name != NULL && !strcmp( ST[ii].name, n) && ST[ii].head==NULL)
        {
            if (ST[ii].scope == 0 || scope+1==ST[ii].scope)  
            {
                return ii;
            }
            else 
            {
                symbol *tmpSym = ST[ii].next;
                while (tmpSym)
                {
                    if (tmpSym->scope == 0 || tmpSym->scope == scope+1)
                    {
                        return ii;
                    }
                    tmpSym=tmpSym->next;
                }
            }   
            return -1;
        }        
    }
    return -1;
}
int lookup_fx_in_ST(char *n)//get the var name and return index if found else returns -1;
{
     
    int index = hash(n);
    int ii; // an index for faster (quadratic) probing.
    for (int i = 0; i < SIZE; i++) 
    {        
        ii = (index+i*i)%SIZE;
        if ( ST[ii].name != NULL && ST[ii].head!=NULL && !strcmp( ST[ii].name, n) )
        {
            return ii;   
        }
               
    }
    return -1;
}

int getIDType(int ii)
{
    char *dT;
    int globe;
    symbol *tmpSym;
    if (scope+1==ST[ii].scope)  
    {
        dT = ST[ii].type;
    }
    else 
    {
        if (ST[ii].scope == 0)
        {
            dT = ST[ii].type;
        }
        tmpSym = ST[ii].next;
        while (tmpSym)
        {
            if (tmpSym->scope == scope+1)
            {
                dT = tmpSym->type;
            }
            else if (dT == NULL && tmpSym->scope == 0)
            {
                dT = tmpSym->type;
            }
            tmpSym=tmpSym->next;
        }
    }

    for (int i=0; i<4; i++)
    {
      if (!strcmp(types[i], dT))
      {
        return i;
      } 
    }
    printf("symtab.c linear 126, no dataType found\n");
    return 2; // this 2 is a bad value, but can hopefully never get returned
}



void print_ST()
{
    printf("\n\n\t\t\tSYMBOL TABLE\n");
    printf("INdex\tName\t\tType\t\tScope\t[size]\t\tSignature \n");
    for( int i=0; i<SIZE; i++)
    {
        if (ST[i].name!=NULL)
        {
            printf("\n%d\t%s\t\t%s\t%d",i,ST[i].name,ST[i].type,ST[i].scope);
            if (ST[i].arraySize != -1) 
                printf("\t[%d]",ST[i].arraySize);
            else 
                printf("\t\t");
            printf("\t");

            signature *tmp = ST[i].head;
            while(tmp)//printing signature
            {
                printf("%s, ",tmp->type);
                tmp=tmp->next;
            }
            symbol *tmpSym = ST[i].next;
            while(tmpSym)//printing chained list
            {
                printf("\n%d\t%s\t\t%s\t%d",i,tmpSym->name,tmpSym->type,tmpSym->scope);
                if (tmpSym->arraySize != -1) 
                    printf("\t[%d]",tmpSym->arraySize);
                 else 
                    printf("\t\t");
                printf("\t");
                signature *tmp = tmpSym->head;
                while(tmp)
                {
                    printf("%s, ",tmp->type);
                    tmp=tmp->next;
                }
                tmpSym= tmpSym->next;
            }
        }
    }
}


// returns hash value computed using the djb2 hash algorithm
// (All of the code in this fill comes from lecture notes by Dr. Koh)
int hash(char *key) {
    int hash = 5381;
    int c;
    while ( (c = *key++))
        hash = ((hash<<5) + hash) + c;
    return (hash % MAXIDS);
}

// adaption of : http://stackoverflow.com/questions/4475996
//               (Howard Hinnant, Implementation 5)
// returns the smallest prime that is >= x
int next_prime(int x) {
    switch (x)
    {
        case 0:
        case 1:
        case 2:
            return 2;
        case 3:
            return 3;
        case 4:
        case 5:
            return 5;
    }
    int k = x / 6;
    int i = x - 6 * k;
    int inc = i < 2 ? 1 : 5;
    x = 6 * k + inc;
    for (i = (3 + inc) / 2; !is_prime(x); x += i)
        i ^= 6;
    return x;
}

// adaption of : http://stackoverflow.com/questions/4475996
//               (Howard Hinnant, Implementation 5)
// returns true if a given non-negative # is prime
// otherwise returns false
// making use of following:
//   if a # is not divisible by 2 or by 3, then
//   it is of the form 6k+1 or of the form 6k+5
int is_prime(int x) {
   if (x <= 3 || x == 5) return 1;
   if (x == 4 || x == 6) return 0;

   int inc = 4;
   for (int i = 5; 1; i += inc)
   {
      int q = x / i;
      if (q < i)
         return 1;
      if (x == q * i)
         return 0;
      inc ^= 6;
   }
   return 1;
}
