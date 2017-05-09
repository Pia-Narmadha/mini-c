%{
  #include <symtab.h>
  #include <tree.h>
  #include <semchk.h>
  #include <stdio.h>
  #include<string.h>
  #include<Debug.h>


  int yylex();
  int yyerror();
  int yylineno; 

  int scope;

  enum dataType {INT_TYPE, CHAR_TYPE, VOID_TYPE, STRING_TYPE};
  enum constType {INT, CHAR, x, STR};
  enum relopType {LT, GT, GTE, LTE, EQ, NEQ};

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

  
  tree *ast;                    /* pointer to AST root */
  extern char *types[];

%}

%union 
{
  int value;
  struct treenode *node;
  char *strval;
}

%token <strval> ID
%token <value> INTCONST
%token <value> CHARCONST
%token <strval> STRCONST

/* keywords */
%token <value> KWD_IF
%token <value> KWD_ELSE   
%token <value> KWD_WHILE 
%token <value> KWD_INT
%token <value> KWD_STRING
%token <value> KWD_CHAR
%token <value> KWD_RETURN 
%token <value> KWD_VOID

/* operators */
%token <value> OPER_ADD
%token <value> OPER_SUB
%token <value> OPER_MUL
%token <value> OPER_DIV
%token <value> OPER_LT
%token <value> OPER_GT
%token <value> OPER_GTE
%token <value> OPER_LTE
%token <value> OPER_EQ
%token <value> OPER_NEQ
%token <value> OPER_ASGN


/* brackets & parens */
%token <value> LSQ_BRKT
%token <value> RSQ_BRKT
%token <value> LCRLY_BRKT
%token <value> RCRLY_BRKT
%token <value> LPAREN
%token <value> RPAREN

/* punctuation */
%token <value> COMMA
%token <value> SEMICLN


/*operators added*/
%token <value> OPER_ALIAS
%token <value> ILLEGAL_TOK
%token <value> ERROR
/*error token added*/
%token <value> ILLEGAL_STR


%type <node> program declList decl varDecl localVarDecl typeSpecifier funDecl formalDeclList formalDecl funBody localDeclList statementList statement compoundStmt assignStmt condStmt loopStmt returnStmt var expression relop addExpr addop term mulop factor funcCallExpr argList

%start program 

%%

program         : declList
                  { 
                    tree *progNode = maketree(PROGRAM);
                    addChild(progNode, $1);
                    ast = progNode;
                    funCallMatch();
                  }
                ;
declList        : decl 
                  { 
                    $$ = $1;
                   
                  }
                | declList decl
                  { 
                    D(printf("\n\tdecllist"));
                    $$ = $1;
                    addChild($$, $2);
                   
                  }
                ;
decl            : varDecl
                  { 
                    D(printf("\n\tdecl")); 
                    $$ = $1;
                    
                  }
                  
                | funDecl 
                  { 
                    D(printf("\n\tdecl"));
                    $$ = $1;
                  }
                ;
varDecl         : typeSpecifier ID LSQ_BRKT INTCONST RSQ_BRKT SEMICLN
                  { 
                    int index;
                    tree *varDecl = maketree(VARDECL); 
                    addChild(varDecl, $1);

                    index=insert_into_ST($2,$1->val,0);      //0 means scope is global
                    multiDeclIdChk(index);

                    addChild(varDecl, maketreeWithVal(IDENTIFIER, index));
                    addChild(varDecl, maketreeWithVal(ARR, $4));
                    ST[index].arraySize = $4;
                    $$=varDecl;
                  }
                | typeSpecifier ID SEMICLN
                  { 
                    int index;
                    tree *varDecl = maketree(VARDECL);
                    addChild(varDecl, $1);
                    index=insert_into_ST($2,$1->val,0);      //0 means scope is global
                    multiDeclIdChk(index);

                    addChild(varDecl, maketreeWithVal(IDENTIFIER,index));
                    $$=varDecl;
                  }
                ;
