#include "execute.h"

static int my_atoi(const char str[])
{
  size_t i = 0;
  int res = 0;

  while (str[i])
  {
    if (str[i] < '0' || str[i] > '9')
      return - 1;
    else
      res = res * 10 + str[i] - '0';
    i++;
  }

  return res;
}

static int open_and_redir(int fd_ionum, const char *file_name, int flags,
                          int order)
{
  int fd = open(file_name, flags);
  if (fd == - 1)
  {
    warn("%s: ", file_name);
    return - 2;
  }

  if (order)
    dup2(fd_ionum, fd);
  else
    dup2(fd, fd_ionum);
  return fd;
}

static int copy_or_close(int fd_ionum, const char *file_name, int order)
{
  if (strcmp(file_name, "-") == 0)
  {
    int test = close(fd_ionum);
    if (test == - 1)
    {
      warn("%d: ", fd_ionum);
      return - 2;
    }
  }
  else
  {
    if (order)
      dup2(fd_ionum, my_atoi(file_name));
    else
      dup2(my_atoi(file_name), fd_ionum);
  }
  return - 1;
}

static int redir_rdwr(int fd_ionum, const char *file_name)
{
  int fd = open(file_name, O_RDWR);
  dup2(fd, fd_ionum);
  return - 1;
}

static int redir_type(int fd_ionum, const char *sign, const char *file_name)
{
  if (strcmp(sign, ">") == 0 || strcmp(sign, ">|") == 0)
    return open_and_redir(fd_ionum == -1 ? 1 : fd_ionum, file_name,
                          O_WRONLY | O_CREAT | O_TRUNC, 0);
  else if (strcmp(sign, "<") == 0)
    return open_and_redir(fd_ionum == -1 ? 0 : fd_ionum, file_name, O_RDONLY,
                          1);
  else if (strcmp(sign, ">>") == 0)
    return open_and_redir(fd_ionum == -1 ? 1 : fd_ionum, file_name,
                          O_WRONLY | O_APPEND, 0);
  else if (strcmp(sign, "<<") == 0)
    return - 1;// TODO
  else if (strcmp(sign, "<<-") == 0)
    return - 1;// TODO
  else if (strcmp(sign, ">&") == 0)
    return copy_or_close(fd_ionum == -1 ? 1 : fd_ionum, file_name, 0);
  else if (strcmp(sign, "<&") == 0)
    return copy_or_close(fd_ionum == -1 ? 0 : fd_ionum, file_name, 1);
  else if (strcmp(sign, "<>") == 0)
    return redir_rdwr(fd_ionum == -1 ? 0 : fd_ionum, file_name);
  else
  {
    warn("%s: Invalid redirection", sign);
    return - 2;
  }
}

static int read_redirection(struct tree *ast)
{
  size_t i = 0;
  int fd_ionum = - 1;
  if (v_size(ast->child) == 3)
  {
    i++;
    struct tree *io_num = v_get(ast->child, 0);
    fd_ionum = my_atoi(io_num->token->s);
  }

  struct tree *sign = v_get(ast->child, i);
  struct tree *word = v_get(ast->child, i + 1);

  return redir_type(fd_ionum, sign->token->s, word->token->s);
}

struct vector *managed_redirections(struct tree *ast)
{
  struct vector *to_close = v_create();

  for (size_t i = 0; i < v_size(ast->child); i++)
  {
    struct tree *son = v_get(ast->child, i);
    son = v_get(son->child, 0);
    if (son->nts == REDIRECTION)
    {
      int *fd = malloc(sizeof (int));
      *fd = read_redirection(son);
      if (*fd == - 2)
      {
        free(fd);
        v_destroy(to_close, free);
        return NULL;
      }
      else if (*fd == - 1)
        free(fd);
      else
        v_append(to_close, fd);
    }
  }

  return to_close;
}
