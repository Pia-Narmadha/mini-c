#include <stdio.h>
#include "tree.h"
#include "codegen.h"
#include "symtab.h"
#include<string.h>
#include "semchk.h"


FILE *fp;
char *arith[4] = {"add", "sub", "mul", "div"};

void emit(int opcode,int r1,int r2,int r3,int offset)//it creates the .asm file and opens it
{
   //fp = fopen("assembly.asm", "w+");// creteas a new file , if the file already exists then truncates its length to zero
   //write code for printing 
   switch (opcode) {
   case 0:
   fprintf(fp, "\n%s, $r%d, $r%d, $r%d",arith[opcode], offset, r1, r2);
   break;
   case 1:
   fprintf(fp, "\n%s, $r%d, $r%d, $r%d",arith[opcode], offset, r1, r2);
   break;
   case 2:
   fprintf(fp, "\n%s, $r%d, $r%d, $r%d",arith[opcode], offset, r1, r2);
   break;
   case 3:
   fprintf(fp, "\n%s, $r%d, $r%d, $r%d",arith[opcode], offset, r1, r2);
   break;
   case 4:
   fprintf(fp, "\nli, $r%d, %d", offset, r1);
   break;
   case 5:
   fprintf(fp, "\nlw, $r%d, @%d", offset, r1);
   break;
   case 6:
   fprintf(fp, "\nsw, $@%d, $r%d\n", offset, r2);  
   }

}
int getLabel()
{
    return label++;
}
void assignmentStm(node *n)
{
    if(n->numChildren >1)
    {
        int r=registerAlloc(get_name_of(n->children[0]->val),1);
        int result=expr(getChild(n,2));

        fprintf(fp,"\nmove $%d,$%d",r,result);
    }
    else
    {
        int result=expr(getChild(n,0));
    }
}
int expr(node *n)
{
    int result, t1, t2, i,offset;
    char op;
    char* str;
    var_list *dummy = fvl;
    if(n->nodeKind == EXPR)
        n=getChild(n,0);
   // printf("node Kind = %s\n", nodeNames[n->nodeKind]);
    switch(n->nodeKind)
    {
        case ADD_OP:
            t1 = expr(getChild(n, 0));
            t2 = expr(getChild(n, 1));
            result = registerAlloc("@",1);
            emit_r3("add",result,t1,t2);
            break;
        case SUB_OP://subraction expression
            t1 = expr(getChild(n, 0));
            t2 = expr(getChild(n, 1));
            result = registerAlloc("@",1);
            emit_r3("sub",result,t1,t2);
            //emit(n->val, t1, t2, 0,result); 
            break;  
        case MUL_OP:
            t1 = expr(getChild(n, 0));
            t2 = expr(getChild(n, 1));
            result = registerAlloc("result",1);
            //emit(n->oper, t1, t2, 0,result);
            emit_r3("mul",result,t1,t2);
            break;
        case DIV_OP:
            t1 = expr(getChild(n, 0));
            t2 = expr(getChild(n, 1));
            result = registerAlloc("result",1);
            emit_r3("div",result,t1,t2);
            break;  
        case LT_OP:
            t1 = expr(getChild(n, 0));
            t2 = expr(getChild(n, 1));
            result = registerAlloc("result",1);
            emit_r3("slt",result,t1,t2);
            break;
        case  GT_OP:
            t1 = expr(getChild(n, 0));
            t2 = expr(getChild(n, 1));
            result = registerAlloc("result",1);
            emit_r3("slt",result,t2,t1);
            break;
        case GTE_OP:
            t1 = expr(getChild(n, 0));
            t2 = expr(getChild(n, 1));
            result = registerAlloc("result",1);
            fprintf(fp,"\nsubi $%d,$%d,1",t2,t2);
            emit_r3("slt",result,t2,t1);
            break;
        case LTE_OP:
            t1 = expr(getChild(n, 0));
            t2 = expr(getChild(n, 1));
            result = registerAlloc("result",1);
            fprintf(fp,"\nsubi $%d,$%d,1",t1,t1);
            emit_r3("slt",result,t1,t2);
            break;
        case EQ_OP:
            t1 = expr(getChild(n, 0));
            t2 = expr(getChild(n, 1));
            result = registerAlloc("result",1);
            emit_r3("seq",result,t1,t2);
            break;
        case NEQ_OP:
            t1 = expr(getChild(n, 0));
            t2 = expr(getChild(n, 1));
            result = registerAlloc("result",1);
            emit_r3("sub",result,t1,t2);
            fprintf(fp,"\nseq $%d,$%d,%d",result,result,0);
            break;
        case IDENTIFIER:
            str = strdup(get_name_of(n->val));
            result = findRegister(str);
            if(result == -1)
            {
                printf("\nRun time error: Use of uninitialized variable %s\n",str);
                exit(1);
            }
            offset =0;
            if(result == -1)// memory spill has occured and the value of the var is stored in stack
            {
                result = registerAlloc(str,1);
                while(dummy != NULL && strcmp(dummy->var_name,str) != 0)
                {
                    dummy = dummy ->next;
                    offset++;
                }
                fprintf(fp,"\nlw $%d,%d($sp)",result,4*offset);
                int value = 0;
            }
            break; 
        case INTEGER:
            result = registerAlloc("@",1);
            emit_i("li",result,n->val);
            break;
        case FUNCALLEXPR:
            result = functionCall(n);
            break;
            
    } 
    return result;
}
void conditionalStm(node *n)
{
    int result,iflabel,elselabel,r;
    iflabel=getLabel();
    elselabel=getLabel();
    //code for the conditional expression
    result = expr(getChild(n,1));

    //branching instruction
    r=registerAlloc("@",0); 
    fprintf(fp,"\nbeq $%d,$%d,L%d",result,r,iflabel);
    
    if(n->numChildren > 3)//code for else part
    {
        statement(getChild(n,4));
        fprintf(fp,"\nj L%d",elselabel);
    }
    //code for if then part
    fprintf(fp,"\nL%d :",iflabel);
    statement(getChild(n,2));

    fprintf(fp,"\nL%d :",elselabel);

}
void loopStm(node *n)
{
    int beginl,endl,result,r;
    beginl=getLabel();
    endl=getLabel();

    //loop condition
    fprintf(fp,"\nL%d :",beginl);
    expr(getChild(n,1));

   //branching code
    r=findRegister("@");
    if(r==-1)
        r=registerAlloc("@",1);
    fprintf(fp,"\nbne $%d,$%d,L%d",result,r,endl);

    //code for statement
    statement(getChild(n,2));
    fprintf(fp,"j L%d",beginl);

    //printing label 
     fprintf(fp,"L%d:",endl);


}
void statement(node *tmp)
{
    printf("\n calling stmt" );
    node *c2,*dummy;
    switch (tmp->nodeKind)
        {
            case COMSTM://compound statement
                c2 = getChild(tmp,0);
                printf("begin compound statement");
                while(c2->children[0]->nodeKind != EMPTY)// handling statement list
                {
                    printf("loop compound statement %d",c2->nodeKind);
                    dummy=getChild(getChild(c2,0),0);
                    c2 = getChild(c2,1);
                    statement(dummy);
                }
                printf("end compound statement");
                break;
            case EXPR:
                expr(getChild(tmp, 0));
                break;
            case ASTM:
                printf("Calling ASTM\n");
                assignmentStm(tmp);
                //expr(tmp);
                break;
            case LSTM://loop statement
                loopStm(tmp);
                break;
            case CONSTM://conditional statement
                conditionalStm(tmp);
                break;
            case RSTM://conditional statement
                if(tmp->numChildren > 1)
                {
                    int result = expr(getChild(tmp,1));
                    fprintf(fp,"\nmove $25,$%d",result);
                }
                fprintf(fp,"\njr $ra");
                break;
            case STM:
                statement(getChild(tmp,0));
                break;
        }
}
void functionBody(node *funbody)
{
    int i;
    node *c1,*c2,*dummy;
    c1=getChild(funbody,0);
    c2=getChild(funbody,1);
    while(c1->children[0]->nodeKind != EMPTY)//c1->nodeKind != EMPTY)//handling local decllist
    {
        dummy =getChild(c1,0);
        printf("varname %s:",get_name_of(dummy->children[1]->val));
        i=registerAlloc(get_name_of(dummy->children[1]->val),1);
        c1=getChild(c1,1);
        // is ignored for time being, but should be later used to allocate space for stack;
    }
    while(c2->children[0]->nodeKind != EMPTY)// handling statement list
    {
        node *tmp=getChild(getChild(c2,0),0);
        c2 = getChild(c2,1);
        statement(tmp);
    }
}
void functionDecl(node * fundecl)
{
    int i, j;
    // writing the function name as comment
    char * str = get_name_of(fundecl->children[1]->val);
    emit_comment(str);
    fprintf(fp,"\n%s:",str);
    fprintf(fp,"addi $sp,$sp,-100");//setting stack space for the function
    
    //setting the registers
    if(fundecl->numChildren > 2 && fundecl->children[2]->nodeKind == FORMALDL)
    {
        node *tmp1=getChild(fundecl,2);
        int i=0;
        while(tmp1->numChildren == 2)
        {
            node *tmp2 = getChild(getChild(tmp1,0),1);
            char *str = get_name_of(tmp2->val);
            registerAlloc(str,1);
            tmp1 = getChild(tmp1,1);
        }
    }
    //call the function body code gen function
    if(fundecl->children[2]->nodeKind == FUNBODY)
        functionBody(getChild(fundecl,2));
    else
        functionBody(getChild(fundecl,3));

    // placing the return value onto stack has been handled  
    
    //jumping back after the function has reached the return statement.
    if(strcmp(str,"main")== 0)
    {
        fprintf(fp,"\nli $v0,10");
        fprintf(fp,"\nsyscall");
    } 
    else
    {
        fprintf(fp,"\njr $ra");
    }    
}
int functionCall(node *n)
{
    int i=1,j=1;
    node *tmp = getChild(n,0);
    char *str = get_name_from_calltable(tmp->val);//get_name_of(tmp->val);
    char *name_list[20];
    //save the callers registers and empty the register array
    while(i<21)
    {
        printf("\ni = %d",i);
        fprintf(fp,"\nsw $%d,%d($fp)",i,i*(-4));
        if(reg[i-1].var_name != NULL)
            name_list[i-1] = strdup(reg[i-1].var_name);
        else
            name_list[i-1] = NULL;
        reg[i-1].var_name = NULL;
        i++;
    }
    fprintf(fp,"\nsw $ra,%d($fp)",i*(-4));//storing the return address register into stack;
    i++;
    fprintf(fp,"\nsw $fp,%d($fp)",i*(-4));//storing fp into stack;
    //load the actual parameters into register
    if(n->numChildren > 1)
    {
        i=1;
        tmp = getChild(n,1);
        while(tmp->numChildren !=0)
        {
            int result ;
            if(tmp->numChildren > 1)
                result = expr(getChild(tmp,1));
            else
                result = expr(getChild(tmp,0));
            fprintf(fp,"\nli $%d,%d",i++,result);
            tmp = getChild(tmp,0);
        }
    }
    
    //alocationg stack space
    fprintf(fp,"\naddi $sp,$sp,%d",-(4+4*20));//4-allocating space for return value, 4*40 -allocating space for registers
    
    //fp point to the return value location which is also top of the stack;
   //return address is managed using the register $ra
    
    //jump to function label
    fprintf(fp,"\njal %s",str);
    
    //restore callers register
    i=1;
    j=1;
    fprintf(fp,"\nlw $fp,%d($sp)",(-4));//restoring fp from stack;
    fprintf(fp,"\nlw $ra,%d($sp)",(-8));//restoring the return address register from stack;
    fprintf(fp,"\naddi $sp,$sp,%d",-12);
    while(i<21)
    {
        fprintf(fp,"\nlw $%d,%d($fp)",i,i*(-4));
        if(name_list[i-1] != NULL)
            reg[i-1].var_name = strdup(name_list[i-1]);
        i++;
    }

    //removing the allocated space
    fprintf(fp,"\nmove $sp,$fp");
    fprintf(fp,"\naddi $sp,$sp,%d",-4);
    
    //storing return valu in register 25
    fprintf(fp,"\nlw $25,0($fp)");

    return 25;
}
void printAsm()// not working
{
    fp = fopen("assembly.asm", "r");
    char c = fgetc(fp);
    while (c != EOF)
    {
        printf ("%c", c);
        c = fgetc(fp);
    }
 
    fclose(fp);
}
int registerAlloc(char *var,int scope)// return the next available register
{
    int x = 0;
    while(x < 20)
    {
        if(reg[x].var_name == NULL)
        {
            reg[x].var_name=strdup(var);
            reg[x].sp=scope;
            return x;
        }
        x++;
    }
    return memorySpill(var);
}
int findRegister(char * var_name)
{
    int x = 0;
    int tmp = -1;
    while(x < 20)
    {
        printf("%s == %s",reg[x].var_name,var_name);
        if(strcmp(reg[x].var_name,var_name) == 0)
        {
            if(reg[x].sp > 0)
                return x;// local value is given higher priority
            else
                tmp = x; // global value
        }
        x++;
    }
    return tmp;
}

