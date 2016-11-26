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
 * \param tree The tree to draw
*/
void tree_print_dot(struct tree* tree);

/**
 * \brief Create a new tree with nts as its non terminal symbol
 * \param nts The non terminal symbol that is the root of the new tree
 * \return The created free, or NULL if an error occured
*/
struct tree* tree_create(enum non_terminal_symbol nts);

/**
 * \brief Add a leaf to the tree
 * \param tree The parent of the leaf to add
 * \param token The value to store in the leaf
*/
void tree_add_terminal_child(struct tree* tree, struct token* token);

/**
 * \brief Add an intern node to the tree
 * \param tree The parent of the intern node to add
 * \param child The sons of the node to create
*/
void tree_add_non_terminal_child(struct tree* tree, struct tree* child);

/**
 * \brief Destroy all the child of a node of a tree
 * \param tree The node to destroy all sons
*/
void tree_delete_all_child(struct tree* tree);

/**
 * \brief Destroy a tree and free the memory
 * \param tree The tree to destroy
*/
void tree_destroy(struct tree* tree);


/**
 * \brief Destroy the ast with the function to destroy vector
 * \param tree The tree to destroy
 * \param v_fun The function to destroy the vectors
*/
void tree_destroy_ast_extract_fun(struct tree* tree, struct vector* v_fun);

/**
 * \brief Function to destroy an element of a tree
 * \param p The parameter to destroy
*/
void tree_destroy_fun(void* p);

#endif /* !TREE_H */