localVarDecl    : typeSpecifier ID LSQ_BRKT INTCONST RSQ_BRKT SEMICLN
                  { 
                    int index;
                    tree *localVarDecl = maketree(LOCALVARDECL); 
                    addChild(localVarDecl, $1);
                    
                    index=insert_into_ST($2,$1->val,scope+1);      //0 < scope means is local
                    multiDeclIdChk(index);

                    addChild(localVarDecl, maketreeWithVal(IDENTIFIER, index));
                    addChild(localVarDecl, maketreeWithVal(ARR, $4));
                    ST[index].arraySize = $4;
                    $$=localVarDecl;
                  }
                | typeSpecifier ID SEMICLN
                  { 
                    int index;
                    tree *localVarDecl = maketree(LOCALVARDECL);
                    addChild(localVarDecl, $1);

                    index=insert_into_ST($2,$1->val,scope+1);//0 < scope means is local
                    multiDeclIdChk(index);

                    addChild(localVarDecl, maketreeWithVal(IDENTIFIER,index)); 
                    $$=localVarDecl;
                  }
                ;
typeSpecifier   : KWD_INT
                  { 
                    D(printf("\n\ttypeSpecifier_int"));
                    $$ = maketreeWithVal(TYPESPEC, INT_TYPE);
                  }
                | KWD_CHAR
                  { 
                    D(printf("\n\ttypeSpecifier_char"));
                    $$ = maketreeWithVal(TYPESPEC, CHAR_TYPE);
                  }
                | KWD_VOID
                  { 
                    D(printf("\n\ttypeSpecifier_void"));
                    $$ = maketreeWithVal(TYPESPEC, VOID_TYPE);
                  }
                ;
funDecl         : typeSpecifier ID LPAREN formalDeclList RPAREN funBody 
                  { 
                      int index;
                      ++scope;
                      tree *funDecl = maketree(FUNDECL);
                      addChild(funDecl, $1);
                      
                      index = insert_into_ST($2,$1->val,0);// 0 means scope is global
                      add_function_signature_into_symtab(index,$1->val, $4);
                      multiDeclFunChk(index);
                      
                      addChild(funDecl, maketreeWithVal(IDENTIFIER, index));
                      addChild(funDecl, $4);
                      addChild(funDecl, $6);
                      $$=funDecl;
                  }
                | typeSpecifier ID LPAREN RPAREN funBody
                  { 
                    int index;
                    ++scope;
                    tree *funDecl = maketree(FUNDECL);
                    addChild(funDecl, $1);

                    index = insert_into_ST($2,$1->val,0);// 0 means scope is global
                    add_function_signature_into_symtab(index,$1->val ,NULL);
                    multiDeclFunChk(index);

                    addChild(funDecl, maketreeWithVal(IDENTIFIER, index));
                    addChild(funDecl, $5);
                    $$=funDecl;
                  }
                ;
formalDeclList  : formalDecl
                  { 
                    $$ = $1;
                  }
                | formalDecl COMMA formalDeclList
                  { 
                    $$ = $3;
                    addChild($$, $1);
                  }
                ;
formalDecl      : typeSpecifier ID
                  { 
                    int index;
                    tree *formalDecl = maketree(FORMALDECL);
                    addChild(formalDecl, $1);
                    index= insert_into_ST($2,$1->val,scope+1);// 0 < scope is local
                    multiDeclIdChk(index);
                    addChild(formalDecl, maketreeWithVal(IDENTIFIER, index));
                    $$=formalDecl;
                  }
                | typeSpecifier ID LSQ_BRKT RSQ_BRKT
                  { 
                    int index;
                    tree *formalDecl = maketree(FORMALDECL);
                    addChild(formalDecl, $1);
                    index= insert_into_ST($2,$1->val,scope+1);// 0 < scope is local
                    multiDeclIdChk(index);
                    D(printf("\nST insertion\n"));
                    addChild(formalDecl, maketreeWithVal(IDENTIFIER, index));
                    $$=formalDecl;
                  }
                ;
funBody         : LCRLY_BRKT localDeclList statementList RCRLY_BRKT
                  { 
                    tree *funBody = maketree(FUNBODY);
                    addChild(funBody, $2);
                    addChild(funBody, $3);
                    $$=funBody;
                  }
                ;
localDeclList   :  
                  {
                    tree *localDeclList = maketree(LOCALDL);
                    addChild(localDeclList, maketreeWithVal(EMPTY, EMPTY));
                    $$=localDeclList;
                  }
                | localVarDecl localDeclList
                  { 
                    tree *localDeclList = maketree(LOCALDL);
                    addChild(localDeclList, $1);
                    addChild(localDeclList, $2);

                    $$=localDeclList;
                  }
                ;
