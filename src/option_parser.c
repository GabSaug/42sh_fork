#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "option_parser.h"
#include "hash_table.h"

static void set_o(int o, char *str, struct hash_table *ht)
{
  char *names[] =
  {
    "ast-print", "dotglob", "expand_aliases", "extglob", "nocaseglob",
    "nullglob", "sourcepath", "xpg_echo"
  };

  int b = 0;
  for (int i = 0; i < 8 && !b; i++)
    b = strcmp(names[i], str) == 0;

  if (!b)
    errx(2, "%s: invalid shell option name", str);

  char *val = malloc(2 * sizeof(char));
  val[1] = '\0';
  val[0] = o ? '1' : '0';

  ht = add_hash(ht, str, val);
}

static struct option opt(int argc, char *argv[], struct hash_table *ht,
                         struct option options)
{
  if (argc > 1)
  {
    if (!strcmp(argv[1], "-c"))
    {
      if (argc < 3)
        errx(2, "-c: option requires an argument");
      else
      {
        options.input_mode = COMMAND_LINE;
        return options;
      }
    }
    else if (!strcmp(argv[1], "--version"))
    {
      printf("Version 0.5\n");
      destroy_hash(ht);
      exit(0);
    }
    else if (!strcmp(argv[1], "--norc"))
    {
      options.norc = 1;
      return opt(argc - 1, argv + 1, ht, options);
    }
    else if(!strcmp(argv[1], "--ast-print"))
    {
      ht = add_hash(ht, "ast-print", "1");
      return opt(argc - 1, argv + 1, ht, options);
    }
    else if (!strcmp(argv[1], "-O") || !strcmp(argv[1], "+O"))
    {
      set_o(argv[1][0] == '+', argv[2], ht);
      return opt(argc - 2, argv + 2, ht, options);
    }
    else if (argv[1][0] && argv[1][0] == '-')
      errx(2, "%s: invalid option", argv[1]);
    else
    {
      options.input_mode = INPUT_FILE;
      return options;
    }
  }
  else
  {
    ht = add_hash(ht, "expand_aliases", strdup("1"));
    option.input_mode = INTERACTIVE;
    return options;
  }
}

static void set_env(struct hash_table *ht)
{
  char *var[] =
  {
    "ENV", "HOME", "IFS", "LANG", "LC_ALL", "LC_COLLATE", "LC_CTYPE",
    "LC_MESSAGES", "LINENO", "NLSPATH", "PATH", "PPID", "PS1", "PS2", "PS4",
    "PWD"
  };

  for (int i = 0; i < 16; i++)
  {
    char *temp = strdup(var[i]);
    char *data = getenv(var[i]);
    if (data)
      ht = add_hash(ht, temp, strdup(data));
  }
}

static void init_opt(struct hash_table *ht)
{
  char *opts[] =
  {
    "ast-print", "dotglob", "expand_aliases", "extglob", "nocaseblog",
    "nullglob", "xpg_echo"
  };

  for (int i = 0; i < 7; i++)
  {
    char *temp = strdup(opts[i]);
    char *data = strdup("0");
    if (data && temp)
      ht = add_hash(ht, temp, data);
  }

  char *temp2 = strdup("sourcepath");
  char *data2 = strdup("1");
  if (data2 && temp2)
    ht = add_hash(ht, temp2, data2);
}

struct option parse_options(int argc, char *argv[], struct hash_table *ht)
{
  set_env(ht);
  init_opt(ht);
  struct option options;
  options.norc = 0;
  return opt(argc, argv, ht, options);
}
