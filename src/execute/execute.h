#ifndef EXECUTE_H
# define EXECUTE_H

# include <string.h>
# include <err.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

# include "hash_table.h"
# include "tree.h"
# include "vector.h"

struct builtin_fun
{
  char name[20];
  int (*fun) (char* argv[]);
};


int execute(struct tree* ast);
int execute_and_or(struct tree* ast);
int execute_command(struct tree* ast);
int execute_pipeline(struct tree *ast);
int execute_simple_command(struct tree *ast);
int execute_shell_command(struct tree *ast);
//int builtin_execution(struct tree *ast, int bi);
int (*builtin_fun_match (char* s)) (char* argv[]);
char *get_child_elt(struct tree *ast, size_t elt);
int execute_if(struct tree *ast);
int execute_while(struct tree *ast);

#endif /* !EXECUTE_H */
