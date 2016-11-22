#include <fnmatch.h>

#include "execute.h"

int execute_compound_list(struct tree *ast)
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

static int test_case_item(struct tree *ast, char *word)
{
  int res = 1;
  size_t i = 0;
  while (i < v_size(ast->child) && res)
  {
    struct tree *son = v_get(ast->child, i);
    if (son->token && son->token->id != WORD)
    {
      i++;
      continue;
    }
    res = fnmatch(son->token->s, word, 0);
    i++;
  }

  return res;
}

static int case_clause(struct tree *ast, char *word)
{
  int res = 0;

  for (size_t i = 0; i < v_size(ast->child); i += 2)
  {
    struct tree *son = v_get(ast->child, i);
    if (test_case_item(son, word) == 0)
    {
      son = v_get(son->child, v_size(son->child) - 1);
      if (son->nts == COMPOUND_LIST)
        res = execute_compound_list(son);
      break;
    }
  }

  return res;
}

int execute_case(struct tree *ast)
{
  struct tree *var = v_get(ast->child, 1);
  char *word = expand(var->token->s);
  struct tree *son = v_get(ast->child, 3);
  if (son->nts == CASE_CLAUSE)
    return case_clause(son, word);
  else
    return 0;
}
