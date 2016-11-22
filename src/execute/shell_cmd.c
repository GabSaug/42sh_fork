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
  else if (child->nts == RULE_FOR)
    return execute_for(child);
  else if (child->nts == RULE_CASE)
    return execute_case(child);
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
  int res = 0;
  while (execute_compound_list(cond) == 0)
  {
    struct tree *do_group = v_get(ast->child, 2);
    struct tree *compound_list = v_get(do_group->child, 1);
    res = execute_compound_list(compound_list);
  }
  return res;
}

int execute_until(struct tree *ast)
{
  struct tree *cond = v_get(ast->child, 1);
  int res = 0;
  while (execute_compound_list(cond) != 0)
  {
    struct tree *do_group = v_get(ast->child, 2);
    struct tree *compound_list = v_get(do_group->child, 1);
    res = execute_compound_list(compound_list);
  }
  return res;
}

int execute_for(struct tree *ast)
{
  struct tree *son = v_get(ast->child, 1);
  char *var = son->token->s;

  struct tree *compound_list = v_get(ast->child, v_size(ast->child) - 1);
  compound_list = v_get(compound_list->child, 1);
  int res;

  son = v_get(ast->child, 3);
  for (size_t i = 3; son->nts == WORD_RULE; i++)
  {
    son = v_get(son->child, 0);
    ht[VAR] = add_hash(ht[VAR], var, son->token->s);
    res = execute_compound_list(compound_list);
    son = v_get(ast->child, i + 1);
  }

  del_hash(ht[VAR], var);

  return res;
}
