#include "execute.h"

static struct tree *search_command(struct tree *ast, size_t *index)
{
  struct tree *child = v_get(ast->child, *index);
  while (child->nts != COMMAND)
  {
    *index += 1;
    child = v_get(ast->child, *index);
  }

  return child;
}

static void fork_pipe(struct tree *child, int inout[],
                      int inout2[], struct hash_table *ht[])
{
  int pid = fork();
  if (!pid)
  {
    // child
    dup2(inout[0], 0);
    dup2(inout2[1], 1);
    close(inout2[0]);

    exit(execute_command(child, ht));
  }
  int child_stat = 0;
  waitpid(pid, &child_stat, 0);
  close(inout2[1]);
  close(inout[0]);
}

static int staircase(struct tree *ast, int inout[], size_t index, size_t size,
                     struct hash_table *ht[])
{
  struct tree *child = search_command(ast, &index);

  int res = 0;
  if (index < size - 1)
  {
    int inout2[2];
    int status = pipe(inout2);
    if (status == -1)
        err(1, "Can not pipe command");

    fork_pipe(child, inout, inout2, ht);
    res = staircase(ast, inout2, index + 1, size, ht);
  }
  else
  {
    int pid = fork();
    if (!pid)
    {
      dup2(inout[0], 0);
      exit(execute_command(child, ht));
    }
    int child_stat = 0;
    waitpid(pid, &child_stat, 0);
    res = WEXITSTATUS(child_stat);
  }

  return res;
}

static void fork_first_pipe(struct tree *ast, int inout[], size_t index,
                            struct hash_table *ht[])
{
  int pid = fork();
  if (!pid)
  {
    // child
    dup2(inout[1], 1);
    close(inout[0]);
    exit(execute_command(v_get(ast->child, index), ht));
  }
  int child_stat = 0;
  waitpid(pid, &child_stat, 0);
  close(inout[1]);
}

int execute_pipeline(struct tree *ast, struct hash_table *ht[])
{
  if (!ast)
    return 1;

  size_t size = v_size(ast->child);
  struct tree *child = v_get(ast->child, 0);
  size_t index = child->nts == COMMAND ? 0 : 1;
  int res = 0;

  if (size > 1)
  {
    int inout[2];
    int status = pipe(inout);
    if (status == -1)
        err(1, "Can not pipe command");

    fork_first_pipe(ast, inout, index, ht);
    res = staircase(ast, inout, index + 1, size, ht);
  }
  else
    res = execute_command(v_get(ast->child, index), ht);

  return index > 0 ? !res : res;
}