statementList   : 
                  { 
                    tree *statementList = maketree(STMLIST);
                    addChild(statementList,maketreeWithVal(EMPTY, EMPTY));
                    $$=statementList;
                  }
                | statement statementList
                  { 
                    tree *statementList = maketree(STMLIST);
                    addChild(statementList, $1);
                    addChild(statementList, $2);
                    $$=statementList;
                  }
                ;
statement       : compoundStmt
                  { 
                    tree *statement = maketree(STM);
                    addChild(statement, $1);
                    $$=statement;
                  }
                | assignStmt
                  { 
                    tree *statement = maketree(STM);
                    addChild(statement, $1);
                    $$=statement;
                  }
                | condStmt
                  { 
                    tree *statement = maketree(STM);
                    addChild(statement, $1);
                    $$=statement;
                  }
                | loopStmt
                  { 
                    D(printf("\n\t statement"));
                    tree *statement = maketree(STM);
                    addChild(statement, $1);
                    $$=statement;
                  }
                | returnStmt
                  { 
                    tree *statement = maketree(STM);
                    addChild(statement, $1);
                    $$=statement;
                  }
                ;
compoundStmt    : LCRLY_BRKT statementList RCRLY_BRKT
                  { 
                    tree *compoundStmt = maketree(COMSTM);
                    addChild(compoundStmt, $2);
                    $$=compoundStmt;
                  }
                ;
assignStmt      : var OPER_ASGN expression SEMICLN
                  { 
                    D(printf("\n\t assignStmt"));
                    tree *assignStmt = maketree(ASTM);
                    addChild(assignStmt, $1);
                    addChild(assignStmt, maketreeWithVal(ASGN, $2));
                    addChild(assignStmt, $3);
                    if ($1->val != -1 ) 
                    {
                      typeChk($1, getChild($3,0));
                    }
                    
                    $$=assignStmt;
                  }
                | expression SEMICLN
                  { 
                    tree *assignStmt = maketree(ASTM);
                    addChild(assignStmt, $1);
                    $$=assignStmt;
                  }
                ;
condStmt        : KWD_IF LPAREN expression RPAREN statement
                  { 
                    tree *condStmt = maketree(CONSTM);
                    addChild(condStmt, maketreeWithVal(IF_TYPE, $1));
                    addChild(condStmt, $3);
                    addChild(condStmt, $5);
                    $$=condStmt;
                  }
                | KWD_IF LPAREN expression RPAREN statement KWD_ELSE statement
                  { 
                    tree *condStmt = maketree(CONSTM);
                    addChild(condStmt, maketreeWithVal(IF_TYPE, $1));
                    addChild(condStmt, $3);
                    addChild(condStmt, $5);
                    addChild(condStmt, maketreeWithVal(ELSE_TYPE, $6));
                    addChild(condStmt, $7);
                    $$=condStmt;
                  }
                ;
loopStmt        : KWD_WHILE LPAREN expression RPAREN statement
                  { 
                    tree *loopStmt = maketree(LSTM);
                    addChild(loopStmt, maketreeWithVal(WHILE_TYPE, $1));
                    addChild(loopStmt, $3);
                    addChild(loopStmt, $5);
                    $$=loopStmt;
                  }
                ;
returnStmt      : KWD_RETURN SEMICLN
                  { 
                    tree *returnStmt = maketree(RSTM);
                    addChild(returnStmt, maketreeWithVal(RETURN_TYPE, $1));
                    $$=returnStmt;
                  }
                | KWD_RETURN expression SEMICLN
                  { 
                    tree *returnStmt = maketree(RSTM);
                    addChild(returnStmt, maketreeWithVal(RETURN_TYPE, $1));
                    addChild(returnStmt, $2);
                    $$=returnStmt;
                  }
                ;
var             : ID
                    { 
                      int index;
                      index=lookup_in_ST($1);
                      $$ = maketreeWithVal(IDENTIFIER, index);
                      if (index == -1) // this and repeated 3x below should be a f(x)
                      {
                        $$->val = -1;
                      }
                      else 
                      {
                        $$->val = getIDType(index);
                      }
                      idDeclChk($1,index,scope+1);
                    }
                | ID LSQ_BRKT addExpr RSQ_BRKT
                  { 
                    int index;
                    index=lookup_in_ST($1);
                    tree *var = maketreeWithVal(VAR, index);
                    addChild(var, maketreeWithVal(IDENTIFIER, index));
                    addChild(var, $3);
                    $$=var;
                    if (index == -1) 
                    {
                      $$->val = -1;;
                    }
                    else 
                    {
                      $$->val = getIDType(index);
                      if(oobArrChk(index, $3) < 1)
                        indexArrChk($3);
                      //oobArrChk(index, $3);
                    } 
                    idDeclChk($1,index,scope+1);
                  }
                ;
