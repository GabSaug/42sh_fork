# include "execute.h"

extern struct hash_table* ht[2];

int execute_command(struct tree* ast)
{
  struct tree* child = v_get(ast->child, 0);
  if (child->nts == SIMPLE_COMMAND)
    return execute_simple_command(child);
  else if (child->nts == SHELL_COMMAND)
    return execute_shell_command(child);
  else
    return 1;
}

char *get_child_elt(struct tree *ast, size_t elt)
{
  struct tree *son = v_get(ast->child, elt);
  son = v_get(son->child, 0);
  return son->token->s;
}

static int execute_bin(char** argv)
{
  int pid = fork();
  if (pid == -1)
  {
    warn("%s: Can't fork to execute the command", argv[0]);
    return 1;
  }
  else if (pid == 0)
  {
    // Child
    execvp(argv[0], argv);
    exit(127);
  }
  else
  {
    // Parent
    int exit_status = 0;
    waitpid(pid, &exit_status, 0);
    if (exit_status != 0)
      warnx("%s: comand not found", argv[0]);
    return WEXITSTATUS(exit_status);
  }
}

static size_t get_size(struct tree *ast)
{
  size_t size = v_size(ast->child);

  size_t i = 0;
  size_t count = 0;
  struct tree *son = v_get(ast->child, i);
  son = v_get(son->child, 0);
  while (i < size)
  {
    i++;
    if (son->nts != REDIRECTION)
      count++;

    if (i < size)
    {
      son = v_get(ast->child, i);
      son = v_get(son->child, 0);
    }
  }
  return count;
}

static char** generate_command(struct tree *ast, size_t *size)
{
  *size = get_size(ast);;
  char **args = malloc(sizeof (char*) * (*size + 1));
  size_t k = 0;

  for (size_t j = 0; j < v_size(ast->child); j++)
  {
    struct tree *son = v_get(ast->child, j);
    son = v_get(son->child, 0);
    if (son->nts != REDIRECTION)
    {
      args[k] = son->token->s;
      k++;
    }
  }
  args[*size] = NULL;

  return args;
}

static int execute_prog(char** argv)
{
  int (*fun) (char** argv) = builtin_fun_match(argv[0]);
  if (fun)
    return fun(argv);
  else
    return execute_bin(argv);
}

int execute_simple_command(struct tree *ast)
{
  size_t size;
  char** argv = generate_command(ast, &size);

  struct vector *to_close = NULL;
  int std_in = dup(0);
  int std_out = dup(1);
  int std_err = dup(2);
  if (size < v_size(ast->child))
  {
    to_close = managed_redirections(ast);
    if (!to_close)
    {
      free(argv);
      return 1;
    }
  }

  int res = execute_prog(argv);
  free(argv);
  if (to_close)
  {
    for (size_t i = 0; i < v_size(to_close); i++)
    {
      int *fd = v_get(to_close, i);
      close(*fd);
    }
    v_destroy(to_close, free);
  }

  dup2(std_in, 0);
  close(std_in);
  dup2(std_out, 1);
  close(std_out);
  dup2(std_err, 2);
  close(std_err);

  return res;
}
