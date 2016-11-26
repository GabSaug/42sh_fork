#include "option_parser.h"

extern struct hash_table* ht[2];

static struct option opt(int argc, char *argv[], struct option options);

static void set_o(int o, char *str)
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

  char *val = malloc(2 * sizeof (char));
  val[1] = '\0';
  val[0] = o ? '1' : '0';

  add_hash(ht[VAR], str, val);
}

static struct option opt2(int argc, char *argv[], struct option options)
{
  if (!strcmp(argv[1], "--norc"))
  {
    options.norc = 1;
    return opt(argc - 1, argv + 1, options);
  }
  else if (!strcmp(argv[1], "--ast-print"))
  {
    add_hash(ht[VAR], "ast-print", "1");
    return opt(argc - 1, argv + 1, options);
  }
  else if (!strcmp(argv[1], "-O") || !strcmp(argv[1], "+O"))
  {
    set_o(argv[1][0] == '+', argv[2]);
    return opt(argc - 2, argv + 2, options);
  }
  else if (argv[1][0] && argv[1][0] == '-')
    errx(2, "%s: invalid option", argv[1]);
  else
  {
    options.input = argv[1];
    options.input_mode = INPUT_FILE;
    return options;
  }
}

static struct option opt(int argc, char *argv[], struct option options)
{
  if (argc > 1)
  {
    if (!strcmp(argv[1], "-c"))
    {
      if (argc < 3)
        errx(2, "-c: option requires an argument");
      options.input = argv[2];
      options.input_mode = COMMAND_LINE;
      return options;
    }
    else if (!strcmp(argv[1], "--version") || !strcmp(argv[1], "--ver"))
    {
      printf("Version 1.0\n");
      exit(0);
    }
    else
      return opt2(argc, argv, options);
  }

  add_hash(ht[VAR], "expand_aliases", "1");
  options.input_mode = INTERACTIVE;
  return options;
}

static void set_env(void)
{
  char *var[] =
  {
    "ENV", "HOME", "IFS", "LANG", "LC_ALL", "LC_COLLATE", "LC_CTYPE",
    "LC_MESSAGES", "LINENO", "NLSPATH", "PATH", "PPID", "PWD"
  };

  for (size_t i = 0; i < sizeof (var) / sizeof (char *); i++)
  {
    char *data = getenv(var[i]);
    if (data)
      add_hash(ht[VAR], var[i], data);
    //else
      //printf("var %s not found\n", var[i]);
  }

  add_hash(ht[VAR], "PS1", "42sh$ ");
  add_hash(ht[VAR], "PS2", "> ");
  add_hash(ht[VAR], "PS4", "+ ");
}

static void init_opt(void)
{
  char *opts[] =
  {
    "ast-print", "dotglob", "expand_aliases", "extglob", "nocaseblog",
    "nullglob", "xpg_echo"
  };

  for (int i = 0; i < 7; i++)
    add_hash(ht[VAR], opts[i], "0");

  add_hash(ht[VAR], "sourcepath", "1");
}

struct option parse_options(int argc, char *argv[])
{
  set_env();
  init_opt();
  struct option options;
  options.norc = 0;
  options.input = NULL;
  return opt(argc, argv, options);
}
