#include <stdlib.h>
#include <stdio.h>

#include "my_malloc.h"
#include "vector.h"
#include "tree.h"


static void tree_print_rec(struct tree* tree, int indent)
{
  if (tree == NULL)
    return;
  if (tree->ts != UNDIFINED)
  {
    printf("leaf = %d\n", tree->ts);
  }
  else
  {
    printf("node has %zu child, name = %d\n", tree->child->size, tree->nts);
    for (size_t i = 0; i < tree->child->size; ++i)
    {
      printf("%*sch %zu : ", 8*indent, "", i);
      tree_print_rec(v_get(tree->child, i), indent + 1);
    }
  }
}

void tree_print(struct tree* tree)
{
  tree_print_rec(tree, 0);
}

struct tree* tree_create(enum non_terminal_symbol nts)
{
  struct tree* tree = my_malloc(sizeof (struct tree));
  tree->nts = nts;
  tree->ts = UNDIFINED;
  tree->child = v_create();
  return tree;
}

void tree_add_terminal_child(struct tree* tree, enum terminal_symbol ts)
{
  struct tree* leaf = tree_create(0);
  leaf->ts = ts;
  /*enum terminal_symbol* terminal_symbol = my_malloc(sizeof (enum terminal_symbol));
  tree->ts = my_malloc(sizeof (enum terminal_symbol));
  tree->ts = ts;*/
  v_append(tree->child, leaf);
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
  if (!tree)
    return;
  v_destroy(tree->child);
  free(tree);
}
