#include <stdio.h>
#define _POSIX_C_SOURCE 200112L
#include <stdlib.h>
#include "execute.h"

extern char **environ;

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

int builtin_export(char *argv[])
{
  if (!argv[1])
  {
    for (size_t i = 0; environ[i]; i++)
      printf("declare - x %s\n", environ[i]);
    return 0;
  }
  else
  {
    size_t i = 1;
    // TODO read options of the builtin
    for (i = i; argv[i]; i++)
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
          continue;
        else
          setenv(argv[i], "", 1);
      }
    }
    return 0;
  }
}
