%{
#include <vslc.h>
#include <stdio.h>

// macros
#define MALLOC malloc(sizeof(node_t))

/* State variables from the flex generated scanner */
extern int yylineno; // The line currently being read
extern char yytext[]; // The text of the last consumed lexeme
/* The main flex driver function used by the parser */
int yylex ( void );
/* The function called by the parser when errors occur */
int yyerror ( const char *error )
{
    fprintf ( stderr, "%s on line %d\n", error, yylineno );
    exit ( EXIT_FAILURE );
}

%}

%left '+' '-'
%left '*' '/'
%right UMINUS

%nonassoc IF THEN
%nonassoc ELSE

%token FUNC PRINT RETURN BREAK IF THEN ELSE WHILE FOR IN DO OPENBLOCK CLOSEBLOCK
%token VAR NUMBER IDENTIFIER STRING

%%
program :
      global_list {                                     // program -> global_list
        root = malloc (sizeof(node_t));                 // root = PROGRAM
        node_init (root, PROGRAM, NULL, 1, $1);         // $1 = GLOBAL_LIST
      }
    ;

global_list :
      global {                                          // global_list -> global
        $$ = malloc (sizeof(node_t));                   // $$ = GLOBAL_LIST
        node_init ($$, GLOBAL_LIST, NULL, 1, $1);       // $1 = GLOBAL
      }
    | global_list global {                              // global_list -> global_list global
        $$ = malloc (sizeof(node_t));                   // $$ = GLOBAL_LIST
        node_init ($$, GLOBAL_LIST, NULL, 2, $1, $2);   // $1 = GLOBAL_LIST, $2 = GLOBAL
      }
    ;

for_statement :
      FOR identifier IN expression '.' '.' expression DO statement {
          $$ = malloc (sizeof(node_t));                 // $$ = FOR_STATEMENT
          node_init ($$, FOR_STATEMENT, NULL, 4, $2, $4, $7, $9); // $2 = IDENTIFIER_DATA, etc.
      }
    ;

global :
      function {
        $$ = MALLOC;
        node_init($$, GLOBAL, NULL, 1, $1);    // $1 = function
      }
    | declaration {
        $$ = MALLOC;
        node_init($$, GLOBAL, NULL, 1, $1); // $1 = declaration
      }
    | array_declaration {
        $$ = MALLOC;
        node_init($$, GLOBAL, NULL, 1, $1); // $1 = array_declaration
      }
    ;

declaration :
      VAR variable_list {
        $$ = MALLOC;
        node_init($$, DECLARATION, NULL, 1, $2);   // $2 = variable_list
      }
    ;

variable_list :
      identifier {
        $$ = MALLOC;
        node_init($$, VARIABLE_LIST, NULL, 1, $1);      // $1 = identifier
      }
    | variable_list ',' identifier {
        $$ = MALLOC;
        node_init($$, VARIABLE_LIST, NULL, 2, $1, $3);   // $1 = variable_list, $3 = identifier
      }
    ;

array_declaration :
      VAR array_indexing {
        $$ = MALLOC;
        node_init($$, ARRAY_DECLARATION, NULL, 1, $2);   // $2 = array_indexing
      }
    ;

array_indexing :
      identifier '[' expression ']' {
        $$ = MALLOC;
        node_init($$, ARRAY_INDEXING, NULL, 2, $1, $3);      // $1 = identifier, $3 = expression
      }
    ;

function :
      FUNC identifier '(' parameter_list ')' statement {
        $$ = MALLOC;
        node_init($$, FUNCTION, NULL, 3, $2, $4, $6);    // $2 = identifier, $4 = parameter_list, $6 = statement
      }
    ;
  
parameter_list :
      variable_list {
        $$ = MALLOC;
        node_init($$, PARAMETER_LIST, NULL, 1, $1);    // $1 = variable_list
      }
    | /* empty */ {
        $$ = MALLOC;
        node_init($$, PARAMETER_LIST, NULL, 0);
      }
    ;

