#include <vslc.h>

// This header defines a bunch of macros we can use to emit assembly to stdout
#include "assert.h"
#include "emit.h"

// In the System V calling convention, the first 6 integer parameters are passed in registers
#define NUM_REGISTER_PARAMS 6
static const char *REGISTER_PARAMS[6] = {RDI, RSI, RDX, RCX, R8, R9};

// Takes in a symbol of type SYMBOL_FUNCTION, and returns how many parameters the function takes
#define FUNC_PARAM_COUNT(func) ((func)->node->children[1]->n_children)
#define GLOBAL_VAR_PREFIX ".gv"
#define FUNCTION_PREFIX ".def"

static void generate_stringtable(void);
static void generate_global_variables(void);
static void generate_function(symbol_t *function);
static void generate_statement(node_t *node, char *func_name);
static void generate_main(symbol_t *first);
static void generate_unary_expression(node_t *expression);
static void generate_binary_expression(node_t *expression);
static void generate_expression(node_t *expression);

/* Entry point for code generation */
void generate_program(void) {
  generate_stringtable();
  // putchar used for spacing
  putchar('\n');
  generate_global_variables();
  putchar('\n');
  // set up source code section
  DIRECTIVE(".section .text");

  // temporary variable to hold the entrypoint so that we dont iterate over
  // the loop twice.
  symbol_t *entrypoint_symbol = NULL;
  // iterate over global symbols
  for (size_t i = 0; i < global_symbols->n_symbols; i++) {
    symbol_t *current = global_symbols->symbols[i];
    // check if symbol is a function
    if (current->type == SYMBOL_FUNCTION) {
      // if the entrypoint is already defined, just generate the function,
      // otherwise, generate the function and set this symbols as the entrypoint
      if (entrypoint_symbol == NULL) {
        entrypoint_symbol = current;
      }
      generate_function(current);
      putchar('\n');
    }
  }
  // generate wrapper
  generate_main(entrypoint_symbol);
}

/* Prints one .asciz entry for each string in the global string_list */
static void generate_stringtable(void) {
  DIRECTIVE(".section .rodata");
  // These strings are used by printf
  DIRECTIVE("intout: .asciz \"%s\"", "%ld ");
  DIRECTIVE("strout: .asciz \"%s\"", "%s ");
  // This string is used by the entry point-wrapper
  DIRECTIVE("errout: .asciz \"%s\"", "Wrong number of arguments");

  // iterate over every entry in string list and use DIRECTIVE macro to print
  for (size_t i = 0; i < string_list_len; i++) {
    DIRECTIVE("string%lu: .asciz %s", i, string_list[i]);
  }
}

/* Prints .zero entries in the .bss section to allocate room for global variables and arrays */
static void generate_global_variables(void) {
  // set up the section
  DIRECTIVE(".section .bss");
  DIRECTIVE(".align 8");
  // we have to iterate over global symbols. We should skip functions and only care about
  // declarations or array_declarations. A global symbol is one of these three.
  // prefix is defined above
  for (size_t i = 0; i < global_symbols->n_symbols; i++) {
    symbol_t *current = global_symbols->symbols[i];
    // we only really care about GLOBAL_VAR or GLOBAL_ARRAY
    switch (current->type) {
      case SYMBOL_GLOBAL_VAR:
        DIRECTIVE("%s%s:\t.zero 8", GLOBAL_VAR_PREFIX, current->name);
        break;

      case SYMBOL_GLOBAL_ARRAY:
        node_t *node = current->node;
        // get the size of the array by looking at the node's second child
        // which should be a NUMBER_DATA node.
        int64_t arr_size = *((int64_t *)node->children[1]->data) * 8;
        DIRECTIVE("%s%s:\t.zero %li", GLOBAL_VAR_PREFIX, current->name, arr_size);
        break;

      case SYMBOL_FUNCTION:
      default:
        break;
    }
  }
}

/* Function for fetching number of params the function has based on it symbol table */
static int get_num_parameters(symbol_table_t *fun_sym_table) {
  int count = 0;
  for (size_t i = 0; fun_sym_table->n_symbols; i++) {
    if (fun_sym_table->symbols[i]->type == SYMBOL_LOCAL_VAR) {
      break;
    }
    count++;
  }
  return count;
}

