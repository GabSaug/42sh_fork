#ifndef EXECUTE_H
# define EXECUTE_H

# include <stdlib.h>
# include <string.h>
# include <err.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <unistd.h>

# include "expansion.h"
# include "hash_table.h"
# include "tree.h"
# include "vector.h"

/**
 * \file execute.h
*/

struct builtin_fun
{
  char name[20];
  int (*fun) (char* argv[], struct hash_table *ht[]);
};

int (*builtin_fun_match (char* s)) (char* argv[], struct hash_table *ht[]);

/**
 * \brief Execute the AST to run commands
 * \param ast The AST given by the parser
 * \param ht The hash tables of variables, functions and aliases
 * \return The success or failure of the execution
*/
int execute(struct tree* ast, struct hash_table *ht[]);

/**
 * \brief Execute the rule and_or of a AST
 * \param ast The AST with in the root the rule and_or
 * \param ht The hash tables of variables, functions and aliases
 * \return The success or failure of execution
*/
int execute_and_or(struct tree* ast, struct hash_table *ht[]);

/**
 * \brief Execute the rule command of a AST
 * \param ast The AST with in the root the rule command
 * \param ht The hash tables of variables, functions and aliases
 * \return The success or failure of execution
*/
int execute_command(struct tree* ast, struct hash_table *ht[]);

/**
 * \brief Execute the rule pipeline of a AST
 * \param ast The AST with in the root the rule pipeline
 * \param ht The hash tables of variables, functions and aliases
 * \return The success or failure of execution
*/
int execute_pipeline(struct tree *ast, struct hash_table *ht[]);

/**
 * \brief Execute the rule simple_command of a AST
 * \param ast The AST with in the root the rule simple_command
 * \param ht The hash tables of variables, functions and aliases
 * \return The success or failure of execution
*/
int execute_simple_command(struct tree *ast, struct hash_table *ht[]);

/**
 * \brief Execute the rule shell_command of a AST
 * \param ast The AST with in the root the rule shell_command
 * \return The success or failure of execution
*/
int execute_shell_command(struct tree *ast, struct hash_table *ht[]);

/**
 * \brief Get the element of the first child of the child of the AST
 * \param ast The ast to get the element
 * \param elt The child of ast to get the element of the first child
 * \return the element found
*/
char *get_child_elt(struct tree *ast, size_t elt);

/**
 * \brief Execute a compound list of a shell command
 * \param ast The ast with the rule compound_list in the root
 * \param ht The hash tables of variables, functions and aliases
 * \return The success or failure of the last execution of the compound list
*/
int execute_compound_list(struct tree *ast, struct hash_table *ht[]);

/**
 * \brief Execute the rule if of a AST
 * \param ast The AST with in the root the rule if
 * \param ht The hash tables of variables, functions and aliases
 * \return The success or failure of execution
*/
int execute_if(struct tree *ast, struct hash_table *ht[]);

/**
 * \brief Execute the rule while of a AST
 * \param ast The AST with in the root the rule while
 * \param ht The hash tables of variables, functions and aliases
 * \return The success or failure of execution
*/
int execute_while(struct tree *ast, struct hash_table *ht[]);

/**
 * \brief Execute the rule until of a AST
 * \param ast The AST with in the rule until in the root
 * \param ht The hash tables of variables, functions and aliases
 * \return The success or failure of the last execution of the loop
*/
int execute_until(struct tree *ast, struct hash_table *ht[]);

/**
 * \brief Execute the rule for of a AST
 * \param ast The AST with the rule for in the root
 * \param ht The hash tables of variables, functions and aliases
 * \return The success or failure of the last execution of the loop
*/
int execute_for(struct tree *ast, struct hash_table *ht[]);

/**
 * \brief Execute the rule ase of a AST
 * \param ast The AST with the rule case in the root
 * \param ht The hash tables of variables, functions and aliases
 * \return The success or failure of the executin of the case
*/
int execute_case(struct tree *ast, struct hash_table *ht[]);

char *expand_word(struct vector *exp);

/**
 * \brief Managed the redirection of a command
 * \param ast The command where there is redirection
 * \return A vector of file descriptors to close after the execution of the
 * command
*/
struct vector *managed_redirections(struct tree *ast);

#endif /* !EXECUTE_H */
