#include <err.h>
#include <stdio.h>
#include <string.h>

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
      options = options | 1;
      return opt(argc - 1, argv + 1, ht, options);
    }
    else if(!strcmp(argv[1], "--ast-print"))
    {
      options = options | (1 << 1);
      return opt(argc - 1, argv + 1, ht, options);
    }
    else if (!strcmp(argv[1], "-O") || !strcmp(argv[1], "+O"))
    {
      printf("Set environment variables\n");
      // TODO set environment variables
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
    option.input_mode = INTERACTIVE;
    return options;
  }
}

struct option parse_options(int argc, char *argv[], struct hash_table *ht)
{
  // TODO initialize env var
  struct option options;
  return opt(argc, argv, ht, options);
}
