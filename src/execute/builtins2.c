#include <stdio.h>
#include "execute.h"

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
