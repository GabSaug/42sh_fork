#include "execute.h"

int execute_shell_command(struct tree *ast, struct hash_table *ht)
{
  struct tree *child = v_get(ast->child, 0);
  if (child->nts == RULE_IF)
    return execute_if(child, ht);
  else
    return 1;
}

int execute_if(struct tree *ast, struct hash_table *ht)
{
  struct tree *compound_list = v_get(ast->child, 1);
  struct tree *cond = v_get(compound_list->child, 0);
  int condition = execute_and_or(cond, ht);

  if (condition == 0)
  {
    compound_list = v_get(ast->child, 3);
    struct tree *then = v_get(compound_list->child, 0);
    return execute_and_or(then, ht);
  }
  else
    return 0;
}
