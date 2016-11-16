static int launch_command(struct tree *ast, struct hash_table *ht)
{
  size_t i = 1;
  struct tree *child = v_get(ast->child, i);
  while (child->nst != COMMAND)
  {
    i++;
    child = v_get(ast->child, i);
  }

  // TODO maybe changes here ?
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
  if (!tree)
    return 1;

  size_t size = v_size(ast->child);
  int res = 0;
  int index = tree->nts == COMMAND ? 0 : 1;

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
  return (v_get(v_get(ast->child, 0)->child, 0))->token->s;
}

static int execute_command(struct tree *ast)
{
  size_t size = v_size(ast->child);

  size_t i = 0;
  while (i < size && (v_get(v_get(ast->child, i), 0))->nts != REDIRECTION)
    i++;
  size = i;

  char *command_name = get_child_elt(ast, 0);

  char *path = NULL;
  if (strlen(command_name) > 2 && strncmp(command_name, "./", 2) == 0)
    path = get_data(ht, "PWD");
  else
    path = get_data(ht, "PATH");

  char *call = malloc(sizeof (char) * (1 + strlen(path)
                      + strlen(command_name)));
  call = strcat(path, command_name);

  char **args = malloc(sizeof (char *) * (1 + size));
  args[0] = call;

  for (size_t j = 1; j < size; j++)
  {
    args[i] = get_child_elt(ast, j);
  }
  args[size] = NULL;

  int pid = fork();
  if (pid == -1)
  {
    warn("%s: Can't fork to execute the command", call);
    return 1;
  }
  else if (pid == 0)
  {
    // Child
    execv(call, args);
    warn("%s: Invalid instruction", call);
    exit(1);
  }
  else
  {
    // Parent
    int exit_status = 0;
    waitpid(pid, &exit_status, 0);
    return WEXITSTATUS(exit_status);
  }
}

int execute_simple_command(struct tree *ast, struct hash_table *ht)
{
  ht = ht;
  // TODO need to verify if there is an assignment word, to create or update a
  // variable
  char *builtin[] =
  {
    "exit", "cd", "shopt", "export", "alias", "unalias", "echo", "continue",
    "break", "source", "history"
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
    default:
      warn("%s: Invalid builtin", get_child_elt(ast, 0));
      return 1;
    // TODO Execute buitins
    }
  }
  else
    return execute_command(ast);
  // TODO need to implement here the redirections
}
