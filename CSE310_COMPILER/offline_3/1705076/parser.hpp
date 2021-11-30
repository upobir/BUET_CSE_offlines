/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_PARSER_HPP_INCLUDED
# define YY_YY_PARSER_HPP_INCLUDED
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
    ID = 258,
    INT = 259,
    FLOAT = 260,
    VOID = 261,
    FOR = 262,
    WHILE = 263,
    IF = 264,
    ELSE = 265,
    RETURN = 266,
    PRINTLN = 267,
    COMMA = 268,
    SEMICOLON = 269,
    LPAREN = 270,
    RPAREN = 271,
    LCURL = 272,
    RCURL = 273,
    LTHIRD = 274,
    RTHIRD = 275,
    CONST_INT = 276,
    CONST_FLOAT = 277,
    ASSIGNOP = 278,
    LOGICOP = 279,
    RELOP = 280,
    ADDOP = 281,
    MULOP = 282,
    NOT = 283,
    INCOP = 284,
    DECOP = 285,
    LOWER_THAN_ELSE = 286
  };
#endif
/* Tokens.  */
#define ID 258
#define INT 259
#define FLOAT 260
#define VOID 261
#define FOR 262
#define WHILE 263
#define IF 264
#define ELSE 265
#define RETURN 266
#define PRINTLN 267
#define COMMA 268
#define SEMICOLON 269
#define LPAREN 270
#define RPAREN 271
#define LCURL 272
#define RCURL 273
#define LTHIRD 274
#define RTHIRD 275
#define CONST_INT 276
#define CONST_FLOAT 277
#define ASSIGNOP 278
#define LOGICOP 279
#define RELOP 280
#define ADDOP 281
#define MULOP 282
#define NOT 283
#define INCOP 284
#define DECOP 285
#define LOWER_THAN_ELSE 286

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_HPP_INCLUDED  */