int memorySpill(char *var)//store the local values in the stack in case of memory spill
{   
    var_list *element;
    
    int x = 20;
    int tmp = -1;
    while(x < 20)//checking if any global values are present in the registers
    {
        if(reg[x].sp == 0)
        {
            break;
        }
    }
    if(x < 20)// a register is found
    {
        int count =0,offset=0;
        // look for the position of the global variable in stack
        element = gvl->next;
        while(element && strcmp(element->var_name,var)!=0)
        {
            count++;
            element = element-> next;
        }
        offset = count *4;
        emit_offset("sw",x,offset,"gp");
        
        strcpy(reg[x].var_name,var);
        reg[x].sp=1;//scope is local
        return x;
    }
 // no register holding global value is found
        //storing the variable in the function variable list

        //searching if the var has a space already allocated into stack
        var_list *dummy = fvl;
        int offset=0;
        while(dummy && strcmp(dummy->var_name,var) != 0)
        {
            dummy = dummy->next;
            offset++;
        }
        if(dummy != NULL)//space has already been allocated into stack
        {
            emit_offset("sw",x,-4*offset,"sp");
        }
        else
        {
            element = (var_list *) malloc (sizeof ( struct var_list));
            element->var_name = strdup(var);
            element->next = fvl->next;
            fvl->next = element;
            // move variable to stack
            emit_offset("sw",x,-4,"sp");
            fprintf(fp,"\n addi $sp,$sp,-4");
        }
       
       //returning the register
        strcpy(reg[x].var_name,var);
        reg[x].sp=1;//scope is local
        return x;
}

