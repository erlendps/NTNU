#define NODETYPES_IMPLEMENTATION
#include <assert.h>
#include <stdlib.h>
#include <vslc.h>

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
void print_syntax_tree(void) {
  if (getenv("GRAPHVIZ_OUTPUT") != NULL)
    graphviz_node_print(root);
  else
    node_print(root, 0);
}

void simplify_syntax_tree(void) {
  root = simplify_tree(root);
}

void destroy_syntax_tree(void) {
  // This will destroy the enitre tree by deleting root and all its children
  destroy_subtree(root);
  root = NULL;
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

/* Initialize a node with type, data, and children */
void node_init(node_t *nd, node_type_t type, void *data, uint64_t n_children, ...) {
  // set type, data and n_children
  nd->type = type;
  nd->data = data;
  nd->n_children = n_children;

  // symbol should be null on init
  nd->symbol = NULL;

  // set children
  // set up va_args
  va_list varargs;
  va_start(varargs, n_children);
  // allocate space
  if ((nd->children = (node_t **)malloc(n_children * sizeof(node_t *))) < 0) {
    printf("Error allocating\n");
    exit(1);
  }
  // set up the children array to point to the children
  for (int i = 0; i < n_children; i++) {
    nd->children[i] = va_arg(varargs, node_t *);
  }
  va_end(varargs);
}

/* Frees the memory owned by the given node, but does not touch its children */
static void node_finalize(node_t *discard) {
  if (discard != NULL) {
    free(discard->data);
    free(discard->children);
    free(discard);
  }
}

/* Frees the memory owned by the given node, and all its children */
static void destroy_subtree(node_t *discard) {
  // check if discard is null
  if (discard != NULL) {
    // recursively destroy subtrees
    for (uint64_t i = 0; i < discard->n_children; i++)
      destroy_subtree(discard->children[i]);
    // free node
    node_finalize(discard);
  }
}

// finds and returns a pointer to the parent of the node
// it also returns the index of the node.
node_t *find_parent(node_t *current, node_t *target, uint64_t *index) {
  node_t *result = NULL;
  if (current == target) {
    return target;
  }
  for (uint64_t i = 0; i < current->n_children; i++) {
    result = find_parent(current->children[i], target, index);
    if (result == target) {
      *index = i;
      return current;
    }
  }
  return result;
}

/*
void main() {
  node_t *n1 = malloc(sizeof(node_t));
  node_t *n2 = malloc(sizeof(node_t));
  node_t *n3 = malloc(sizeof(node_t));
  node_t *n4 = malloc(sizeof(node_t));
  node_t *n5 = malloc(sizeof(node_t));
  node_init(n1, GLOBAL_LIST, NULL, 1, n2);
  node_init(n2, FUNCTION, NULL, 2, n3, n4);
  node_init(n3, PARAMETER_LIST, NULL, 0);
  node_init(n4, STATEMENT, NULL, 1, n5);
  node_init(n5, BLOCK, NULL, 0);
  uint64_t index;
  node_t *n41 = find_parent(n1, n4, &index);
  if (n41 == NULL) {
    printf("\nNULL\n");
  }
}
*/

// method for squashing self, i.e replacing itself with the child
static node_t *squash_self(node_t *node) {
  uint64_t index;
  node_t *parent = find_parent(root, node, &index);
  if (parent == NULL) {
    return node;
  }
  node_t *child = node->children[0];
  parent->children[index] = child;
  node_finalize(node);
  return child;
}

// method for squashing this nodes child (kill it) and steal
// that nodes children
static node_t *squash_child(node_t *node) {
  // always best to be sure
  if (node == NULL) {
    return NULL;
  }
  // to continue, we have to make sure the node only has one children.
  if (node->n_children != 1) {
    return node;
  }
  node_t *child = node->children[0];
  node->n_children = child->n_children;
  // first realloc the children pointer with the new num of children
  node->children = (node_t **)realloc(node->children, node->n_children * sizeof(node_t *));
  // steal the children
  for (uint64_t i = 0; i < child->n_children; i++) {
    node->children[i] = child->children[i];
  }
  // free the squashed child and return the node
  node_finalize(child);
  return node;
}

static node_t *flatten_list(node_t *node) {
  // if a list has 2 children, then the leftmost is another list.
  // if the list only has 1 child, do nothing
  if (node->n_children != 2) {
    return node;
  }
  // uint64_t num_of_children = node->children[0]->n_children + 1;
  uint64_t num_of_children = 0;
  for (uint64_t i = 0; i < node->n_children; i++) {
    num_of_children += node->children[i]->n_children;
  }
  node_t *list_child = node->children[0];
  node_t *item_child = node->children[1];
  node->n_children = num_of_children;
  node->children = (node_t **)realloc(node->children, node->n_children * sizeof(node_t *));
  uint64_t count = 0;
  for (uint64_t i = 0; i < (list_child->n_children); i++) {
    node->children[count++] = list_child->children[i];
  }
  node->children[count] = item_child;
  // node->children[count] = item_child->children[0];
  node_finalize(list_child);
  // node_finalize(item_child);
  return node;
}

/* Recursive function to convert a parse tree into an abstract syntax tree */
static node_t *simplify_tree(node_t *node) {
  if (node == NULL)
    return NULL;

  // Simplify everything is the node's subtree before proceeding
  for (uint64_t i = 0; i < node->n_children; i++)
    node->children[i] = simplify_tree(node->children[i]);

  switch (node->type) {
    // TODO: Task 2.1
    // Eliminate nodes of purely syntactic value.
    // These nodes only have one child, and carry no semantic value.

    // For nodes that only serve as a wrapper for a (optional) node below,
    // you may squash the child and take over its children instead.
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

    case PARAMETER_LIST:
      return squash_child(node);

    case STATEMENT:
      return squash_self(node);

    case DECLARATION_LIST:
      return flatten_list(node);

    case STATEMENT_LIST:
      return flatten_list(node);

    case PRINT_STATEMENT:
      return squash_child(node);

    case PRINT_LIST:
      return flatten_list(node);

    case PRINT_ITEM:
      return squash_self(node);

    case EXPRESSION_LIST:
      return flatten_list(node);

    case ARGUMENT_LIST:
      return squash_child(node);

    // TODO: Task 2.2
    // Flatten linked list structures.
    // Any list node with two children, has a list node to the left, and an element to the right.
    // We return the left list node, but with the right node appended to its list

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

static node_t *constant_fold_expression(node_t *node) {
  assert(node->type == EXPRESSION);

  // TODO: Task 2.3
  // Replace expression nodes by their values, if it is possible to compute them now

  // First, expression nodes with no operator, and only one child, are just wrappers for
  //   NUMBER_DATA, IDENTIFIER_DATA or ARRAY_INDEXING, and can be replaced by its children

  // For expression nodes that are operators, we can only do constant folding if all its children are NUMBER_DATA.
  // In such cases, the expression node can be replaced with the value of its operator, applied to its child(ren).

  // Remember to free up the memory used by the original node(s), if they get replaced by a new node
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