# include "execute.h"

int execute_command(struct tree* ast, struct hash_table* ht)
{
  struct tree* child = v_get(ast->child, 0);
  if (child->nts == SIMPLE_COMMAND)
    return execute_simple_command(child, ht);
  else
    return 1;
}

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

  if (index)
    return !res;
  else
    return res;
}


static char *get_child_elt(struct tree *ast, size_t elt)
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

static int execute_prog(struct tree *ast, struct hash_table *ht)
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

  char *command_name = get_child_elt(ast, 0);

  char *path = NULL;
  if (strlen(command_name) > 2 && strncmp(command_name, "./", 2) == 0)
  {
    path = get_data(ht, "PWD");
    command_name++;
    return concat_path(ast, path, command_name, size);
  }
  else
  {
    path = get_data(ht, "PATH");
    size_t size_path = strlen(path);
    size_t k1 = 0;
    size_t k2 = 0;
    while (path[k2] && path[k2] != ':')
      k2++;
    path[k2] = '\0';
    int res = concat_path(ast, path + k1, command_name, size);
    while (res != 0 && k2 < size_path)
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
    if (res != 0)
      warn("%s: Invalid binary", command_name);
    return res;
  }
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
  {
    switch (bi)
    {
    case 0:
      exit(EXIT_SUCCESS);
    case 1:
      return 0;
    case 2:
      return 1;
    default:
      warn("%s: Invalid builtin", get_child_elt(ast, 0));
      return 1;
    // TODO Execute buitins
    }
  }
  else
    return execute_prog(ast, ht);
  // TODO need to implement here the redirections
}
