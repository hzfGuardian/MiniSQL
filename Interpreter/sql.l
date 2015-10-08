%{

#include <stdio.h>
#include "sql.h"
#include "sql.tab.h"

extern "C" {

	int yylex(void);

}

%}
         
number          [-]?[0-9]+(\.[0-9]+)?
name            [a-zA-Z][a-zA-Z0-9]*
whitespace      [ \n\t]
terminator      [;]
nsplit          [,]
whsplit         (and|or)
operator        [<>=|(<=)|(>=)]
lbrace          [(]
rbrace          [)]

%%

{lbrace}  {
                yylval.pNode = MallocNewNode();
                yylval.pNode->type=LBRACE;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"(");
                return LBRACE;
        }

{rbrace}  {
                yylval.pNode = MallocNewNode();
                yylval.pNode->type=RBRACE;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),")");
                return RBRACE;
        }

select  {
                yylval.pNode = MallocNewNode();
                yylval.pNode->type=ST;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"select");
                return ST;
        }

from    {
                yylval.pNode = MallocNewNode();
                yylval.pNode->type=FM;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"from");
                return FM;
        }

where    {
                yylval.pNode = MallocNewNode();
                yylval.pNode->type=WH;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"where");
                return WH;
        }

drop  {
                yylval.pNode = MallocNewNode();
                yylval.pNode->type=DP;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"drop");
                return CT; 
    }

table  {
                yylval.pNode = MallocNewNode();
                yylval.pNode->type=TABLE;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"table");
                return TABLE; 
    }

create  {
                yylval.pNode = MallocNewNode();
                yylval.pNode->type=CT;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"create");
                return CT; 
    }

index  {
                yylval.pNode = MallocNewNode();
                yylval.pNode->type=IDX;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"index");
                return IDX; 
    }

primary  {
                yylval.pNode = MallocNewNode();
                yylval.pNode->type=PRIMARY;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"primary");
                return PRIMARY; 
    } 

key  {
                yylval.pNode = MallocNewNode();
                yylval.pNode->type=KEY;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"key");
                return KEY; 
    } 


int  {
                yylval.pNode = MallocNewNode();
                yylval.pNode->type=INT;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"int");
                return INT; 
    }

float  {
                yylval.pNode = MallocNewNode();
                yylval.pNode->type=FLOAT;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"float");
                return FLOAT; 
    }

char  {
                yylval.pNode = MallocNewNode();
                yylval.pNode->type=CHAR;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"char");
                return CHAR; 
    }

{whsplit} {
                yylval.pNode = MallocNewNode();
                yylval.pNode->type=WHSPLIT;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"%s",yytext);
                return WHSPLIT;
          }

{operator} {
                yylval.pNode = MallocNewNode();
                yylval.pNode->type= OPERATOR;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"%s",yytext);
                return OPERATOR;
           }

{nsplit}       ;

{name}         {
                printf("%s\n", yytext);
                yylval.pNode = MallocNewNode();
                yylval.pNode->type=NAME;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"%s",yytext);
                return NAME;
               }

{number}      {
                yylval.pNode = MallocNewNode();
                yylval.pNode->type=NUMBER;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"%s",yytext);
                return NUMBER;
             }


{terminator}    {
                        return TERMINATOR;
                }

{whitespace}    ;

.            ;

%%


int yywrap()
{
     return 1;
}




