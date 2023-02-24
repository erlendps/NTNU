#define NODETYPES_IMPLEMENTATION
#include <assert.h>
#include <stdlib.h>
#include <vslc.h>

/* Enum for search status */
typedef enum SEARCH_STATUS {
  SEARCH_STATUS_NOT_FOUND,
  SEARCH_STATUS_CHILD_FOUND,
  SEARCH_STATUS_DONE
} SEARCH_STATUS;

typedef enum CONST_FOLDING_OP {
  CONST_FOLDING_OP_PLUS,
  CONST_FOLDING_OP_MINUS,
  CONST_FOLDING_OP_MULT,
  CONST_FOLDING_OP_DIV
} CONST_FOLDING_OP;

/* Global root for parse tree and abstract syntax tree */
node_t *root;

// Tasks
static void node_print(node_t *node, int nesting);
static void node_finalize(node_t *discard);
static void destroy_subtree(node_t *discard);
static node_t *simplify_tree(node_t *node);
static node_t *constant_fold_expression(node_t *node);
static node_t *replace_for_statement(node_t *for_node);

/* External interface */
void print_syntax_tree() {
  if (getenv("GRAPHVIZ_OUTPUT") != NULL)
    graphviz_node_print(root);
  else
    node_print(root, 0);
}

void simplify_syntax_tree(void) {
  root = simplify_tree(root);
}

void destroy_syntax_tree(void) {
  destroy_subtree(root);
  root = NULL;
}

/* Initialize a node with type, data, and children */
void node_init(node_t *nd, node_type_t type, void *data, uint64_t n_children, ...) {
  va_list child_list;
  *nd = (node_t){
      .type = type,
      .data = data,
      .symbol = NULL,
      .n_children = n_children,
      .children = (node_t **)malloc(n_children * sizeof(node_t *))};
  va_start(child_list, n_children);
  for (uint64_t i = 0; i < n_children; i++)
    nd->children[i] = va_arg(child_list, node_t *);
  va_end(child_list);
}

/* Inner workings */
/* Prints out the given node and all its children recursively */
static void node_print(node_t *node, int nesting) {
  if (node != NULL) {
    printf("%*s%s", nesting, "", node_strings[node->type]);
    if (node->type == IDENTIFIER_DATA ||
        node->type == STRING_DATA ||
        node->type == EXPRESSION ||
        node->type == RELATION)
      printf("(%s)", (char *)node->data);
    else if (node->type == NUMBER_DATA)
      printf("(%ld)", *((int64_t *)node->data));
    putchar('\n');
    for (int64_t i = 0; i < node->n_children; i++)
      node_print(node->children[i], nesting + 1);
  } else
    printf("%*s%p\n", nesting, "", node);
}

/* Frees the memory owned by the given node, but does not touch its children */
static void node_finalize(node_t *discard) {
  if (discard != NULL) {
    free(discard->data);
    free(discard->children);
    free(discard);
  }
}

/* Recursively frees the memory owned by the given node, and all its children */
static void destroy_subtree(node_t *discard) {
  if (discard != NULL) {
    for (uint64_t i = 0; i < discard->n_children; i++)
      destroy_subtree(discard->children[i]);
    node_finalize(discard);
  }
}

/* Wrapper function for strcmp so it makes sense */
int str_equals(char *str, char *target) {
  return !abs(strcmp(str, target));
}

/* Method for finding where in the tree the parent of target is. It "returns"
(pass by reference) the pointer to the parent, as well as the index of the target
in the children array. The method for finding is a variation of DFS. */
SEARCH_STATUS find_parent(node_t *current, node_t *target, node_t **result, uint64_t *index) {
  if (current == target) {
    return SEARCH_STATUS_CHILD_FOUND;
  }

  for (uint64_t i = 0; i < current->n_children; i++) {
    SEARCH_STATUS tmp = find_parent(current->children[i], target, result, index);
    if (tmp == SEARCH_STATUS_CHILD_FOUND) {
      *index = i;
      *result = current;
      return SEARCH_STATUS_DONE;
    } else if (tmp == SEARCH_STATUS_DONE) {
      return SEARCH_STATUS_DONE;
    }
  }
  return SEARCH_STATUS_NOT_FOUND;
}

/* Function for squashing itself, i.e killing itself and giving it's children
to it's parent. */
static node_t *squash_self(node_t *node) {
  // create the pointers we will use.
  node_t *parent = NULL;
  uint64_t index;
  // get the parent
  find_parent(root, node, &parent, &index);
  node_t *child = node->children[0];
  // replace the child of the parent
  parent->children[index] = child;
  // kill kill kill
  node_finalize(node);
  return child;
}

/* Function for squashing the child, i.e node kills it's only child and
takes all it's children. */
static node_t *squash_child(node_t *node) {
  // check if n_children is 0
  if (node->n_children == 0) {
    return node;
  }
  // we know node only has a single child in the beginning.
  node_t *child = node->children[0];
  // the number of children child has, will be the new numer of children
  // node has. Also realloc space for the new children.
  node->n_children = child->n_children;
  node->children = (node_t **)realloc(node->children, (node->n_children) * sizeof(node_t *));
  // iterate over the child's children, and give them to node
  for (uint64_t i = 0; i < node->n_children; i++) {
    node->children[i] = child->children[i];
  }
  // finally kill child
  node_finalize(child);
  return node;
}