void emit_comment(char *str)
{
    fprintf(fp,"\n#");
    fprintf(fp,"%s",str);
}

/*void initcodegen(node *p)
{
    fp = fopen("assembly.asm", "w+");
    fprintf(fp,"\n# assembly code");
    fprintf(fp,"\n.text");
    fprintf(fp,"\nmain");
    codegen(p);

    fclose(fp);
}
*/
void codegen(node *parent)
{
    int i=0;
    node *n;

    fvl = (var_list *) malloc (sizeof ( struct var_list));
    fvl->next = NULL;
    gvl = (var_list *) malloc (sizeof ( struct var_list));
    gvl->next = NULL;

    fp = fopen("assembly.asm", "w+");
    fprintf(fp,"\n# assembly code");
    fprintf(fp,"\n.text");
    fprintf(fp,"\nj main");

    parent = getChild(parent,0);
    //printf("num %d",parent->numChildren);
    while(i < parent->numChildren)
    {
        n = getChild(parent,i);
        i++;
        printf("loop %d",i);
        switch (n->nodeKind)
        {
            case FUNDECL:
                functionDecl(n);
                break;
        }
    }

    //printf("\n calling codegen ");
   /* for (i = 0; i < n->numChildren; i++) 
        codegen(getChild(n, i));
        switch (n->nodeKind)
        {
          case FUNDECL:
            //functionDecl(n);
            break;
          case STM:
            statement(n);
            break;
         }*/
}


void emit_i(char *str,int r1,int num)
{
    fprintf(fp,"\n%s $%d,%d",str,r1,num);
}
void emit_label(char *str,char *str2)
{
    fprintf(fp,"\n%s %s",str,str2);
}
void emit_r2_label(char *str,int r1,int r2,char *str2)
{
    fprintf(fp,"\n%s $%d,$%d,%s",str,r1,r2,str2);
}
void emit_r2(char *str,int r1,int r2)
{
    fprintf(fp,"\n%s $%d,$%d",str,r1,r2);
}
void emit_r3(char *str,int r1,int r2,int r3)
{
    fprintf(fp,"\n%s $%d,$%d,$%d",str,r1,r2,r3);
}
void emit_offset(char *str,int r1,int offset,char* pointer)
{
    fprintf(fp,"\n%s $%d,%d,(%s)",str,r1,offset,pointer);
}
