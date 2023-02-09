%{
#include <vslc.h>
// The tokens defined in parser.y
#include "y.tab.h"
%}
%option noyywrap
%option array
%option yylineno

WHITESPACE [\ \t\v\r\n]
COMMENT \/\/[^\n]+
QUOTED \"([^\"\n]|\\\")*\"
DIGIT [0-9]
LETTER [a-zA-Z]
FUNCTION_DEC func
PRINT print
RETURN return
BREAK break
IF if
THEN then
ELSE else
WHILE while
FOR for
IN in
DO do
OPENBLOCK begin
ENDBLOCK end
VAR var

%%
{WHITESPACE}+                   { /* Eliminate whitespace */ }
{COMMENT}                       { /* Eliminate comments */ }
{QUOTED}                        { return STRING; }
{DIGIT}+                        { return NUMBER; }
{LETTER}({DIGIT}|{LETTER}|_)*   { return IDENTIFIER; } 
{FUNCTION_DEC}                  { return FUNC; }
{PRINT}                         { return PRINT; }
{RETURN}                        { return RETURN; }
{BREAK}                         { return BREAK; }
{IF}                            { return IF; }
{THEN}                          { return THEN; }
{ELSE}                          { return ELSE; }
{WHILE}                         { return WHILE; }
{FOR}                           { return FOR; }
{IN}                            { return IN; }
{DO}                            { return DO; }
{OPENBLOCK}                     { return OPENBLOCK; }
{ENDBLOCK}                      { return ENDBLOCK; }
{VAR}                           { return VAR; }

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
