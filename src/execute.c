#include <stdlib.h>

#include "execute.h"
#include "lexer.h"

static int execute_input(struct tree* ast);
static int execute_list(struct tree* ast);
static int execute_and_or(struct tree* ast);
static int execute_pipeline(struct tree* ast);

int execute(struct tree* ast)
{
  return execute_input(ast);
}

static int execute_input(struct tree* ast)
{
  struct tree* child = v_get(ast->child, 0);
  if (child->nts == LIST)
    execute_list(child);
  return 0;
}

static int execute_list(struct tree* ast)
{
  struct tree* child = v_get(ast->child, 0);
  size_t i = 0;
  do {
    execute_and_or(child);
    i ++;
    child = v_get(ast->child, 2 * i);
  } while (child);
  return 0;
}

static int execute_and_or(struct tree* ast)
{
  struct tree* pipeline = v_get(ast->child, 0);
  size_t i = 0;
  int ret;
  do {
    ret = execute_pipeline(pipeline);
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

static int execute_pipeline(struct tree* ast)
{
  return EXIT_FAILURE;
}


