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
st              (select|SELECT|Select)
fm              (from|FROM|From)
wh              (where|WHERE|Where)
terminator      [;]
nsplit          [,]
whsplit         (and|AND|or|OR|Or|And)
operator        [<>=|(<=)|(>=)]

%%

 
{st}  {
                yylval.pNode = MallocNewNode();
                yylval.pNode->type=ST;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"select");
                return ST;
        }

{fm}    {
                yylval.pNode = MallocNewNode();
                yylval.pNode->type=FM;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"from");
                return FM;
        }

{wh}    {
                yylval.pNode = MallocNewNode();
                yylval.pNode->type=WH;
                snprintf(yylval.pNode->text,sizeof(yylval.pNode->text),"where");
                return WH;
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




