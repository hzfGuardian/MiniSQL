/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_ANALYSIS_HPP_INCLUDED
# define YY_YY_ANALYSIS_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    AND = 258,
    ON = 259,
    SELECT = 260,
    FROM = 261,
    WHERE = 262,
    DROP = 263,
    TABLE = 264,
    CREATE = 265,
    INDEX = 266,
    PRIMARY = 267,
    KEY = 268,
    VALUES = 269,
    UNIQUE = 270,
    INSERT = 271,
    INTO = 272,
    DELETE = 273,
    QUIT = 274,
    EXECFILE = 275,
    INT = 276,
    FLOAT = 277,
    CHAR = 278,
    NAME = 279,
    STRING = 280,
    INTNUM = 281,
    FLOATNUM = 282,
    EMPTY = 283,
    COMPARISON = 284,
    STMT = 285,
    ATTR_INFO = 286,
    ATTR_LIST = 287,
    ATTR = 288,
    DATA_TYPE = 289,
    ATTR_VALUE_LIST = 290,
    WH_LIST = 291,
    WH_NAME = 292
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 23 "Analysis.y" /* yacc.c:1909  */

	struct SQLGrammarTree *pNode;

#line 96 "Analysis.hpp" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_ANALYSIS_HPP_INCLUDED  */