/* Prints the entry point. preable, statements and epilouge of the given function */
static void generate_function(symbol_t *function) {
  symbol_table_t *local_symtable = function->function_symtable;

  // function label
  LABEL("%s%s", FUNCTION_PREFIX, function->name);
  // label for readability in assembly
  LABEL(".%s_prologue", function->name);
  // create call frame
  PUSHQ(RBP);
  MOVQ(RSP, RBP);
  // push all parameters to the stack
  for (int i = 0; i < NUM_REGISTER_PARAMS; i++) {
    PUSHQ(REGISTER_PARAMS[i]);
  }
  // next we push all the local variables
  // we have to iterate over the functions local symbol table to do this
  for (size_t i = 0; i < local_symtable->n_symbols; i++) {
    if (local_symtable->symbols[i]->type == SYMBOL_LOCAL_VAR) {
      PUSHQ("$0");
    }
  }
  putchar('\n');
  LABEL(".%s_body", function->name);

  // now we can handle the function body
  generate_statement(function->node->children[2], function->name);

  // clean up
  // reset stack pointer and pop rbp
  putchar('\n');
  // safeguard of functions not returning a value
  MOVQ("$0", RAX);
  LABEL(".%s_epilogue", function->name);
  MOVQ(RBP, RSP);
  POPQ(RBP);
  RET;
}

/* Wrapper function for strcmp so it makes sense */
int str_equals(char *str, char *target) {
  return !abs(strcmp(str, target));
}

static void generate_function_call(node_t *call) {
  // before calling, we have to prepare the parameters
  symbol_t *symbol = call->children[0]->symbol;
  assert(symbol->type == SYMBOL_FUNCTION);
  // get num of parameters
  int64_t n_parameters = FUNC_PARAM_COUNT(symbol);
  for (int64_t i = n_parameters - 1; i >= 0; i--) {
    generate_expression(call->children[1]->children[i]);
    // just push everything to the stack, so we dont by accident
    // rewrite a parameter register
    PUSHQ(RAX);
  }
  uint64_t num_register_params;
  if (NUM_REGISTER_PARAMS > n_parameters) {
    num_register_params = n_parameters;
  } else {
    num_register_params = NUM_REGISTER_PARAMS;
  }
  for (uint64_t i = 0; i < num_register_params; i++) {
    POPQ(REGISTER_PARAMS[i]);
  }
  // every parameter is now in either the stack or in it's register

  EMIT("call %s%s", FUNCTION_PREFIX, symbol->name);
}

/* Function for generating an expression. The function is recursive
   and should store the result at each level in %rax.
 */
static void generate_expression(node_t *expression) {
  // and switch on type
  switch (expression->type) {
    case EXPRESSION:
      // experssion case: an expression node has either 1 or 2 children. Delegate
      // the generation to other functions.
      if (expression->n_children == 1) {
        generate_unary_expression(expression);
      } else {
        generate_binary_expression(expression);
      }
      break;

    case NUMBER_DATA:
      // number case: just place the number in rax
      EMIT("movq $%ld, %s", *((int64_t *)expression->data), RAX);
      break;

    case IDENTIFIER_DATA:
      // identifier case: we first check if its a global or local variable
      // (that's why we fetch the symbol). If it's global we just reference it,
      // however if it's local, we have to reference it based on rbp. Since the
      // first 6 entries in the stack are parameters, we offset by 7 + the symbol
      // sequence number.
      // fetch the symbol
      symbol_t *symbol_id = expression->symbol;
      if (symbol_id->type == SYMBOL_GLOBAL_VAR) {
        EMIT("movq %s%s, %s", GLOBAL_VAR_PREFIX, symbol_id->name, RAX);
      } else if (symbol_id->type == SYMBOL_LOCAL_VAR) {
        EMIT("movq -%lu%s, %s", (7 + (symbol_id->sequence_number - get_num_parameters(symbol_id->function_symtable))) * 8, MEM(RBP), RAX);
      } else if (symbol_id->type == SYMBOL_PARAMETER) {
        size_t pos = symbol_id->sequence_number;
        bool downwards = true;
        if (pos > 5) {
          pos = pos - 5 + 1;
          // pos is orignally counting "down" the stack, but we change it so it now
          // counts up, meaning we also have to add 1 (since the return address is before rbp)
          downwards = false;
        }
        // now we can reference it
        if (downwards) {
          EMIT("movq -%lu%s, %s", (1 + pos) * 8, MEM(RBP), RAX);
        } else {
          EMIT("movq %lu%s, %s", pos * 8, MEM(RBP), RAX);
        }
      }
      break;

    case ARRAY_INDEXING:
      // array indexing case: first fetch the address of the array. Then get
      // the index, and finally do a array access.
      symbol_t *symbol_ai = expression->children[0]->symbol;
      if (symbol_ai->type == SYMBOL_GLOBAL_ARRAY) {
        // get the index of the array
        generate_expression(expression->children[1]);
        // the index of the array is now in rax, copy it to r11
        MOVQ(RAX, R11);

        // first move array into r10
        EMIT("movq $%s%s, %s", GLOBAL_VAR_PREFIX, symbol_ai->name, R10);
        // place the contents of the array in rax
        EMIT("movq (%s, %s, %d), %s", R10, R11, 8, RAX);
      }
      break;

    default:
      break;
  }
}

