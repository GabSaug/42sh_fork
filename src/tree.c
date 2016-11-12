#include <stdlib.h>

#include "my_malloc.h"
#include "vector.h"
#include "tree.h"

struct tree* tree_create(enum non_terminal_symbol nts)
{
  struct tree* tree = my_malloc(sizeof (struct tree));
  tree->nts = nts;
  tree->child = v_create();
  return tree;
}

void tree_add_terminal_child(struct tree* tree, enum terminal_symbol ts)
{
  enum terminal_symbol* terminal_symbol = my_malloc(sizeof (enum terminal_symbol));
  *terminal_symbol = ts;
  v_append(tree->child, terminal_symbol);
}

void tree_add_non_terminal_child(struct tree* tree, struct tree* child)
{
  v_append(tree->child, child);
}

void tree_delete_all_child(struct tree* tree)
{
  v_erase(tree->child);
}

void tree_destroy(struct tree* tree)
{
  v_destroy(tree->child);
  free(tree);
}