/* Function for flattening a list */
static node_t *flatten_list(node_t *node) {
  // we know a list has either 1 or 2 children. If only one, it is already as flat as
  // it can be. If it has 2, the leftmost child is another list, while the right is
  // an item.
  if (node->n_children == 1) {
    return node;
  }
  // two children, leftmost is a list. We have to potentially create a bigger children array
  // and place the second child at the back of this new array.
  node_t *left_child = node->children[0];
  node_t *right_child = node->children[1];
  node->n_children = left_child->n_children + 1;
  // realloc
  node->children = (node_t **)realloc(node->children, (node->n_children) * sizeof(node_t *));

  // iterate and flatten by giving the left child's children to node
  uint64_t index = 0;
  while (index < left_child->n_children) {
    node->children[index] = left_child->children[index];
    index++;
  }
  // place right child at the last place
  node->children[index] = right_child;
  // kill left child
  node_finalize(left_child);
  return node;
}

/*
void main() {
  root = malloc(sizeof(node_t));
  node_t *n2 = malloc(sizeof(node_t));
  node_t *n3 = malloc(sizeof(node_t));
  node_t *n4 = malloc(sizeof(node_t));
  node_t *n5 = malloc(sizeof(node_t));
  node_init(root, GLOBAL_LIST, NULL, 1, n2);
  node_init(n2, FUNCTION, NULL, 2, n3, n4);
  node_init(n3, PARAMETER_LIST, NULL, 0);
  node_init(n4, STATEMENT, NULL, 1, n5);
  node_init(n5, BLOCK, NULL, 0);
  squash_self(n4);
  node_print(root, 0);
}
*/

/* Recursive function to convert a parse tree into an abstract syntax tree */
static node_t *simplify_tree(node_t *node) {
  if (node == NULL)
    return NULL;

  // Simplify everything is the node's subtree before proceeding
  for (uint64_t i = 0; i < node->n_children; i++)
    node->children[i] = simplify_tree(node->children[i]);

  switch (node->type) {
    case PROGRAM:
      root = node->children[0];
      node_finalize(node);
      return root;

    case GLOBAL_LIST:
      return flatten_list(node);

    case GLOBAL:
      return squash_self(node);

    case DECLARATION:
      return squash_child(node);

    case VARIABLE_LIST:
      return flatten_list(node);

    case ARRAY_DECLARATION:
      return squash_child(node);

    case ARRAY_INDEXING:
      break;

    case FUNCTION:
      break;

    case PARAMETER_LIST:
      return squash_child(node);

    case STATEMENT:
      return squash_self(node);

    case BLOCK:
      break;

    case DECLARATION_LIST:
      return flatten_list(node);

    case STATEMENT_LIST:
      return flatten_list(node);

    case ASSIGNMENT_STATEMENT:
      break;

    case RETURN_STATEMENT:
      break;

    case PRINT_STATEMENT:
      return squash_child(node);

    case PRINT_LIST:
      return flatten_list(node);

    case PRINT_ITEM:
      return squash_self(node);

    case BREAK_STATEMENT:
      break;

    case IF_STATEMENT:
      break;

    case WHILE_STATEMENT:
      break;

    case RELATION:
      break;

    case EXPRESSION_LIST:
      return flatten_list(node);

    case ARGUMENT_LIST:
      return squash_child(node);

    // Do contstant folding, if possible
    // Also prunes expressions that are just wrapping atomic expressions
    case EXPRESSION:
      return constant_fold_expression(node);

    case FOR_STATEMENT:
      return replace_for_statement(node);

    default:
      break;
  }

  return node;
}

/* Function for doing an unary operation. Stores the result in result. Returns true
if the assignment was successful, false otherwise. */
static bool unary_operation(int64_t number, int64_t *result, CONST_FOLDING_OP op) {
  switch (op) {
    case CONST_FOLDING_OP_MINUS:
      *result = -number;
      return true;

    default:
      break;
  }
  return false;
}

/* Function for doing a binary operation. Stores the result in result. Returns true
if the assignment was successful, false otherwise. */
static bool binary_operation(int64_t num1, int64_t num2, int64_t *result, CONST_FOLDING_OP op) {
  switch (op) {
    case CONST_FOLDING_OP_PLUS:
      *result = num1 + num2;
      return true;

    case CONST_FOLDING_OP_MINUS:
      *result = num1 - num2;
      return true;

    case CONST_FOLDING_OP_MULT:
      *result = num1 * num2;
      return true;

    case CONST_FOLDING_OP_DIV:
      *result = num1 / num2;
      return true;

    default:
      break;
  }
  return false;
}

