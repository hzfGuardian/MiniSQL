%{

#include "../SQLGrammarTree.h"
#include <cstdio>
#include <cstdlib>
#include <cstdarg>

using namespace std;

extern int yylex();
extern void yyrestart(FILE* f);
void yyerror(const char *s, ...);

extern int yylineno;
extern FILE* yyin;
extern char* yytext;


%}



%union {
	struct SQLGrammarTree *pNode;
}


%token AND ON
%token SELECT FROM WHERE DROP TABLE CREATE INDEX PRIMARY KEY VALUES UNIQUE INSERT INTO DELETE QUIT EXECFILE

%token INT FLOAT CHAR

%token NAME
%token STRING
%token INTNUM
%token FLOATNUM
%token EMPTY

%token COMPARISON


%token STMT
%token ATTR_INFO ATTR_LIST ATTR DATA_TYPE 
%token ATTR_VALUE_LIST 
%token WH_LIST WH_NAME




%type<pNode> AND ON
%type<pNode> SELECT FROM WHERE DROP TABLE CREATE INDEX PRIMARY KEY VALUES UNIQUE INSERT INTO DELETE QUIT EXECFILE
%type<pNode> INT FLOAT CHAR
%type<pNode> NAME STRING INTNUM FLOATNUM EMPTY

%type<pNode> COMPARISON 

%type<pNode> STMT
%type<pNode> ATTR_INFO ATTR_LIST ATTR DATA_TYPE 
%type<pNode> ATTR_VALUE_LIST 
%type<pNode> WH_LIST WH_NAME


/* %type<pNode> program */
%type<pNode> error
%type<pNode> stmt_list stmt 
%type<pNode> attr_info attr_list attr data_type 
%type<pNode> attr_value_list 
%type<pNode> wh_list wh_name
%type<pNode> real_value



%%

stmt_list: error ';'		{	yyerror("grammar error."); nm_clear();	}
|	stmt_list error ';'		{	yyerror("grammar error."); nm_clear();	}


stmt_list: stmt ';'	{ProcessTree($1);nm_clear();}
|	stmt_list stmt ';'	{ProcessTree($2);nm_clear();}
;


stmt: CREATE TABLE NAME '(' attr_info ')'	{$$=NewFatherAddSon(STMT, 4, $1, $2, $3, $5);}

|	DROP TABLE NAME 						{$$=NewFatherAddSon(STMT, 3, $1, $2, $3);}

|	CREATE INDEX NAME ON NAME '(' NAME ')'	{$$=NewFatherAddSon(STMT, 5, $1, $2, $3, $5, $7);}

|	DROP INDEX NAME							{$$=NewFatherAddSon(STMT, 3, $1, $2, $3);}

|	INSERT INTO NAME VALUES '(' attr_value_list ')'	{$$=NewFatherAddSon(STMT, 3, $1, $3, $6);}

|	DELETE FROM NAME 		 				{$$=NewFatherAddSon(STMT, 2, $1, $3);}

|	DELETE FROM NAME WHERE wh_list			{$$=NewFatherAddSon(STMT, 3, $1, $3, $5);}

|	SELECT '*' FROM NAME 					{$$=NewFatherAddSon(STMT, 2, $1, $4);}

|	SELECT '*' FROM NAME WHERE wh_list 		{$$=NewFatherAddSon(STMT, 3, $1, $4, $6);}

|	EXECFILE NAME       					{$$=NewFatherAddSon(STMT, 2, $1, $2);}

|	QUIT									{nm_clear();	printf("Bye. See you next time.\n");	exit(0);}

;


attr_info: attr_list 						{$$=NewFatherAddSon(ATTR_INFO, 1, $1);}
|	attr_list ',' PRIMARY KEY '(' NAME ')'	{$$=NewFatherAddSon(ATTR_INFO, 2, $1, $6);}
;

attr_list: attr 							{$$=NewFatherAddSon(ATTR_LIST, 1, $1);}
|	attr_list ',' attr 						{$$=FatherAddSon($1, $3);}
;

attr: NAME data_type						{$$=NewFatherAddSon(ATTR, 2, $1, $2);}
|	NAME data_type UNIQUE					{$$=NewFatherAddSon(ATTR, 3, $1, $2, $3);}
;

data_type: INT 								{$$=NewFatherAddSon(DATA_TYPE, 1, $1);}
|	FLOAT 									{$$=NewFatherAddSon(DATA_TYPE, 1, $1);}
|	CHAR '(' INTNUM ')'						{$$=NewFatherAddSon(DATA_TYPE, 2, $1, $3);}
;




attr_value_list: real_value 				{$$=NewFatherAddSon(ATTR_VALUE_LIST, 1, $1);}
|	attr_value_list ',' real_value 			{$$=FatherAddSon($1, $3);}
;




wh_list: wh_name                          	{$$=NewFatherAddSon(WH_LIST, 1, $1);}
|	wh_list AND wh_name     				{$$=FatherAddSon($1, $3);}
;

wh_name: NAME COMPARISON real_value			{$$=NewFatherAddSon(WH_NAME, 3, $1, $2, $3);}


real_value: STRING 							{$$=$1;}
|	INTNUM									{$$=$1;}
|	FLOATNUM 								{$$=$1;}
|	EMPTY									{$$=$1;}
;



%%


void yyerror(const char *s, ...)
{
    va_list ap;
    va_start(ap, s);	
    fprintf(stderr, "line %d: error near '%s': ", yylineno, yytext);
    
    vfprintf(stderr, s, ap);
    fprintf(stderr, "\n");
    va_end(ap);
}


