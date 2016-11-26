#include <stdlib.h>

#include "execute.h"
#include "lexer.h"

int execute_and_or(struct tree* ast, struct hash_table *ht[])
{
  struct tree* pipeline = v_get(ast->child, 0);
  size_t i = 0;
  int ret;
  do {
    ret = execute_pipeline(pipeline, ht);
    i++;
    struct tree* operator = v_get(ast->child, i);
    if (!operator)
      break;
    operator = v_get(operator->child, 0);
    if ((operator->token->id == AND_IF && ret != EXIT_SUCCESS)
        || (operator->token->id == OR_IF && ret == EXIT_SUCCESS))
      i += 2;
    i++;
    pipeline = v_get(ast->child, i);
  } while (pipeline);
  return ret;
}

static int execute_list(struct tree* ast, struct hash_table *ht[])
{
  int res = 0;
  struct tree* child = v_get(ast->child, 0);
  size_t i = 0;
  do {
    res = execute_and_or(child, ht);
    i ++;
    child = v_get(ast->child, 2 * i);
  } while (child);
  return res;
}

static int execute_input(struct tree* ast, struct hash_table *ht[])
{
  struct tree* child = v_get(ast->child, 0);
  int res = 0;
  if (child->nts == LIST)
    res = execute_list(child, ht);
  return res;
}

int execute(struct tree* ast, struct hash_table *ht[])
{
  return execute_input(ast, ht);
}
