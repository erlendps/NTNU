%{
#include <vslc.h>
// The tokens defined in parser.y
#include "y.tab.h"
#include <string.h>


int get_token(char *str) {
  if (!strcmp(str, "func")) {
    return FUNC;
  } else if (!strcmp(str, "print")) {
    return PRINT;
  } else if (!strcmp(str, "return")) {
    return RETURN;
  } else if (!strcmp(str, "break")) {
    return BREAK;
  } else if (!strcmp(str, "if")) {
    return IF;
  } else if (!strcmp(str, "then")) {
    return THEN;
  } else if (!strcmp(str, "else")) {
    return ELSE;
  } else if (!strcmp(str, "while")) {
    return WHILE;
  } else if (!strcmp(str, "for")) {
    return FOR;
  } else if (!strcmp(str, "in")) {
    return IN;
  } else if (!strcmp(str, "do")) {
    return DO;
  } else if (!strcmp(str, "begin")) {
    return OPENBLOCK;
  } else if (!strcmp(str, "end")) {
    return CLOSEBLOCK;
  } else if (!strcmp(str, "var")) {
    return VAR;
  } else {
    return IDENTIFIER;
  }
}
%}
%option noyywrap
%option array
%option yylineno

WHITESPACE [\ \t\v\r\n]
COMMENT \/\/[^\n]+
QUOTED \"([^\"\n]|\\\")*\"
DIGIT [0-9]
LETTER [a-zA-Z]

%%
{WHITESPACE}+                   { /* Eliminate whitespace */ }
{COMMENT}                       { /* Eliminate comments */ }
{QUOTED}                        { return STRING; }
{DIGIT}+                        { return NUMBER; }
{LETTER}({DIGIT}|{LETTER}|_)*   { return get_token(yytext); } 

  /* 
   TODO:

   Add the rest of the translation rules here.
   See the lexical structure definition of the modified VSL in PS2.
   Also see the `%token` directives in parser.y for all symbolic names that can be returned - e.g. STRING, NUMBER, IDENTIFIER.

   Hint to get you started:
   The scanner returns STRING when matching the QUOTED regex above.
   When should the scanner return a NUMBER, IDENTIFIER, etc?
   In which specific scenarios should the scanner return a FUNC or PRINT?
  */

  /* Unknown chars get returned as single char tokens */
.                       { return yytext[0]; }
%%
