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
LETTER [a-zA-Z_]
FUNC func
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
STARTBLOCK begin
ENDBLOCK end
VAR var

%%
{FUNC}                          { return FUNC; }
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
{STARTBLOCK}                    { return OPENBLOCK; }
{ENDBLOCK}                      { return CLOSEBLOCK; }
{VAR}                           { return VAR; }
{WHITESPACE}+                   { /* Eliminate whitespace */ }
{COMMENT}                       { /* Eliminate comments */ }
{QUOTED}                        { return STRING; }
{DIGIT}+                        { return NUMBER; }
{LETTER}({DIGIT}|{LETTER})*     { return IDENTIFIER; } 
.                               { return yytext[0]; }
%%
