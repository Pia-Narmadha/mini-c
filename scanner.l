/* definitions */

%{
#include<stdio.h>
#include"y.tab.h"
#include <string.h>
#include<stdlib.h>

int yycol=1;
char *buf;
char *s="";
void convert_to_char();
%}

digit       [0-9]
leadzero    0{digit}+
number      0|[1-9]{digit}*
letter      [a-zA-Z]
identifier  {letter}({letter}|{digit})*
escapesequence	\\(n|t|\\|\")
character1	"'"{escapesequence}"'"
character	(("'")(.)("'"))
whitespace  [ \n\t]
newline		[\n]
%x STRING 
%x COMMENT

%%

 /* rules */ 
 /* <STRING> is a state and when begin string is seen the scanner goes into string state 
  and does the opertinons in it until it see begin 0  */

\"              { BEGIN STRING; s = buf; }
<STRING>\\\\    { *s++ = '\\'; }
<STRING>\\n     { *s++ = '\n'; }
<STRING>\\t     { *s++ = '\t'; }
<STRING>\\\"    { *s++ = '\"'; }
<STRING>\"      { 
                  *s = 0;
                  BEGIN 0;
		  		  yylval.strval=strdup(buf);
                  return STRCONST;
                }
<STRING>\n      { printf("unterminated string at line %d col %d\n",yylineno,yycol); exit(1); }
<STRING>.       { *s++ = *yytext; }

 /* <COMMENT> is a state and when begin comment is seen the scanner goes into comment state 
  and does the opertinons in it until it see begin 0  */

"/\*"		{  BEGIN COMMENT;}
<COMMENT>.	{}
<COMMENT>\n	{}
<COMMENT><<EOF>> {printf("unterminated comment at line %d col %d\n",yylineno,yycol);yyterminate();}
<COMMENT>\*"/"	{BEGIN 0;}

 /*KEYWORD*/

"if"		{return KWD_IF;}
"else"		{return KWD_ELSE;}
"while"		{return KWD_WHILE;}
"int"		{return KWD_INT;}
"string"	{return KWD_STRING;}
"char"		{return KWD_CHAR;}
"return"	{return KWD_RETURN;}
"void"		{return KWD_VOID;}

 /*identifier*/

{identifier}    {yylval.strval=strdup(yytext);return ID;}

 /*Integer constant*/
{leadzero}	{return ERROR;}
{number}        {yylval.value=atoi(yytext);return INTCONST;}

 /*character constant*/
{character}	{yylval.strval=strdup(yytext+1);yylval.strval[yyleng-2]=0;return CHARCONST;}
{character1}	{yylval.strval=strdup(yytext+1);yylval.strval[yyleng-2]=0;
		convert_to_char();
		return CHARCONST;}

 /*operators*/
"+"             {return OPER_ADD;}
"-"             {return OPER_SUB;}
"*"             {return OPER_MUL;}
"/"             {return OPER_DIV;}
"<"             {return OPER_LT;}
">"             {return OPER_GT;}
"<="             {return OPER_LTE;}
">="             {return OPER_GTE;}
"=="             {return OPER_EQ;}
"!="             {return OPER_NEQ;}
"="             {return OPER_ASGN;}
\[             {return LSQ_BRKT;}
\]             {return RSQ_BRKT;}
"{"             {return LCRLY_BRKT;}
"}"             {return RCRLY_BRKT;}
\(             {return LPAREN ;}
\)             {return RPAREN ;}
","             {return COMMA;}
";"             {return SEMICLN;}
"@"             {return OPER_ALIAS;}
 
 /*comment*/
 "//".*        {}

 /*Space handling*/
{newline}	{yylineno++;yycol=1;}
{whitespace}    {/* skip whitespace */}
 



%%

void convert_to_char()
{
		if(strcmp(yylval.strval,"\\t")==0)
			yylval.strval="\t";
		else if(strcmp(yylval.strval,"\\n")==0)
			yylval.strval="\n";
		else if(strcmp(yylval.strval,"\\\\")==0)
			yylval.strval="\\";
		else if(strcmp(yylval.strval,"\\\"")==0)
			yylval.strval="\"";
		yylval.strval[yyleng-3]=0;
}