/* Function for doing the actual folding. It calculate the result and create a new
node, discarding the subtree. */
static node_t *do_constant_fold(node_t *node, CONST_FOLDING_OP op) {
  // node should only have 1 or 2 children
  int64_t number;
  bool success;
  if (node->n_children == 1) {
    success = unary_operation(*((int64_t *)(node->children[0]->data)), &number, op);
  } else if (node->n_children == 2) {
    success = binary_operation(
        *((int64_t *)(node->children[0]->data)),
        *((int64_t *)(node->children[1]->data)),
        &number,
        op);
  } else {
    return node;
  }
  if (!success) {
    // something weird happend
    return node;
  }
  // get the parent
  node_t *parent = NULL;
  uint64_t index;
  find_parent(root, node, &parent, &index);
  // need to malloc data so the new node can own it
  int64_t *data = malloc(sizeof(int64_t));
  *data = number;
  // malloc new node and init
  node_t *folded_node = malloc(sizeof(node_t));
  node_init(folded_node, NUMBER_DATA, data, 0);
  // replace node with folded_node in the parent
  parent->children[index] = folded_node;
  // finally destroy the subtree
  destroy_subtree(node);
  return folded_node;
}

static node_t *constant_fold_expression(node_t *node) {
  assert(node->type == EXPRESSION);
  // if node data is null, i.e no operation and the node only has 1 child,
  // replace itself.
  if (node->n_children == 1 && node->data == NULL) {
    return squash_self(node);
  } else if (node->data == NULL || node->n_children == 0) {
    return node;
  }
  // check if all children are of type NUMBER_DATA
  bool all_number_data = true;
  for (uint64_t i = 0; i < node->n_children; i++) {
    if (node->children[i]->type != NUMBER_DATA) {
      all_number_data = false;
      break;
    }
  }
  if (!all_number_data) {
    return node;
  }

  // everything is number data, check operator
  if (str_equals(node->data, "+")) {
    return do_constant_fold(node, CONST_FOLDING_OP_PLUS);
  } else if (str_equals(node->data, "-")) {
    return do_constant_fold(node, CONST_FOLDING_OP_MINUS);
  } else if (str_equals(node->data, "*")) {
    return do_constant_fold(node, CONST_FOLDING_OP_MULT);
  } else if (str_equals(node->data, "/")) {
    return do_constant_fold(node, CONST_FOLDING_OP_DIV);
  }
  return node;
}

// Helper macros for manually building an AST
#define NODE(variable_name, ...)                  \
  node_t *variable_name = malloc(sizeof(node_t)); \
  node_init(variable_name, __VA_ARGS__)
// After an IDENTIFIER_NODE has been added to the tree, it can't be added again
// This macro replaces the given variable with a new node, containting a copy of the data
#define DUPLICATE_VARIABLE(variable)                     \
  do {                                                   \
    char *identifier = strdup(variable->data);           \
    variable = malloc(sizeof(node_t));                   \
    node_init(variable, IDENTIFIER_DATA, identifier, 0); \
  } while (false)
#define FOR_END_VARIABLE "__FOR_END__"

// Replaces the FOR_STATEMENT with a BLOCK.
// The block contains varables, setup, and a while loop
static node_t *replace_for_statement(node_t *for_node) {
  assert(for_node->type == FOR_STATEMENT);

  // extract child nodes from the FOR_STATEMENT
  node_t *variable = for_node->children[0];
  node_t *start_value = for_node->children[1];
  node_t *end_value = for_node->children[2];
  node_t *body = for_node->children[3];

  // TODO: Task 2.4
  // Replace the FOR_STATEMENT node, by instead creating the syntax tree of an equivalent block with a while-statement
  // As an example, the following statement:
  //
  // for i in 5..N+1
  //     print a[i]
  //
  // should become:
  //
  // begin
  //     var i, __FOR_END__
  //     i := 5
  //     __FOR_END__ := N+1
  //     while i < __FOR_END__ begin
  //         print a[i]
  //         i := i + 1
  //     end
  // end
  //

  // To aid in the manual creation of AST nodes, you can create named nodes using the NODE macro
  // As an example, the following creates the
  // var <variable>, __FOR_END__
  // part of the transformation
  NODE(end_variable, IDENTIFIER_DATA, strdup(FOR_END_VARIABLE), 0);
  NODE(variable_list, VARIABLE_LIST, NULL, 2, variable, end_variable);
  NODE(declaration, DECLARATION, NULL, 1, variable_list);
  NODE(declaration_list, DECLARATION_LIST, NULL, 1, declaration);

  // An important thing to note, is that nodes may not be re-used
  // since that will cause errors when freeing up the syntax tree later.
  // because we want to use a lot of IDENTIFIER_DATA nodes with the same data, we have the macro
  DUPLICATE_VARIABLE(variable);
  // Now we can use <variable> again, and it will be a new node for the same identifier!
  NODE(init_assignment, ASSIGNMENT_STATEMENT, NULL, 2, variable, start_value);
  // We do the same whenever we want to reuse <end_variable> as well
  DUPLICATE_VARIABLE(end_variable);
  NODE(end_assignment, ASSIGNMENT_STATEMENT, NULL, 2, end_variable, end_value);

  // TODO: The rest is up to you. Good luck!
  // Don't fret if this part gets too cumbersome. Try your best

  // TODO: Instead of returning the original for_node, destroy it, and return your equivalent block
  return for_node;
}
