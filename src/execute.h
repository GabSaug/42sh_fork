#ifndef EXECUTE_H
# define EXECUTE_H

# include "tree.h"

int execute(struct tree* ast);
int execute_command(struct tree* ast, struct hash_table* ht);

#endif /* !EXECUTE_H */
