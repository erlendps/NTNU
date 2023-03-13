#include <assert.h>
#include <vslc.h>

/* Global symbol table and string list */
symbol_table_t *global_symbols;
char **string_list;
size_t string_list_len;
size_t string_list_capacity;

static void find_globals(void);
static void bind_names(symbol_table_t *local_symbols, node_t *root);
static void print_symbol_table(symbol_table_t *table, int nesting);
static void destroy_symbol_tables(void);

static size_t add_string(char *string);
static void print_string_list(void);
static void destroy_string_list(void);

/* External interface */

/* Creates a global symbol table, and local symbol tables for each function.
 * While building the symbol tables:
 *  - All usages of symbols are bound to their symbol table entries.
 *  - All strings are entered into the string_list
 */
void create_tables(void) {
  // set start capacity and entries for global string list
  string_list_len = 0;
  string_list_capacity = 0;
  // find the global symbols and place them in the global symbol table
  find_globals();
  // iterate over each symbol. If the symbol is a function, we bind the names.
  for (size_t i = 0; i < global_symbols->n_symbols; i++) {
    if (global_symbols->symbols[i]->type == SYMBOL_FUNCTION) {
      symbol_t *function = global_symbols->symbols[i];
      bind_names(function->function_symtable, function->node->children[2]);
    }
  }
}

/* Prints the global symbol table, and the local symbol tables for each function.
 * Also prints the global string list.
 * Finally prints out the AST again, with bound symbols.
 */
void print_tables(void) {
  print_symbol_table(global_symbols, 0);
  printf("\n == STRING LIST == \n");
  print_string_list();
  printf("\n == BOUND SYNTAX TREE == \n");
  print_syntax_tree();
}

/* Destroys all symbol tables and the global string list */
void destroy_tables(void) {
  destroy_symbol_tables();
  destroy_string_list();
}

/* Recursive function for fetching the name of a node, i.e the identifier.
 * Note that it only searches the leftmost child, so the method does not work
 * in the general case. It works for functions and declarations.
 */
char *get_name(node_t *node) {
  // base case
  if (node->type == IDENTIFIER_DATA) {
    return (char *)(node->data);
  } else {
    // make sure we don't segfault
    assert(node->n_children > 0);
    return get_name(node->children[0]);
  }
}

/* Helper function for inserting global symbols */
void insert_global_symbol(symbol_t *symbol) {
  // check if null
  if (symbol != NULL) {
    // try to insert, if it fails, do cleanup
    if (symbol_table_insert(global_symbols, symbol) == INSERT_COLLISION) {
      destroy_symbol_tables();
      destroy_syntax_tree();
      exit(1);
    }
  }
}

/* Helper function for creating a new symbol. Takes in a pointer to a symbol which it
 * should initiate. Node is the node that creates the symbol.
 */
void create_symbol(symbol_t *symbol, node_t *node, symtype_t type) {
  // get the name of the symbol
  char *name = strdup(get_name(node));
  // set the fields
  symbol->name = name;
  symbol->type = type;
  symbol->node = node;
  symbol->function_symtable = NULL;
}
/* Internal matters */

/* Goes through all global declarations in the syntax tree, adding them to the global symbol table.
 * When adding functions, local symbol tables are created, and symbols for the functions parameters are added.
 */
static void find_globals(void) {
  // init the global symbol table
  global_symbols = symbol_table_init();
  // root is the global_list node, we just need to iterate over roots children
  for (uint64_t i = 0; i < root->n_children; i++) {
    node_t *global = root->children[i];
    // switch on the node type
    switch (global->type) {
      case ARRAY_DECLARATION:
        // array_declaration, just create the symbol and insert it into the global symbols
        symbol_t *arr_symbol = malloc(sizeof(symbol_t));
        create_symbol(arr_symbol, global, SYMBOL_GLOBAL_ARRAY);
        insert_global_symbol(arr_symbol);
        break;

      case DECLARATION:
        // declaratino case, mostly the same as array_declaration, but we have to iterate over
        // the children (since they are the nodes containing the symbol)
        for (uint64_t j = 0; j < global->n_children; j++) {
          symbol_t *symbol = malloc(sizeof(symbol_t));
          create_symbol(symbol, global->children[j], SYMBOL_GLOBAL_VAR);
          insert_global_symbol(symbol);
        }
        break;

      case FUNCTION:
        // the function case requires more work
        // first we create the function symbol
        symbol_t *symbol = malloc(sizeof(symbol_t));
        create_symbol(symbol, global, SYMBOL_FUNCTION);
        // give the function symbol it's own symbol table
        symbol->function_symtable = symbol_table_init();
        // set backup for hashtable so that we can reach the global scope
        symbol->function_symtable->hashmap->backup = global_symbols->hashmap;
        // insert into global table
        insert_global_symbol(symbol);
        // insert the parameters in the function's table
        node_t *param_list = global->children[1];
        // the rest is the same as for the declaration case
        for (uint64_t j = 0; j < param_list->n_children; j++) {
          symbol_t *param_symbol = malloc(sizeof(symbol_t));
          create_symbol(param_symbol, param_list->children[j], SYMBOL_PARAMETER);
          if (symbol_table_insert(symbol->function_symtable, param_symbol) == INSERT_COLLISION) {
            destroy_symbol_tables();
            destroy_syntax_tree;
            exit(1);
          }
        }
        break;

      default:
        break;
    }
  }
}