statement :
      assignment_statement {
        $$ = MALLOC;
        node_init($$, STATEMENT, NULL, 1, $1);       // $1 = assignment_statement
      }
    | return_statement {
        $$ = MALLOC;
        node_init($$, STATEMENT, NULL, 1, $1);       // $1 = return_statement
      }
    | print_statement {
        $$ = MALLOC;
        node_init($$, STATEMENT, NULL, 1, $1);       // $1 = print_statement
      }
    | if_statement {
        $$ = MALLOC;
        node_init($$, STATEMENT, NULL, 1, $1);       // $1 = if_statement
      }
    | while_statement {
        $$ = MALLOC;
        node_init($$, STATEMENT, NULL, 1, $1);       // $1 = while_statement
      }
    | for_statement {
        $$ = MALLOC;
        node_init($$, STATEMENT, NULL, 1, $1);       // $1 = for_statement
      }
    | break_statement {
        $$ = MALLOC;
        node_init($$, STATEMENT, NULL, 1, $1);       // $1 = break_statement
      }
    | block {
        $$ = MALLOC;
        node_init($$, STATEMENT, NULL, 1, $1);       // $1 = block
      }
    ;

block :
      OPENBLOCK declaration_list statement_list CLOSEBLOCK {
        $$ = MALLOC;
        node_init($$, BLOCK, NULL, 2, $2, $3);       // $2 = declaration_list, $3 = statement_list
      }
    | OPENBLOCK statement_list CLOSEBLOCK {
        $$ = MALLOC;
        node_init($$, BLOCK, NULL, 1, $2);       // $2 = statement_list
      }
    ;

declaration_list :
      declaration {
        $$ = MALLOC;
        node_init($$, DECLARATION_LIST, NULL, 1, $1);    // $1 = declaration_list
      }
    | declaration_list declaration {
        $$ = MALLOC;
        node_init($$, DECLARATION_LIST, NULL, 2, $1, $2);    // $1 = declaration_list, $2 = declaration
      }
    ;

statement_list :
      statement {
        $$ = MALLOC;
        node_init($$, STATEMENT_LIST, NULL, 1, $1);      // $1 = statement
      }
    | statement_list statement {
        $$ = MALLOC;
        node_init($$, STATEMENT_LIST, NULL, 2, $1, $2);      // $1 = statement
      }
    ;

assignment_statement :
      identifier ':' '=' expression {
        $$ = MALLOC;
        node_init($$, ASSIGNMENT_STATEMENT, NULL, 2, $1, $4);    // $1 = identifier, $4 = expression
      }
    | array_indexing ':' '=' expression {
        $$ = MALLOC;
        node_init($$, ASSIGNMENT_STATEMENT, NULL, 2, $1, $4);    // $1 = array_indexing, $4 = expression
    }
  ;

return_statement :
      RETURN expression {
        $$ = MALLOC;
        node_init($$, RETURN_STATEMENT, NULL, 1, $2);    // $2 = expression
      }
    ;

print_statement :
      PRINT print_list {
        $$ = MALLOC;
        node_init($$, PRINT_STATEMENT, NULL, 1, $2);    // $2 = print_list
      }
    ;

print_list :
      print_item {
        $$ = MALLOC;
        node_init($$, PRINT_LIST, NULL, 1, $1);    // $1 = print_item 
      }
    | print_list ',' print_item {
        $$ = MALLOC;
        node_init($$, PRINT_LIST, NULL, 2, $1, $3);    // $1 = print_list, $3 = print_item
      }
    ;

print_item :
      expression {
        $$ = MALLOC;
        node_init($$, PRINT_ITEM, NULL, 1, $1);      // $1 = expression
      }
    | string {
        $$ = MALLOC;
        node_init($$, PRINT_ITEM, NULL, 1, $1);      // $1 = string
      }
    ;

break_statement :
      BREAK {
        $$ = MALLOC;
        node_init($$, BREAK_STATEMENT, NULL, 0);
      }
    ;
  
if_statement :
      IF relation THEN statement {
        $$ = MALLOC;
        node_init($$, IF_STATEMENT, NULL, 2, $2, $4);    // $2 = relation, $4 = statement
      }
    | IF relation THEN statement ELSE statement {
        $$ = MALLOC;
        node_init($$, IF_STATEMENT, NULL, 3, $2, $4, $6);    // $2 = relation, $4 = statement, $6 = statement
      }
    ;

while_statement :
      WHILE relation DO statement {
        $$ = MALLOC;
        node_init($$, WHILE_STATEMENT, NULL, 2, $2, $4);   // $2 = relation, $4 = statement
      }
    ;