expression      : addExpr
                  { 
                    tree *expression = maketree(EXPR);
                    addChild(expression, $1);
                    $$=expression;
                    $$->val = $1->val;
                  }
                | expression relop addExpr
                  { 
                    tree *expression = maketree(EXPR);
                    addChild(expression, $1);
                    addChild(expression, $2);
                    addChild(expression, $3);
                    $$=expression;
                    $$->val = $3->val;
                  }
                ;
 relop          : OPER_LTE { $$ = maketreeWithVal(RELOP, LTE); }
                | OPER_LT { $$ = maketreeWithVal(RELOP, LT); }
                | OPER_GT { $$ = maketreeWithVal(RELOP, GT); }
                | OPER_GTE { $$ = maketreeWithVal(RELOP, GTE); }
                | OPER_EQ { $$ = maketreeWithVal(RELOP, EQ); }
                | OPER_NEQ { $$ = maketreeWithVal(RELOP, NEQ); }
                ;
addExpr         : term
                 { 
                    $$ = $1;
                    $$->val = $1->val;
                  }
                | addExpr addop term
                  { 
                    tree *addExpr = maketree(ADDEXPR);
                    addChild(addExpr, $1);
                    addChild(addExpr, $2);
                    addChild(addExpr, $3);
                    $$=addExpr;
                      $$->val = !($1->val==0 && $3->val==0)*-1;
                  }
                ;
addop           : OPER_ADD { $$ = maketreeWithVal(ADDOP, ADD); }
                | OPER_SUB { $$ = maketreeWithVal(ADDOP, SUB); }
                ;
term            : factor
                  { 
                    $$ = $1;
                    $$->val = $1->val;
                  }
                | term mulop factor
                  { 
                    tree *term = maketree(TERM);
                    addChild(term, $1);
                    addChild(term, $2);
                    addChild(term, $3);
                    $$=term;
                    $$->val = !($1->val==0 && $3->val==0)*-1;
                  }
                ;
mulop           : OPER_MUL { $$ = maketreeWithVal(MULOP, MUL); }
                | OPER_DIV { $$ = maketreeWithVal(MULOP, DIV); }
                ;
factor          : LPAREN expression RPAREN
                  { 
                    tree *factor = maketree(FACTOR);
                    addChild(factor, $2);
                    $$=factor;
                    $$->val = $2->val;
                  }
                | var
                  { 
                    tree *factor = maketree(FACTOR);
                    addChild(factor, $1);          
                    $$=factor;
                    $$->val = $1->val;
                  }
                | funcCallExpr
                  { 
                    tree *factor = maketree(FACTOR);
                    addChild(factor, $1);          
                    $$=factor;
                    $$->val = $1->val;
                  }
                | INTCONST { $$ = maketreeWithVal(FACTOR, INT);$$->val= $1; }
                | CHARCONST { $$ = maketreeWithVal(FACTOR, CHAR); $$->val = 1;}
                | STRCONST { $$ = maketreeWithVal(FACTOR, STR); $$->val=2;}
                ;
funcCallExpr    : ID LPAREN argList RPAREN
                  { 
                    int index;
                    tree *funcCallExpr = maketree(FUNCALLEXPR);
                    addChild(funcCallExpr, maketreeWithVal(IDENTIFIER, index));
                    addChild(funcCallExpr, $3);
                    $$=funcCallExpr; 
                    insertInCallTable(yylineno,$1,$3);
                  }
                | ID LPAREN RPAREN
                  { 
                    tree *funcCallExpr = maketree(FUNCALLEXPR);
                    int index;

                    addChild(funcCallExpr, maketreeWithVal(IDENTIFIER, index));
                    $$=funcCallExpr;

                    insertInCallTable(yylineno,$1,NULL);
                  }
                ;
argList         : expression
                  { 
                    tree *argList = maketree(ARGL);
                    addChild(argList, $1);
                    $$=argList;
                  }
                | argList COMMA expression
                  { 
                    tree *argList = maketree(ARGL);
                    addChild(argList, $1);
                    addChild(argList, $3);
                    $$=argList;
                  }
                ;
%%

int yyerror(char * msg) {
  printf("\nError at line %d: %s\n", yylineno, msg);
  return 0;
}



