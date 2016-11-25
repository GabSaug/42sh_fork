#define _POSIX_C_SOURCE 200112L
#include <stdlib.h>
#include <stdio.h>
#include "builtins.h"
#include "execute.h"

extern char **environ;

static int change_opt(char *names[], char *argv[], char *set, size_t i)
{
  while (argv[i])
  {
    size_t j = 100;
    for (size_t k = 0; k < 8 && j == 100; k++)
    {
      if (strcmp(names[k], argv[i]) == 0)
        j = k;
    }

    if (j < 100)
    {
      if (set[0] == ' ')
      {
        char *val = get_data(ht[VAR], names[j]);
        if (val)
          printf("%-*s\t%s\n", 15 , names[j], val[0] == '0' ? "off" : "on");
      }
      else if (set[0] != 'q')
        ht[VAR] = add_hash(ht[VAR], names[j], set);
    }
    else
      warnx("shopt: %s: invalid shell option name", argv[i]);
    i++;
  }
  return 0;
}

static int read_opt_shopt(char *argv[], char *names[])
{
  char *set = "    ";
  if (argv[1][0] && argv[1][0] == '-')
  {
    for (size_t l = 1; argv[1][l]; l++)
    {
      if (argv[1][l] == 's' && strcmp(set, "0\0") != 0)
        set = "1\0";
      else if (argv[1][l] == 'u' && strcmp(set, "1\0") != 0)
        set = "0\0";
      else if (argv[1][l] == 's' || argv[1][l] == 'u')
      {
        warnx("shopt: cannot set and unset shell options simultaneously");
        return 1;
      }
      else if (argv[1][l] == 'q' && set[0] == ' ')
        set = "q\0";
      else if (argv[1][l] != 'q')
      {
        warnx("shopt: invalid option");
        return 2;
      }
    }
  }

  return change_opt(names, argv, set, argv[1][0] && argv[1][0] == '-' ? 2 : 1);
}

int builtin_shopt(char *argv[])
{
  char *names[] =
  {
    "ast-print", "dotglob", "expand_aliases", "extglob", "nocaseglob",
    "nullglob", "sourcepath", "xpg_echo"
  };

  if (argv[1] == NULL)
  {
    for (size_t i = 0; i < sizeof (names) / sizeof (char*); i++)
    {
      char *val = get_data(ht[VAR], names[i]);
      if (val != NULL)
        printf("%-*s\t%s\n", 15, names[i], val[0] == '0' ? "off" : "on");
    }
    return 0;
  }
  else
    return read_opt_shopt(argv, names);
}

static void print_alias(struct elt_hash *e)
{
  if (!e)
    return;
  char *tmp = e->data;
  printf("alias %s=%s\n", e->key, tmp);
  print_alias(e->next);
}

int builtin_alias(char *argv[])
{
  if (!argv[1])
  {
    for (size_t i = 0; i < ht[ALIAS]->capacity; i++)
      print_alias(ht[ALIAS]->table[i]);
  }
  else
  {
    for (size_t i = 1; argv[i]; i++)
    {
      char *tmp = argv[i];
      size_t j = 0;
      for (j = 0; tmp[j] && tmp[j] != '='; j++)
        continue;
      if (tmp[j])
      {
        tmp[j] = '\0';
        ht[ALIAS] = add_hash(ht[ALIAS], tmp, tmp + j + 1);
      }
      else
      {
        char *to_disp = get_data(ht[ALIAS], tmp);
        if (to_disp)
          printf("alias %s=%s\n", tmp, to_disp);
        else
        {
          warnx("alias: %s: not found", tmp);
          return 1;
        }
      }
    }
  }

  return 0;
}

int builtin_unalias(char *argv[])
{
  if (!argv[1])
  {
    warnx("unalias: usage: unalias [-a] name [name ...]");
    return 2;
  }
  else
  {
    int res = 0;
    for (size_t i = 1; argv[i]; i++)
    {
      int tmp = del_hash(ht[ALIAS], argv[i]);
      if (!tmp)
      {
        warnx("unalias: %s: notfound", argv[i]);
        res = 1;
      }
    }
    return res;
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
        continue;
      else
        setenv(argv[i], "", 1);
    }
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

int builtin_export(char *argv[])
{
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

int builtin_source(char *argv[])
{
  if (!argv[1])
  {
    warnx("source: filename argument required");
    return 2;
  }

  int fd = open(argv[1], O_RDONLY | O_CLOEXEC);
  if (fd == -1)
  {
    warn("Error to open file");
    return 1;
  }
  struct stat stat_buf;
  if (stat(argv[1], &stat_buf) == -1)
  {
    warn("Impossible to read stat from %s", argv[1]);
    return 1;
  }

  if (S_ISDIR(stat_buf.st_mode))
  {
    warnx("source: %s: is a directory", argv[1]);
    return 1;
  }

  size_t size_file = stat_buf.st_size;
  char* file = mmap(NULL, size_file, PROT_READ, MAP_PRIVATE, fd, 0);
  struct vector *token = NULL;
  int ret = process_input(file, token);
  munmap(file, size_file);
  close(fd);
  return ret;
}
