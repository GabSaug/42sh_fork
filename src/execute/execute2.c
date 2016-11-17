# include "execute.h"

int execute_command(struct tree* ast, struct hash_table* ht)
{
  struct tree* child = v_get(ast->child, 0);
  if (child->nts == SIMPLE_COMMAND)
    return execute_simple_command(child, ht);
  else
    return 1;
}

char *get_child_elt(struct tree *ast, size_t elt)
{
  struct tree *son = v_get(ast->child, elt);
  son = v_get(son->child, 0);
  return son->token->s;
}

static int bin_execution(char *path, char *args[])
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
    execv(path, args);
    exit(127);
  }
  else
  {
    // Parent
    int exit_status = 0;
    waitpid(pid, &exit_status, 0);
    free(path);
    free(args);
    return WEXITSTATUS(exit_status);
  }
}

static int concat_path(struct tree *ast, char *path, char *command_name,
                       size_t size)
{
  size_t size_cmd = strlen(command_name);
  char *call = NULL;
  if (command_name[size_cmd - 1] == '/')
  {
    call = malloc(sizeof (char) * (1 + strlen(path) + size_cmd));
    call = strcpy(call, path);
    call = strcat(call, command_name);
  }
  else
  {
    call = malloc(sizeof (char) * (2 + strlen(path) + size_cmd));
    call = strcpy(call, path);
    call = strcat(call, "/");
    call = strcat(call, command_name);
  }

  char **args = malloc(sizeof (char *) * (1 + size));
  args[0] = call;

  for (size_t j = 1; j < size; j++)
  {
    args[j] = get_child_elt(ast, j);
  }
  args[size] = NULL;

  return bin_execution(call, args);
}

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

static int binary_from_path(struct tree *ast, struct hash_table *ht,
                            char *command_name, size_t size)
{
  char *path = get_data(ht, "PATH");
  size_t size_path = strlen(path);
  size_t k1 = 0;
  size_t k2 = 0;
  while (path[k2] && path[k2] != ':')
    k2++;
  path[k2] = '\0';
  int res = concat_path(ast, path + k1, command_name, size);
  while (res == 127 && k2 < size_path)
  {
    path[k2] = ':';
    k2++;
    k1 = k2;
    while (path[k2] && path[k2] != ':')
      k2++;
    path[k2] = '\0';
    res = concat_path(ast, path + k1, command_name, size);
  }
  if (k2 < size_path)
    path[k2] = ':';

  if (res == 127)
    warnx("%s: command not found", command_name);
  return res;
}

static int execute_prog(struct tree *ast, struct hash_table *ht)
{
  size_t size = get_size(ast);
  char *command_name = get_child_elt(ast, 0);

  if (strlen(command_name) > 2 && strncmp(command_name, "./", 2) == 0)
  {
    char *path = get_data(ht, "PWD");
    command_name++;
    return concat_path(ast, path, command_name, size);
  }
  else if (strlen(command_name) > 1 && strncmp(command_name, "/", 1) == 0)
  {
    return concat_path(ast, "", command_name + 1, size);
  }
  else
    return binary_from_path(ast, ht, command_name, size);
}

int execute_simple_command(struct tree *ast, struct hash_table *ht)
{
  ht = ht;
  // TODO need to verify if there is an assignment word, to create or update a
  // variable
  char *builtin[] =
  {
    "exit", "true", "false", "cd", "shopt", "export", "alias", "unalias",
    "echo", "continue", "break", "source", "history"
  };

  int bi = -1;
  for (size_t i = 0; i < sizeof (builtin) / sizeof (char *); i++)
  {
    if (strcmp(builtin[i], get_child_elt(ast, 0)) == 0)
    {
      bi = i;
      break;
    }
  }
  if (bi >= 0)
    return builtin_execution(ast, ht, bi);
  else
    return execute_prog(ast, ht);
  // TODO need to implement here the redirections
}
