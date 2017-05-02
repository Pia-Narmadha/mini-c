#include"tree.h"
#include<stdio.h>
#include<stdlib.h>

enum dataType {INT_TYPE, CHAR_TYPE, VOID_TYPE, STRING_TYPE};

char *types[4] = {
  "INT_TYPE", "CHAR_TYPE", "VOID_TYPE","STRING_TYPE"
};

/* string values for ast node types, makes tree output more readable */

char *nodeNames[40] = {
  "program", "declList", "decl", "vardecl",
  "typeSpecfier",  
  "funDecl", "formalDeclList", "formalDecl", "funBody", "localDeclList", "localVarDecl", 
  "statementList", "statement", "compoundStmt", "assignStmt", "ASGN",
  "condStmt", "IF_TYPE", "ELSE_TYPE", "loopStmt", "WHILE_TYPE", 
  "returnStmt", "RETURN_TYPE", 
  "var", "identifier",
  "expression",
  "relop", 
  "addExpr", "addop", "ADD", "SUB",
  "term","mulop", "MUL", "DIV", 
  "factor", "ARR", 
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

  // if (nodeNames[node->nodeKind] == "typeSpecifier")
  if (node->nodeKind == 4)
    printf("%s ", types[node->val]);

  printf("%s\n", nodeNames[node->nodeKind]);
  /*if(node->nodeKind == 25 )
    printf("%s type:%d",nodeNames[node->nodeKind],node->val);*/
  int i, j;

  for (i = 0; i < node->numChildren; i++)  {
    for (j = 0; j < nestLevel; j++) 
      printf("\t");
    printAst(getChild(node, i), nestLevel + 1);
  }
}


