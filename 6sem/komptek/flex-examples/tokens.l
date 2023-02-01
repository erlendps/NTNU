%{
  #include <stdio.h>
  enum { IF, THEN, ENDIF, INT, END };
%}
space [\n\r\t\v\ ]
%%
{space}+ {}
if      { return IF; }
then    { return THEN; }
endif   { return ENDIF; }
end     { return END; }
[0-9]+  { return INT; }
%%
int yywrap() { return 0; }
int main ()
{
    int token = 0;
    while ( token != END)
    {
        token = yylex();
        switch ( token )
        {
            case IF: printf ( "Found if\n" ); break;
            case THEN: printf ( "Found then\n" ); break;
            case ENDIF: printf ( "Found endif\n" ); break;
            case INT: printf ( "Found integer %s\n", yytext ); break;
            case END: printf ( "Hanging up... bye\n" ); break;
        }
    }
}
