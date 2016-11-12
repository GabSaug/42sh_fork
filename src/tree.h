#ifndef TREE_H
# define TREE_H

# include "lexer.h"
# include "parser.h"

struct tree
{
  enum terminal_symbol ts;
  enum non_terminal_symbol nts;
  struct vector* child;
};

void tree_print(struct tree* tree, int indent);

struct tree* tree_create(enum non_terminal_symbol nts);
void tree_add_terminal_child(struct tree* tree, enum terminal_symbol ts);
void tree_add_non_terminal_child(struct tree* tree, struct tree* child);
void tree_delete_all_child(struct tree* tree);
void tree_destroy(struct tree* tree);

#endif /* !TREE_H */
