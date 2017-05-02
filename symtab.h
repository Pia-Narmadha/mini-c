#ifndef SYMTAB_H
#define SYMTAB_H

#define MAXIDS 1000
#define SIZE next_prime(ceil(MAXIDS/0.45)) /* HT has a load-factor of 0.45 the MAXIDS */

typedef struct fields symbol;
typedef struct function_sign_list signature;
typedef struct treenode tree;


symbol *ST;
extern char *types[];
extern int yylineno; 
extern int scope;

struct fields
{
    char *name;
    char *type;
    int scope;
    int arraySize;
    signature *head;
    symbol *next;
};
struct function_sign_list
{
    char *type;
    signature *next;
};

/*Inserts the identifier into the symbol table
type(t) is the type of the variable or the return type of the function in case of a function
*/
int insert_into_ST(char *n, int t, int s);
void insert_function_signature(int index,int pos,int type);
int lookup_in_ST(char *n);//get the var name and return index if found else returns -1;
int getIDType(int index);
int lookup_fx_in_ST(char *n);

void print_ST();//prints the symbol table

signature* getfunctionSignature(int index,int pos);
char* getfunctionReturnType(int index,int pos);
void  add_function_signature_into_symtab(int index,int ret, tree* formalDL);
int hash(char *name);
int next_prime(int x);
int is_prime(int x);

#endif
