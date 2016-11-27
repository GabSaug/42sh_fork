#include <stdio.h>
#include "builtins.h"
#include "execute.h"

static int change_opt(char *names[], char *argv[], char *set, size_t i,
                      struct hash_table *ht[])
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

static int read_opt_shopt(char *argv[], char *names[], struct hash_table *ht[])
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

  return change_opt(names, argv, set, argv[1][0] && argv[1][0] == '-' ? 2 : 1,
                    ht);
}

int builtin_shopt(char *argv[], struct hash_table *ht[])
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
    return read_opt_shopt(argv, names, ht);
}

static void print_alias(struct elt_hash *e)
{
  if (!e)
    return;
  char *tmp = e->data;
  printf("alias %s='%s'\n", e->key, tmp);
  print_alias(e->next);
}

static int print_one_alias(char *tmp, size_t j, struct hash_table *ht[])
{
  if (tmp[j])
  {
    tmp[j] = '\0';
    ht[ALIAS] = add_hash(ht[ALIAS], tmp, tmp + j + 1);
  }
  else
  {
    char *to_disp = get_data(ht[ALIAS], tmp);
    if (to_disp)
      printf("alias %s='%s'\n", tmp, to_disp);
    else
    {
      warnx("alias: %s: not found", tmp);
      return 1;
    }
  }

  return 0;
}

int builtin_alias(char *argv[], struct hash_table *ht[])
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
      if (print_one_alias(tmp, j, ht) == 1)
        return 1;
    }
  }

  return 0;
}

int builtin_unalias(char *argv[], struct hash_table *ht[])
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
