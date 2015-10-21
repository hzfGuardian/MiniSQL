
%option yylineno case-insensitive

%{

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "../SQLGrammarTree.h"
#include "Analysis.hpp"

extern void yyerror(const char* s, ...);

%}

%x COMMENT

%%

and     {
                return AND;   
        }

on      {
                return ON;   
        }

select  {   
				yylval.pNode = MallocNewNode();
                yylval.pNode->type=SELECT;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"select");
                return SELECT;   
        }

from    {
                return FROM;   
        }

where   {
                return WHERE;   
        }

drop    {   
				yylval.pNode = MallocNewNode();
                yylval.pNode->type=DROP;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"drop");
                return DROP;   
        }

table   {   
				yylval.pNode = MallocNewNode();
                yylval.pNode->type=TABLE;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"table");
                return TABLE;   
        }

create  {   
				yylval.pNode = MallocNewNode();
                yylval.pNode->type=CREATE;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"create");
                return CREATE;   
        }

index   {   
				yylval.pNode = MallocNewNode();
                yylval.pNode->type=INDEX;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"index");
                return INDEX;   
        }

primary {
                return PRIMARY;   
        }

key     {   
                return KEY;   
        }

values  {
                return VALUES;   
        }

unique  {   
				yylval.pNode = MallocNewNode();
                yylval.pNode->type=UNIQUE;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"unique");
                return UNIQUE;   
        }

insert  {   
				yylval.pNode = MallocNewNode();
                yylval.pNode->type=INSERT;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"insert");
                return INSERT;   
        }

into	{
                return INTO; 
		}

delete  {   
				yylval.pNode = MallocNewNode();
                yylval.pNode->type=DELETE;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"delete");
                return DELETE;   
        }

^quit   {   
				yylval.pNode = MallocNewNode();
                yylval.pNode->type=QUIT;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"quit");
                return QUIT;   
        }

execfile  {   
				yylval.pNode = MallocNewNode();
                yylval.pNode->type=EXECFILE;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"execfile");
                return EXECFILE;   
        }

int     {   
				yylval.pNode = MallocNewNode();
                yylval.pNode->type=INT;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"int");
                return INT;   
        }

float   {   
				yylval.pNode = MallocNewNode();
                yylval.pNode->type=FLOAT;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"float");
                return FLOAT;   
        }

char    {   
				yylval.pNode = MallocNewNode();
                yylval.pNode->type=CHAR;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"char");
                return CHAR;   
        }


-?[0-9]+    {   
				yylval.pNode = MallocNewNode();
                yylval.pNode->type=INTNUM;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"%s",yytext);
                return INTNUM;   
        }


-?[0-9]+"."[0-9]* |
-?"."[0-9]+     |   
-?[0-9]+E[-+]?[0-9]+ |  
-?[0-9]+"."[0-9]*E[-+]?[0-9]+   |       
-?"."[0-9]+E[-+]?[0-9]+         {   
				yylval.pNode = MallocNewNode();
                yylval.pNode->type=FLOATNUM;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"%s",yytext);
                return FLOATNUM;   
        }


'(\\.|''|[^'\n])*'  |
\"(\\.|\"\"|[^"\n])*\"          {   
				yylval.pNode = MallocNewNode();
                yylval.pNode->type=STRING;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"%s",yytext+1);
                yylval.pNode->text[yyleng-2]=0;
                return STRING;   
        }

'(\\.|[^'\n])*$                 {   yyerror("Unterminated string %s", yytext);  }
\"(\\.|[^"\n])*$                 {   yyerror("Unterminated string %s", yytext);  }



[-+&~|^/%()*,.;!]               {   return yytext[0];   }

"<>"   {   
				yylval.pNode = MallocNewNode();
                yylval.pNode->type=COMPARISON;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"%s",yytext);
                return COMPARISON;   
        }	

"<="   {   
				yylval.pNode = MallocNewNode();
                yylval.pNode->type=COMPARISON;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"%s",yytext);
                return COMPARISON;   
        }

">="   {   
				yylval.pNode = MallocNewNode();
                yylval.pNode->type=COMPARISON;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"%s",yytext);
                return COMPARISON;   
        }

"<"   {   
				yylval.pNode = MallocNewNode();
                yylval.pNode->type=COMPARISON;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"%s",yytext);
                return COMPARISON;   
        }

">"   {   
				yylval.pNode = MallocNewNode();
                yylval.pNode->type=COMPARISON;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"%s",yytext);
                return COMPARISON;   
        }

"="   {   
				yylval.pNode = MallocNewNode();
                yylval.pNode->type=COMPARISON;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"%s",yytext);
                return COMPARISON;   
        }

[A-Za-z][A-Za-z0-9_]*   {   
				yylval.pNode = MallocNewNode();
                yylval.pNode->type=NAME;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"%s",yytext);
                return NAME;   
        }

`[^`/\\.\n]+`   {
				yylval.pNode = MallocNewNode();
                yylval.pNode->type=NAME;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"%s",yytext+1);
                yylval.pNode->text[yyleng-2]=0;
                return NAME;   
        }

`[^`\n]*$               {   yyerror("Unterminated name %s", yytext);  }

[ \t\n]                 ;

.                       {   yyerror("mystery character '%c'", *yytext);    }

%%

int yywrap()
{
	/*
    if(yyin == stdin)
        printf("yywrap called.\n");
    if(yyin != stdin) {
        fclose(yyin);
        return 0;
    }
    */
    return 1;
}



