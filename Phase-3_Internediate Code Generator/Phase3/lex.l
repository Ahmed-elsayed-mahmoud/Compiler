%{
	#include "y.tab.h"
	#include <bits/stdc++.h>
	int lineCounter = 1;

	extern int printLineNumber(int num);
%}
letter  [a-zA-Z]
digit [0-9]
digits {digit}+
id {letter}({letter}|{digit})*
num "-"?{digits}
fnum "-"?{digits}.{digits}("E"{digits}+)?
relop "=="|"!="|">"|">="|"<"|"<="
boolop "&&"|"||"
op "+"|"-"|"*"|"/"|"%"|"&"|"|"
binary "true"|"false"
%%
"System.out.println"	{return SYSTEM_OUT;}
"if"	{return IF_WORD;}
"else"	{return ELSE_WORD;}
"while"	{return WHILE_WORD;}
"for"	{return FOR_WORD;}
"int"	{return INT_WORD;}
"float"	{return FLOAT_WORD;}
"boolean" {return BOOLEAN_WORD;}
{op}	{yylval.aopval = strdup(yytext); return ARITH_OP;}
{num}	{yylval.ival = atoi(yytext); return INT;}
{fnum}	{yylval.fval = atof(yytext); return FLOAT;}
{relop} {yylval.aopval = strdup(yytext); return RELA_OP;}
{boolop} {yylval.aopval = strdup(yytext); return BOOL_OP;}
{binary} {if(!strcmp(yytext,"true")){ yylval.bval = 1;} else { yylval.bval = 0;} return BOOL;}
{id}	{yylval.idval = strdup(yytext);return IDENTIFIER;}
";" { return SEMI_COLON;}
"=" {return EQUALS;}
"(" { return LEFT_BRACKET;}
")" {return RIGHT_BRACKET;}
"{" {return LEFT_BRACKET_CURLY;}
"}" {return RIGHT_BRACKET_CURLY;}
\n	{++lineCounter;	printLineNumber(lineCounter);}
%%


int yywrap() {
   // open next reference or source file and start scanning
   return -1;
}
