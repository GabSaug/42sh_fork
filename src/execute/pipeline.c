#include "execute.h"

static int launch_command(struct tree *ast, struct hash_table *ht)
{
  size_t i = 1;
  struct tree *child = v_get(ast->child, i);
  while (child->nts != COMMAND)
  {
    i++;
    child = v_get(ast->child, i);
  }

  return execute_command(child, ht);
}

static int staircase(struct tree *ast, int inout[], size_t index, size_t size,
                     struct hash_table *ht)
{
  dup2(inout[0], 0);
  int res = 0;
  if (index < size - 1)
  {
    int inout2[2];
    int status = pipe(inout2);
    if (status == -1)
        err(1, "Can not pipe command");

    if (!fork())
    {
      // child
      dup2(inout2[1], 1);
      close(inout2[0]);

      res = launch_command(v_get(ast->child, index), ht);
    }
    close(inout2[1]);
    close(inout[0]);

    res = staircase(ast, inout2, index + 1, size, ht);
  }
  else
  {
    res = launch_command(v_get(ast->child, index), ht);
    close(inout[0]);
  }

  return res;
}

int execute_pipeline(struct tree *ast, struct hash_table *ht)
{
  if (!ast)
    return 1;

  size_t size = v_size(ast->child);
  int res = 0;
  int index = ast->nts == COMMAND ? 0 : 1;

  if (size > 1)
  {
    int inout[2];
    int status = pipe(inout);
    if (status == -1)
        err(1, "Can not pipe command");

    if (!fork())
    {
      // child
      dup2(inout[1], 1);
      close(inout[0]);
      exit(execute_command(v_get(ast->child, index), ht));
    }
    close(inout[1]);

    res = staircase(ast, inout, index + 1, size, ht);
  }
  else
    res = execute_command(v_get(ast->child, 0), ht);

  return res;
}