/* Function for generating a unary expression. The only unary expression
   that should be left is the negation (-).
 */
static void generate_unary_expression(node_t *expression) {
  char *data = (char *)expression->data;
  if (str_equals(data, "-")) {
    // generate the child, which is now stored in rax
    generate_expression(expression->children[0]);
    // then negate RAX
    NEGQ(RAX);
  }
}

/* Function for generating binary expression. This function handles all
   binary expression, so addition, subtraction and others. It also handles
   function calls, but it delegates this to generate_function_call.
 */
static void generate_binary_expression(node_t *expression) {
  // get operator
  char *data = (char *)expression->data;
  // special case for function call
  if (str_equals(data, "call")) {
    generate_function_call(expression);
    // return, since the rest of the function assumes that both children are
    // expressions.
    return;
  }

  // stack machine implementation
  // generate lhs and push to stack
  generate_expression(expression->children[0]);
  PUSHQ(RAX);
  // generate rhs
  generate_expression(expression->children[1]);
  // pop lhs to r10
  POPQ(R10);

  // the operation to perform between r10 and rax is defined by the operator (data)
  if (str_equals(data, "+")) {
    // add r10 to rax
    ADDQ(R10, RAX);
  } else if (str_equals(data, "-")) {
    // subtract rax from r10
    SUBQ(RAX, R10);
    // since result now is stored in r10, move this to rax
    MOVQ(R10, RAX);
  } else if (str_equals(data, "*")) {
    // sign extend rax to rdx:rax
    CQO;
    // multiply rdx:rax by r10
    IMULQ(R10, RAX);
  } else if (str_equals(data, "/")) {
    // here we have to swap r10 and rax, because its rdx:rax / r10
    PUSHQ(R10);
    MOVQ(RAX, R10);
    POPQ(RAX);
    // sign extend rax to rdx:rax
    CQO;
    // divide rdx:rax by r10 (which means now divide lhs by rhs)
    IDIVQ(R10);
  }
}

static void generate_assignment_statement(node_t *statement) {
  assert(statement->type == ASSIGNMENT_STATEMENT);
  // two cases: assignee is identifier_data or array_indexing
  node_t *identifier = statement->children[0];
  node_t *expression = statement->children[1];

  if (identifier->type == IDENTIFIER_DATA) {
    symbol_t *symbol = identifier->symbol;
    // generate expression, stores result in rax
    generate_expression(expression);
    if (symbol->type == SYMBOL_GLOBAL_VAR) {
      // symbol is global case
      EMIT("movq %s, %s%s", RAX, GLOBAL_VAR_PREFIX, symbol->name);
    } else if (symbol->type == SYMBOL_LOCAL_VAR) {
      // symbol is local case
      EMIT("movq %s, -%lu%s", RAX, (7 + (symbol->sequence_number - get_num_parameters(symbol->function_symtable))) * 8, MEM(RBP));
    } else if (symbol->type == SYMBOL_PARAMETER) {
      size_t pos = symbol->sequence_number;
      bool downwards = true;
      if (pos > 5) {
        pos = pos - 5 + 1;
        // pos is orignally counting "down" the stack, but we change it so it now
        // counts up, meaning we also have to add 1 (since the return address is before rbp)
        downwards = false;
      }
      // now we can reference it
      if (downwards) {
        EMIT("movq %s, -%lu%s", RAX, (1 + pos) * 8, MEM(RBP));
      } else {
        EMIT("movq %s, %lu%s", RAX, pos * 8, MEM(RBP));
      }
    }

  } else if (identifier->type == ARRAY_INDEXING) {
    assert(identifier->children[0]->symbol->type == SYMBOL_GLOBAL_ARRAY);
    // get the index (stored in rax)
    generate_expression(identifier->children[1]);
    // move to r11
    MOVQ(RAX, R11);
    // we have to push it to the stack, incase expression contains array indexing
    // (in which case it would overwrite r11 again)
    PUSHQ(R11);
    // generate expression, stores result in rax
    generate_expression(expression);
    // restore r11
    POPQ(R11);
    // get the array
    EMIT("movq $%s%s, %s", GLOBAL_VAR_PREFIX, identifier->children[0]->symbol->name, R10);
    // write to the array
    EMIT("movq %s, (%s, %s, %d)", RAX, R10, R11, 8);
  }
}

