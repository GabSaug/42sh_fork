#ifndef TREE_H
# define TREE_H

# include "lexer.h"
# include "parser.h"

/**
 * \file tree.h
*/

struct tree
{
  struct token* token;
  enum non_terminal_symbol nts;
  struct vector* child;
};

/**
 * \brief Create a dot file that represent the AST
*/
void tree_print_dot(struct tree* tree);
void tree_print(struct tree* tree);

struct tree* tree_create(enum non_terminal_symbol nts);
void tree_add_terminal_child(struct tree* tree, struct token* token);
void tree_add_non_terminal_child(struct tree* tree, struct tree* child);
void tree_delete_all_child(struct tree* tree);
void tree_destroy(struct tree* tree);

#endif /* !TREE_H */
