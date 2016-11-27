#define _POSIX_C_SOURCE 200112L
#include <stdlib.h>
#include "builtins.h"

extern char **environ;

static void change_setenv(char *argv[], size_t i)
{
  size_t j = 0;
  for (j = 0; argv[i][j] && argv[i][j] != '='; j++)
    continue;
  if (argv[i][j])
  {
    argv[i][j] = '\0';
    setenv(argv[i], argv[i] + j + 1, 1);
  }
  else
  {
    char *env = getenv(argv[i]);
    if (env)
      return;
    else
      setenv(argv[i], "", 1);
  }
}

static int set_environ(char *argv[], size_t i)
{
  int res = 0;
  for (i = i; argv[i]; i++)
  {
    if (argv[i][0] == '-')
    {
      warnx("export: `%s': not a valid identifier", argv[i]);
      res = 1;
      continue;
    }

    change_setenv(argv, i);
  }
  return res;
}

static int unset_environ(char *argv[], size_t i)
{
  int res = 0;
  for (i = i; argv[i]; i++)
  {
    if (argv[i][0] == '-')
    {
      warnx("export: `%s': not a valid identifier", argv[i]);
      res = 1;
    }
    else
      unsetenv(argv[i]);
  }
  return res;
}

int builtin_export(char *argv[], struct hash_table *ht[])
{
  ht = ht;
  if (!argv[1] || (strcmp(argv[1], "-p") == 0 && !argv[2]))
  {
    for (size_t i = 0; environ[i]; i++)
      printf("declare - x %s\n", environ[i]);
    return 0;
  }
  else
  {
    size_t i = 1;
    int set = 1;
    if (strcmp(argv[1], "-n") == 0)
    {
      i++;
      set = 0;
    }
    // TODO read options of the builtin
    if (set)
      return set_environ(argv, i);
    else
      return unset_environ(argv, i);
  }
}

static int source_error(char *argv[], int *fd, struct stat *stat_buf)
{
  if (!argv[1])
  {
    warnx("source: filename argument required");
    return 2;
  }

  *fd = open(argv[1], O_RDONLY | O_CLOEXEC);
  if (*fd == -1)
  {
    warn("Error to open file");
    return 1;
  }
  if (stat(argv[1], stat_buf) == -1)
  {
    warn("Impossible to read stat from %s", argv[1]);
    return 1;
  }

  if (S_ISDIR(stat_buf->st_mode))
  {
    warnx("source: %s: is a directory", argv[1]);
    return 1;
  }

  return 0;
}

int builtin_source(char *argv[], struct hash_table *ht[3])
{
  int fd = 0;
  struct stat stat_buf;
  int res = source_error(argv, &fd, &stat_buf);

  if (res != 0)
    return res;

  size_t size_file = stat_buf.st_size;
  char* file = mmap(NULL, size_file, PROT_READ, MAP_PRIVATE, fd, 0);

  struct shell_tools tools;
  for (size_t i = 0; i < 3; i++)
    tools.ht[i] = ht[i];

  tools.option.input = file;
  tools.v_token = NULL;
  res = process_input(&tools);
  munmap(file, size_file);
  close(fd);
  return res;
}
