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
int execute_command(struct tree* ast, struct hash_table* ht);
int execute_pipeline(struct tree *ast, struct hash_table *ht);
int execute_simple_command(struct tree *ast, struct hash_table *ht);

#endif /* !EXECUTE_H */
