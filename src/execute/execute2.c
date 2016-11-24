#include <stdio.h>

#include "execute.h"
#include "my_string.h"

extern struct hash_table* ht[3];

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

static char** generate_command(struct tree *ast, size_t index_start)
{
  struct vector* v_args = v_create();

  for (size_t j = index_start; j < v_size(ast->child); j++)
  {
    struct tree *son = v_get(ast->child, j);
    son = v_get(son->child, 0);
    if (son->nts != REDIRECTION)
    {
      struct vector* v_arg_tmp = expand(my_strdup(son->token->s), 0);
      v_concat(v_args, v_arg_tmp);
      v_destroy(v_arg_tmp, NULL);
    }
  }
  char **args = my_malloc(sizeof (char*) * (v_size(v_args) + 1));
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

static int execute_args(struct tree *ast, size_t i, struct vector *to_close,
                        int res)
{
  if (i == v_size(ast->child)) // Only prefix
  {
    for (size_t j = 0; j < v_size(ast->child); ++j)
    {
      struct tree* prefix = v_get(ast->child, j);
      res = execute_assignment(v_get(prefix->child, 0));
    }
  }
  else // only element
  {
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
  return res;
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
  for (i = 0; i < v_size(ast->child); ++i)
  {
    struct tree* element = v_get(ast->child, i); // i is offset of +1
    struct tree* child = v_get(element->child, 0);
    if (child->nts == 0)
    {
      struct token* token = child->token;
      if (!(strchr(token->s, '=') > token->s && !is_digit(token->s[0]))) // Is a WORD
        break;
    }
  }

  res = execute_args(ast, i, to_close, res);

  dup2(std_in, 0);
  close(std_in);
  dup2(std_out, 1);
  close(std_out);
  dup2(std_err, 2);
  close(std_err);

  return res;
}
