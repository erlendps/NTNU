%{
  #include <stdio.h>
%}
%state STRING
space [\n\r\t\v\ ]
%%
<INITIAL>{space}*     {}
<INITIAL>if  { printf ( "Found 'if'\n" ); }
<INITIAL>end { printf ( "Found 'end'\n" ); return 0; }
<INITIAL>\"  { printf ( "Found string: " ); BEGIN(STRING); }
<STRING>\"   { printf ( "\n" ); BEGIN(INITIAL); }
<STRING>.    { printf ( "%c,", yytext[0] ); }
%%
int yywrap() { return 0; }
int main () { yylex(); }
