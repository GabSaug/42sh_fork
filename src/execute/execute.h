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

int execute(struct tree* ast, struct hash_table *ht);
int execute_and_or(struct tree* ast, struct hash_table *ht);
int execute_command(struct tree* ast, struct hash_table* ht);
int execute_pipeline(struct tree *ast, struct hash_table *ht);
int execute_simple_command(struct tree *ast, struct hash_table *ht);
int execute_shell_command(struct tree *ast, struct hash_table *ht);
int builtin_execution(struct tree *ast, struct hash_table *ht, int bi);
char *get_child_elt(struct tree *ast, size_t elt);
int execute_if(struct tree *ast, struct hash_table *ht);

#endif /* !EXECUTE_H */