relation :
      expression '=' expression {
        $$ = MALLOC;
        node_init($$, RELATION, strdup("="), 2, $1, $3);   // $1 = $3 = expression
      }
    | expression '!' '=' expression {
        $$ = MALLOC;
        node_init($$, RELATION, strdup("!="), 2, $1, $4);   // $1 = $3 = expression
      }
    | expression '<' expression {
        $$ = MALLOC;
        node_init($$, RELATION, strdup("<"), 2, $1, $3);   // $1 = $3 = expression
      }
    | expression '>' expression {
        $$ = MALLOC;
        node_init($$, RELATION, strdup(">"), 2, $1, $3);   // $1 = $3 = expression
      }
    ;

expression :
      expression '+' expression {
        $$ = MALLOC;
        node_init($$, EXPRESSION, strdup("+"), 2, $1, $3);   // $1 = $3 = expression
      }
    | expression '-' expression {
        $$ = MALLOC;
        node_init($$, EXPRESSION, strdup("-"), 2, $1, $3);   // $1 = $3 = expression
      }
    | expression '*' expression {
        $$ = MALLOC;
        node_init($$, EXPRESSION, strdup("*"), 2, $1, $3);   // $1 = $3 = expression
      }
    | expression '/' expression {
        $$ = MALLOC;
        node_init($$, EXPRESSION, strdup("/"), 2, $1, $3);   // $1 = $3 = expression
      }
    | '-' expression {
        $$ = MALLOC;
        node_init($$, EXPRESSION, strdup("-"), 1, $2);      // $2 = expression
      }
    | '(' expression ')' {
        $$ = MALLOC;
        node_init($$, EXPRESSION, NULL, 1, $2);      // $2 = expression
      }
    | number {
        $$ = MALLOC;
        node_init($$, EXPRESSION, NULL, 1, $1);    // $1 = number
      }
    | identifier {
        $$ = MALLOC;
        node_init($$, EXPRESSION, NULL, 1, $1);    // $1 = identifier
      }
    | array_indexing {
        $$ = MALLOC;
        node_init($$, EXPRESSION, NULL, 1, $1);    // $1 = array_indexing
      }
    | identifier '(' argument_list ')' {
        $$ = MALLOC;
        node_init($$, EXPRESSION, NULL, 2, $1, $3);    // $1 = array_indexing, $3 = argument_list
      }
    ;

expression_list :
      expression {
        $$ = MALLOC;
        node_init($$, EXPRESSION_LIST, NULL, 1, $1);        // $1 = expression
      }
    | expression_list ',' expression {
        $$ = MALLOC;
        node_init($$, EXPRESSION_LIST, NULL, 2, $1, $3);   // $1 = expression_list, $3 = expression
      }
    ;

argument_list :
      expression_list {
        $$ = MALLOC;
        node_init($$, ARGUMENT_LIST, NULL, 1, $1);       // $1 = expression_list
      }
    | /* empty */ {
        $$ == MALLOC;
        node_init($$, ARGUMENT_LIST, NULL, 0);
      }
    ;

number :
      NUMBER {
        $$ = MALLOC;
        int64_t *num = malloc(sizeof(int64_t));
        *num = strtol(yytext, NULL, 10);
        node_init($$, NUMBER_DATA, num, 0);
      }
    ;

identifier :
      IDENTIFIER {
        $$ = MALLOC;
        node_init($$, IDENTIFIER_DATA, strdup(yytext), 0);
      }
    ;

string :
      STRING {
        $$ = MALLOC;
        node_init($$, STRING_DATA, strdup(yytext), 0);
      }
    ;
/*
    TODO:
    Include the remaining modified VSL grammar as specified in PS2 - starting with `global`

    HINT:
    Recall that 'node_init' takes any number of arguments where:
    1. Node to initialize.
    2. Node type (see "include/nodetypes.h").
    3. Data (so far, no nodes need any data, but consider e.g. the NUMBER_DATA or IDENTIFIER_DATA node types - what data should these contain?).
    4. Number of node children.
    5->. Children - Note that these are constructed by other semantic actions (e.g. `program` has one child that is constructed by the `global_list semantic action).
    This should be a pretty large file when you are done.

    HINT (OPTIONAL):
    Note that mallocing and initializing of nodes happens a lot.
    You may want to create C macros to reduce redundancy.
*/

%%
