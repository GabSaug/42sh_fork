#include "execute.h"

static int execute_compound_list(struct tree *ast)
{
  int res = 0;
  for (size_t i = 0; i < v_size(ast->child); i++)
  {
    struct tree *child = v_get(ast->child, i);
    if (child->nts == AND_OR)
      res = execute_and_or(child);
  }
  return res;
}

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
  int condition = execute_compound_list(compound_list);

  if (condition == 0)
  {
    compound_list = v_get(ast->child, 3);
    return execute_compound_list(compound_list);
  }
  else if (v_size(ast->child) > 5)
  {
    struct tree *compound_list = v_get(ast->child, 4);
    compound_list = v_get(compound_list->child, 1);
    return execute_compound_list(compound_list);
  }
  else
    return 0;
}

int execute_while(struct tree *ast)
{
  struct tree *cond = v_get(ast->child, 1);
  int ret = 0;
  while (execute_compound_list(cond) == 0)
  {
    struct tree *do_group = v_get(ast->child, 2);
    struct tree *compound_list = v_get(do_group->child, 1);
    ret = execute_compound_list(compound_list);
  }
  return ret;
}

int execute_until(struct tree *ast)
{
  struct tree *cond = v_get(ast->child, 1);
  int ret = 0;
  while (execute_compound_list(cond) != 0)
  {
    struct tree *do_group = v_get(ast->child, 2);
    struct tree *compound_list = v_get(do_group->child, 1);
    ret = execute_compound_list(compound_list);
  }
  return ret;
}
