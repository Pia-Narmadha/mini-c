#include<stdio.h>
#include "symtab.h"

int main()
{
   int x = insert_into_ST("name1",1,1);
   insert_into_ST("name1",1,2);
   insert_into_ST("name1",1,3);
   char * str = get_name_of(x);
   printf("%s",str);
}