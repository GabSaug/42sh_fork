#include "execute.h"

int execute_shell_command(struct tree *ast)
{
  struct tree *child = v_get(ast->child, 0);
  if (child->nts == RULE_IF)
    return execute_if(child);
  else if (child->nts == RULE_WHILE)
    return execute_while(child);
  else if (child->nts == RULE_UNTIL)
    return execute_until(child);
  else
    return 1;
}

int execute_if(struct tree *ast)
{
  struct tree *compound_list = v_get(ast->child, 1);
  struct tree *cond = v_get(compound_list->child, 0);
  int condition = execute_and_or(cond);

  if (condition == 0)
  {
    compound_list = v_get(ast->child, 3);
    struct tree *then = v_get(compound_list->child, 0);
    return execute_and_or(then);
  }
  else
    return 0;
}

int execute_while(struct tree *ast)
{
  struct tree *compound_list = v_get(ast->child, 1);
  struct tree *cond = v_get(compound_list->child, 0);
  int ret = 0;
  while (execute_and_or(cond) == 0)
  {
    compound_list = v_get(ast->child, 3);
    struct tree *then = v_get(compound_list->child, 0);
    ret = execute_and_or(then);
  }
  return ret;
}

int execute_until(struct tree *ast)
{
  struct tree *compound_list = v_get(ast->child, 1);
  struct tree *cond = v_get(compound_list->child, 0);
  int ret = 0;
  while (execute_and_or(cond) != 0)
  {
    compound_list = v_get(ast->child, 3);
    struct tree *then = v_get(compound_list->child, 0);
    ret = execute_and_or(then);
  }
  return ret;

}