static void generate_print_statement(node_t *statement) {
  assert(statement->type == PRINT_STATEMENT);
  // iterate over each print item
  for (uint64_t i = 0; i < statement->n_children; i++) {
    node_t *item = statement->children[i];
    if (item->type == STRING_DATA) {
      // string, we need to put strout in rdi and find the position
      // of the string in string list
      int64_t pos = *((int64_t *)item->data);
      MOVQ("$strout", RDI);
      EMIT("movq $string%ld, %s", pos, RSI);
    } else {
      // expression, generate the expression, move intout to rdi
      // and place rax in rsi
      generate_expression(item);
      MOVQ("$intout", RDI);
      MOVQ(RAX, RSI);
    }
    // place 0 in rax to not crash printf
    MOVQ("$0", RAX);
    // call printf
    EMIT("call printf");
  }
  // every string is printed
  // place \n in rdi and call putchar
  MOVQ("$'\\n'", RDI);
  EMIT("call putchar");
}

static void generate_return_statement(node_t *statement, char *func_name) {
  assert(statement->type == RETURN_STATEMENT);
  generate_expression(statement->children[0]);
  EMIT("jmp .%s_epilogue", func_name);
}

/* Recursively generate the given statement node, and all sub-statements. */
static void generate_statement(node_t *node, char *func_name) {
  switch (node->type) {
    case BLOCK:
      // case if we have a block, generate the statement list instead, which should be
      // the last child.
      generate_statement(node->children[node->n_children - 1], func_name);
      break;

    case STATEMENT_LIST:
      // if we have a statement list we do recursive calls
      for (uint64_t i = 0; i < node->n_children; i++) {
        generate_statement(node->children[i], func_name);
      }
      break;

    // now base cases begin
    case ASSIGNMENT_STATEMENT:
      generate_assignment_statement(node);
      break;

    case RETURN_STATEMENT:
      generate_return_statement(node, func_name);
      break;

    case PRINT_STATEMENT:
      generate_print_statement(node);
      break;

    // TODO: for next assignment
    case IF_STATEMENT:
    case WHILE_STATEMENT:
    case BREAK_STATEMENT:
      break;
    default:
      break;
  }
}

// Generates a wrapper, to be able to use a vsl function as our entrypoint
static void generate_main(symbol_t *first) {
  // Make the globally available main function
  DIRECTIVE(".globl main");
  LABEL("main");

  // Save old base pointer, and set new base pointer
  PUSHQ(RBP);
  MOVQ(RSP, RBP);

  // Which registers argc and argv are passed in
  const char *argc = RDI;
  const char *argv = RSI;

  const size_t expected_args = FUNC_PARAM_COUNT(first);

  SUBQ("$1", argc);  // argc counts the name of the binary, so subtract that
  EMIT("cmpq $%ld, %s", expected_args, argc);
  JNE("ABORT");  // If the provdied number of arguments is not equal, go to the abort label

  if (expected_args == 0)
    goto skip_args;  // No need to parse argv

  // Now we emit a loop to parse all parameters, and push them to the stack,
  // in right-to-left order

  // First move the argv pointer to the vert rightmost parameter
  EMIT("addq $%ld, %s", expected_args * 8, argv);

  // We use rcx as a counter, starting at the number of arguments
  MOVQ(argc, RCX);
  LABEL("PARSE_ARGV");  // A loop to parse all parameters
  PUSHQ(argv);          // push registers to caller save them
  PUSHQ(RCX);

  // Now call strtol to parse the argument
  EMIT("movq (%s), %s", argv, RDI);  // 1st argument, the char *
  MOVQ("$0", RSI);                   // 2nd argument, a null pointer
  MOVQ("$10", RDX);                  // 3rd argument, we want base 10
  EMIT("call strtol");

  // Restore caller saved registers
  POPQ(RCX);
  POPQ(argv);
  PUSHQ(RAX);  // Store the parsed argument on the stack

  SUBQ("$8", argv);         // Point to the previous char*
  EMIT("loop PARSE_ARGV");  // Loop uses RCX as a counter automatically

  // Now, pop up to 6 arguments into registers instead of stack
  for (size_t i = 0; i < expected_args && i < NUM_REGISTER_PARAMS; i++)
    POPQ(REGISTER_PARAMS[i]);

skip_args:

  EMIT("call %s%s", FUNCTION_PREFIX, first->name);
  MOVQ(RAX, RDI);     // Move the return value of the function into RDI
  EMIT("call exit");  // Exit with the return value as exit code

  LABEL("ABORT");  // In case of incorrect number of arguments
  MOVQ("$errout", RDI);
  EMIT("call puts");  // print the errout string
  MOVQ("$1", RDI);
  EMIT("call exit");  // Exit with return code 1
}
