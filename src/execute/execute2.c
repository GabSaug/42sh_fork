#include <stdio.h>

#include "execute.h"
#include "my_string.h"

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
    warnx("%s: command not found", argv[0]);
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

/*static size_t get_size(struct tree *ast)
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
}*/

/*static struct vector* generate_command(struct tree *ast)
{
  size_t size = v_size(ast->child);
  struct vector* args = v_create();

  for (size_t j = 0; j < size; j++)
  {
    struct vector* arg_tmp = expand(get_child_elt(ast, j));
    v_concat(args, arg_tmp);
    v_destroy(arg_tmp, NULL);
  }

  return args;
}*/

static char** generate_command(struct tree *ast, size_t index_start)
{
  /**size = get_size(ast);;
  char **args = malloc(sizeof (char*) * (*size + 1));
  size_t k = 0;*/

  struct vector* v_args = v_create();

  for (size_t j = index_start; j < v_size(ast->child); j++)
  {
    struct tree *son = v_get(ast->child, j);
    son = v_get(son->child, 0);
    if (son->nts != REDIRECTION)
    {
      /*args[k] = son->token->s;
      k++;*/
      struct vector* v_arg_tmp = expand(my_strdup(son->token->s));
      v_concat(v_args, v_arg_tmp);
      v_destroy(v_arg_tmp, NULL);
    }
  }
  char **args = malloc(sizeof (char*) * (v_size(v_args) + 1));
  for (size_t i = 0; i < v_size(v_args); ++i)
    args[i] = v_get(v_args, i);
  args[v_size(v_args)] = NULL;
  v_destroy(v_args, NULL);

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

static int execute_assignment(struct tree* assignment)
{
  char* s = assignment->token->s;
  char* equal = strchr(s, '=');
  *equal = '\0';
  //printf("key = %s$ data = %s$\n", s, equal + 1);
  add_hash(ht[VAR], s, equal + 1);
  return 0;
}

int execute_simple_command(struct tree *ast)
{
  int res = 0;
  struct vector *to_close = NULL;
  int std_in = dup(0);
  int std_out = dup(1);
  int std_err = dup(2);
  to_close = managed_redirections(ast);
  if (!to_close)
    return 1;

  size_t i = 0;
  for (i = v_size(ast->child); i > 0; --i)
  {
    struct tree* child = v_get(ast->child, i - 1); // i is offset of +1
    if (child->nts == PREFIX)
      break;
  }
  if (i == v_size(ast->child)) // Only prefix
  {
    //printf("only prefix\n");
    for (size_t j = 0; j < v_size(ast->child); ++j)
    {
      struct tree* prefix = v_get(ast->child, j);
      res = execute_assignment(v_get(prefix->child, 0));
    }
  }
  else // only element
  {
    //printf("element\n");
    char** argv = generate_command(ast, i);
    res = execute_prog(argv);
    for (size_t i = 0; argv[i]; ++i)
      free(argv[i]);
    free(argv);
  }
    
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

  /*size_t size;
  char** argv = generate_command(ast, &size);

  struct vector *to_close = NULL;
  int std_in = dup(0);
  int std_out = dup(1);
  int std_err = dup(2);
  //if (size < v_size(ast->child))
  //{
    to_close = managed_redirections(ast);
    if (!to_close)
    {
      free(argv);
      return 1;
    }
  //}

  int res = execute_prog(argv);
  for (size_t i = 0; argv[i]; ++i)
    free(argv[i]);
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

  //return res;
}*/
