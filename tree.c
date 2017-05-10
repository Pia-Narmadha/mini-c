#include"tree.h"
#include<stdio.h>
#include<stdlib.h>
#include"symtab.h"
enum dataType {INT_TYPE, CHAR_TYPE, VOID_TYPE, STRING_TYPE};
enum operators {ADD, SUB, MUL, DIV, GT, LT, GTE, LTE, NEQ, EQ, ASSGN};
char *oper[11] = {"+", "-", "*", "/", ">", "<", ">=", "<=", "!=", "==", "=" };
char *types[4] = {
  "INT_TYPE", "CHAR_TYPE", "VOID_TYPE","STRING_TYPE"
};

/* string values for ast node types, makes tree output more readable */

char *nodeNames[41] = {
  "program", "declList", "decl", "vardecl",
  "typeSpecfier",  
  "funDecl", "formalDeclList", "formalDecl", "funBody", "localDeclList", "localVarDecl", 
  "statementList", "statement", "compoundStmt", "assignStmt", "ASGN",
  "condStmt", "IF_TYPE", "IF_ELSE_TYPE", "loopStmt", "WHILE_TYPE", 
  "returnStmt", "RETURN_TYPE", 
  "var", "identifier",
  "expression",
  "relop", 
  "addExpr", "addop", "ADD", "SUB",
  "term","mulop", "MUL", "DIV", 
  "factor", "integer", "ARR", 
  "funcCallExpr", "argList",
  "EMPTY"
};

tree *maketree(int kind) {
  tree *this = (tree *) malloc(sizeof(struct treenode));
  this->nodeKind = kind;
  this->numChildren = 0;
  return this;

}

tree *maketreeWithVal(int kind, int val) {
  tree *this = (tree *) malloc(sizeof(struct treenode));
  this->nodeKind = kind;
  this->numChildren = 0;
  this->val = val;
  return this;

}

void addChild(tree *parent, tree *child) {
  if (parent->numChildren == MAXCHILDREN) {
    printf("Cannot add child to parent node\n");
    exit(1);
  }
  nextAvailChild(parent) = child;
  parent->numChildren++;
  
}

void printAst(tree *node, int nestLevel) {

  switch(node->nodeKind) {
  case 36:
    printf("<%d>\n", node->val);
    break;
  case 4:
    printf("<%s>\n", types[node->val]);
    break;
  case 27:
    printf("<%s>\n", oper[node->oper]);
    break;
  case 31:
    printf("<%s>\n", oper[node->oper]);
    break;
  case 24:
    printf("<%s>\n", get_name_of(node->val));
    break;
  case 14:
    printf("<%s>\n", oper[node->oper]);
    break;
  default:
    printf("%s\n", nodeNames[node->nodeKind]);
    break;
  }

  int i, j;

  for (i = 0; i < node->numChildren; i++)  {
    for (j = 0; j < nestLevel; j++) 
      printf("\t");
    printAst(getChild(node, i), nestLevel + 1);
  }
}


