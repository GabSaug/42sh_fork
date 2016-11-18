# include "execute.h"
  
int execute_command(struct tree* ast, struct hash_table* ht)
{
  struct tree* child = v_get(ast->child, 0);
  if (child->nts == SIMPLE_COMMAND)
    return execute_simple_command(child, ht);
  else if (child->nts == SHELL_COMMAND)
    return execute_shell_command(child, ht);
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
    warn("%s: Can't fork to execute the command", path);
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
    return WEXITSTATUS(exit_status);
  }
}

static int generate_command(struct tree *ast)
{
  size_t size = v_size(ast->childs);
  char **args = malloc(sizeof (char*) * (size + 1));

  for (size_t j = 0; j < size; j++)
    args[j] = get_child_elt(ast, j);
  args[size] = NULL;

  return args;
}

/*
static size_t get_size(struct tree *ast)
{
  size_t size = v_size(ast->child);

  size_t i = 0;
  struct tree *son = v_get(ast->child, i);
  son = v_get(son->child, 0);
  while (i < size && son->nts != REDIRECTION)
  {
    i++;
    if (i < size)
    {
      son = v_get(ast->child, i);
      son = v_get(son->child, 0);
    }
  }
  size = i;
  return size;
}
*/

static int execute_prog(char** argv, struct hash_table *ht)
{
  // Ca va changer lol
  int (*fun) (char** argv, struct hash_table* ht) = builtin_fun_match(argv[0]);
  if (fun)
    return fun(argv, ht);
  else
    return execute_bin(argv);
}

int execute_simple_command(struct tree *ast, struct hash_table *ht)
{
  char** argv = generate_command(ast);
  int res = execute_prog(argv, ht);
  free(argv);
  return res;
}
