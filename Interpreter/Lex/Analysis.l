
%option yylineno case-insensitive

%{

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "../SQLGrammarTree.h"
#include "../../MiniSQL.h"
#include "Analysis.hpp"

extern void yyerror(const char* s, ...);
int finished_state = 0;

%}

%x COMMENT

%%


and     {
				finished_state = 0;
                return AND;   
        }

on      {
				finished_state = 0;
                return ON;   
        }

select  {   
				finished_state = 0;
				yylval.pNode = MallocNewNode();
                yylval.pNode->type=SELECT;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"select");
                return SELECT;   
        }

from    {
                finished_state = 0;
                return FROM;   
        }

where   {
                finished_state = 0;
                return WHERE;   
        }

drop    {   
				finished_state = 0;
				yylval.pNode = MallocNewNode();
                yylval.pNode->type=DROP;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"drop");
                return DROP;   
        }

table   {   
				finished_state = 0;
				yylval.pNode = MallocNewNode();
                yylval.pNode->type=TABLE;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"table");
                return TABLE;   
        }

create  {   
				finished_state = 0;
				yylval.pNode = MallocNewNode();
                yylval.pNode->type=CREATE;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"create");
                return CREATE;   
        }

index   {   
				finished_state = 0;
				yylval.pNode = MallocNewNode();
                yylval.pNode->type=INDEX;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"index");
                return INDEX;   
        }

primary {
                finished_state = 0;
                return PRIMARY;   
        }

key     {   
                finished_state = 0;
                return KEY;   
        }

values  {
                finished_state = 0;
                return VALUES;   
        }

unique  {   
				finished_state = 0;
				yylval.pNode = MallocNewNode();
                yylval.pNode->type=UNIQUE;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"unique");
                return UNIQUE;   
        }

insert  {   
				finished_state = 0;
				yylval.pNode = MallocNewNode();
                yylval.pNode->type=INSERT;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"insert");
                return INSERT;   
        }

into	{
                finished_state = 0;
                return INTO; 
		}

delete  {   
				finished_state = 0;
				yylval.pNode = MallocNewNode();
                yylval.pNode->type=DELETE;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"delete");
                return DELETE;   
        }

^quit   {   
				finished_state = 0;
				yylval.pNode = MallocNewNode();
                yylval.pNode->type=QUIT;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"quit");
                return QUIT;   
        }

execfile  {   
				finished_state = 0;
				yylval.pNode = MallocNewNode();
                yylval.pNode->type=EXECFILE;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"execfile");
                return EXECFILE;   
        }

int     {   
				finished_state = 0;
				yylval.pNode = MallocNewNode();
                yylval.pNode->type=INT;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"int");
                return INT;   
        }

float   {   
				finished_state = 0;
				yylval.pNode = MallocNewNode();
                yylval.pNode->type=FLOAT;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"float");
                return FLOAT;   
        }

char    {   
				finished_state = 0;
				yylval.pNode = MallocNewNode();
                yylval.pNode->type=CHAR;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"char");
                return CHAR;   
        }

null    {
                finished_state = 0;
                yylval.pNode = MallocNewNode();
                yylval.pNode->type=EMPTY;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"null");
                return EMPTY; 
        }

-?[0-9]+    {   
				finished_state = 0;
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
				finished_state = 0;
				yylval.pNode = MallocNewNode();
                yylval.pNode->type=FLOATNUM;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"%s",yytext);
                return FLOATNUM;   
        }


'(\\.|''|[^'\n])*'  |
\"(\\.|\"\"|[^"\n])*\"          {   
				finished_state = 0;
				yylval.pNode = MallocNewNode();
                yylval.pNode->type=STRING;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"%s",yytext+1);
                yylval.pNode->text[yyleng-2]=0;
                return STRING;   
        }

'(\\.|[^'\n])*$                 {   finished_state = 0;	yyerror("Unterminated string %s", yytext);  }
\"(\\.|[^"\n])*$                 {   finished_state = 0;	yyerror("Unterminated string %s", yytext);  }

[;]								{	finished_state = 1;	return yytext[0];	}

[-+&~|^/%()*,.!]               {   finished_state = 0;	return yytext[0];   }

"<>"   {   
				finished_state = 0;
				yylval.pNode = MallocNewNode();
                yylval.pNode->type=COMPARISON;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"%s",yytext);
                return COMPARISON;   
        }	

"<="   {   
				finished_state = 0;
				yylval.pNode = MallocNewNode();
                yylval.pNode->type=COMPARISON;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"%s",yytext);
                return COMPARISON;   
        }

">="   {   
				finished_state = 0;
				yylval.pNode = MallocNewNode();
                yylval.pNode->type=COMPARISON;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"%s",yytext);
                return COMPARISON;   
        }

"<"   {   
				finished_state = 0;
				yylval.pNode = MallocNewNode();
                yylval.pNode->type=COMPARISON;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"%s",yytext);
                return COMPARISON;   
        }

">"   {   
				finished_state = 0;
				yylval.pNode = MallocNewNode();
                yylval.pNode->type=COMPARISON;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"%s",yytext);
                return COMPARISON;   
        }

"="   {   
				finished_state = 0;
				yylval.pNode = MallocNewNode();
                yylval.pNode->type=COMPARISON;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"%s",yytext);
                return COMPARISON;   
        }

[A-Za-z][A-Za-z0-9_]*   {   
				finished_state = 0;
				yylval.pNode = MallocNewNode();
                yylval.pNode->type=NAME;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"%s",yytext);
                return NAME;   
        }

`[^`/\\.\n]+`   {
				finished_state = 0;
				yylval.pNode = MallocNewNode();
                yylval.pNode->type=NAME;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"%s",yytext+1);
                yylval.pNode->text[yyleng-2]=0;
                return NAME;   
        }

`[^`\n]*$               {  finished_state = 0; yyerror("Unterminated name %s", yytext);  }

[\n]			{	
				if(yyin == stdin) {
					if(finished_state)	
						printf(PROMPT);	
					else
						printf(ENTER_SIGN);
				}
		}
				

[ \t]                 {	finished_state = 0;	}

.                       {  finished_state = 0; yyerror("mystery character '%c'", *yytext);    }

%%

int yywrap()
{
    if(yyin == stdin)
        printf("yywrap called.\n");
    else {
        yyin = stdin;
        return 0;
    }

    return 1;
}



