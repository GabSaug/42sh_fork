#include <stdlib.h>

#include "execute.h"
#include "lexer.h"

int execute_command(struct tree* ast, struct hash_table* ht)
{
  struct tree* child = v_get(ast->child, 0);
  if (child->token == SIMPLE_COMMAND)
    return execute_simple_command(child, ht);
}