/* A recursive function that traverses the body of a function, and:
 *  - Adds variable declarations to the function's local symbol table.
 *  - Pushes and pops local variable scopes when entering blocks.
 *  - Binds identifiers to the symbol it references.
 *  - Inserts STRING_DATA nodes' data into the global string list, and replaces it with its list position.
 */
static void bind_names(symbol_table_t *local_symbols, node_t *node) {
  switch (node->type) {
    case BLOCK:
      // need a new scope, i.e hashmap
      symbol_hashmap_t *hashmap = symbol_hashmap_init();
      hashmap->backup = local_symbols->hashmap;
      local_symbols->hashmap = hashmap;
      // recursive call with new scope
      for (uint64_t i = 0; i < node->n_children; i++) {
        bind_names(local_symbols, node->children[i]);
      }
      // when it has returned, we can destroy the scope we just created
      local_symbols->hashmap = hashmap->backup;
      symbol_hashmap_destroy(hashmap);
      break;

    case DECLARATION:
      // the children here should be new variables in the current scope
      for (uint64_t i = 0; i < node->n_children; i++) {
        symbol_t *symbol = malloc(sizeof(symbol_t));
        create_symbol(symbol, node->children[i], SYMBOL_LOCAL_VAR);
        if (symbol_table_insert(local_symbols, symbol) == INSERT_COLLISION) {
          destroy_string_list();
          destroy_symbol_tables();
          destroy_syntax_tree();
          exit(1);
        };
      }
      break;

    case IDENTIFIER_DATA:
      // the only time we reach this, IDENTIFIER_DATA is a node that is referencing
      // a declaration, and needs to be bound
      symbol_t *symbol;
      // error handling
      if ((symbol = symbol_hashmap_lookup(local_symbols->hashmap, (char *)(node->data))) == NULL) {
        printf("ERROR, variable is not defined.\n");
        destroy_string_list();
        destroy_symbol_tables();
        destroy_syntax_tree();
        exit(1);
      }
      // bind the node to the symbol it references
      node->symbol = symbol;
      break;

    case STRING_DATA:
      // place string in string_list
      // overwrite data with the index
      int64_t *index = malloc(sizeof(int64_t));
      *index = add_string((char *)(node->data));
      node->data = index;
      break;

    // default case, make recursive calls with the children
    default:
      for (uint64_t i = 0; i < node->n_children; i++) {
        bind_names(local_symbols, node->children[i]);
      }
      break;
  }
}

/* Prints the given symbol table, with sequence number, symbol names and types.
 * When printing function symbols, its local symbol table is recursively printed, with indentation.
 */
static void print_symbol_table(symbol_table_t *table, int nesting) {
  // check if null
  if (table != NULL) {
    // iterate over each symbol entry. If we encounter a function
    // we make a recursive call to print that function's symbol table
    for (size_t i = 0; i < table->n_symbols; i++) {
      symbol_t *symbol = table->symbols[i];
      printf("%*s%lu: ", nesting, "", i);
      printf("%s", SYMBOL_TYPE_NAMES[symbol->type]);
      printf("(%s)\n", symbol->name);
      if (symbol->type == SYMBOL_FUNCTION) {
        print_symbol_table(symbol->function_symtable, nesting + 4);
      }
    }
  }
}

/* Helper method to destroy a symbol */
void destroy_symbol(symbol_t *symbol) {
  // first destroy the name
  free(symbol->name);
  // then the rest of the symbol
  free(symbol);
}

/* Frees up the memory used by the global symbol table, all local symbol tables, and their symbols */
static void destroy_symbol_tables(void) {
  // first iterate over each entry in the global symbol table
  // if we encounter a function, we have to destroy its symbol-table first
  for (size_t i = 0; i < global_symbols->n_symbols; i++) {
    symbol_t *symbol = global_symbols->symbols[i];
    if (symbol->type == SYMBOL_FUNCTION) {
      // free hashmap for function, this should be the only hashtable left (apart from the global)
      symbol_table_t *func_table = symbol->function_symtable;
      symbol_hashmap_destroy(func_table->hashmap);
      // free each symbol in the functions symbol-table
      for (size_t j = 0; j < func_table->n_symbols; j++) {
        destroy_symbol(func_table->symbols[j]);
      }
      // free the symbol array and finally the table
      free(func_table->symbols);
      free(func_table);
    }
    // finally destroy the symbol
    destroy_symbol(symbol);
  }
  // destroy the global hashmap, the symbols array and the global symbol table
  symbol_hashmap_destroy(global_symbols->hashmap);
  free(global_symbols->symbols);
  free(global_symbols);
}

/* Adds the given string to the global string list, resizing if needed.
 * Takes ownership of the string, and returns its position in the string list.
 */
static size_t add_string(char *string) {
  // edge case, if len == capacity, expand list by 10 entries
  if (string_list_len == string_list_capacity) {
    string_list_capacity += 10;
    string_list = realloc(string_list, string_list_capacity * sizeof(char *));
  }
  // instead of using strdup, just place the pointer in the array
  // results in fewer memory mallocings and freeings
  string_list[string_list_len] = string;
  return string_list_len++;
}

/* Prints all strings added to the global string list */
static void print_string_list(void) {
  // iterate over the list and print
  for (size_t i = 0; i < string_list_len; i++) {
    printf("%lu: %s\n", i, string_list[i]);
  }
}

/* Frees all strings in the global string list, and the string list itself */
static void destroy_string_list(void) {
  // first free all the string in the list
  for (size_t i = 0; i < string_list_len; i++) {
    free(string_list[i]);
  }
  // free string_list
  free(string_list);
}
