#include <stdlib.h>

#include "execute.h"
#include "lexer.h"

static int execute_input(struct tree* ast, struct hash_table *ht);
static int execute_list(struct tree* ast, struct hash_table *ht);
static int execute_and_or(struct tree* ast, struct hash_table *ht);

int execute(struct tree* ast, struct hash_table *ht)
{
  return execute_input(ast, ht);
}

static int execute_input(struct tree* ast, struct hash_table *ht)
{
  struct tree* child = v_get(ast->child, 0);
  if (child->nts == LIST)
    execute_list(child, ht);
  return 0;
}

static int execute_list(struct tree* ast, struct hash_table *ht)
{
  struct tree* child = v_get(ast->child, 0);
  size_t i = 0;
  do {
    execute_and_or(child, ht);
    i ++;
    child = v_get(ast->child, 2 * i);
  } while (child);
  return 0;
}

static int execute_and_or(struct tree* ast, struct hash_table *ht)
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
    if (operator->token->id == AND_IF && ret == EXIT_FAILURE)
      i += 2;
    i++;
    pipeline = v_get(ast->child, i);
  } while (pipeline);
  return 0;
}
