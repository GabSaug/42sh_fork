#include <err.h>
#include <stdio.h>
#include <string.h>

#include "options.h"

static int read_options(int argc, char *argv[], char options)
{
  if (argc > 1)
  {
    if (!strcmp(argv[1], "-c"))
    {
      if (argc < 3)
        errx(2, "-c: option requires an argument");
      else
        printf("execute a command\n");
        // TODO EXECUTE argv[2]
      return 0;
    }
    else if (!strcmp(argv[1], "--version"))
    {
      printf("Version 0.5\n");
      return 0;
    }
    else if (!strcmp(argv[1], "--norc"))
    {
      options = options | 1;
      return read_options(argc - 1, argv + 1, options);
    }
    else if(!strcmp(argv[1], "--ast-print"))
    {
      options = options | (1 << 1);
      return read_options(argc - 1, argv + 1, options);
    }
    else if (!strcmp(argv[1], "-O") || !strcmp(argv[1], "+O"))
    {
      printf("Set environment variables\n");
      // TODO set environment variables
      return read_options(argc - 2, argv + 2, options);
    }
    else if (argv[1][0] && argv[1][0] == '-')
      errx(2, "%s: invalid option", argv[1]);
    else
    {
      printf("execute a file\n");
      return 0;
      // TODO execute instructions from given file
    }
  }
  else
  {
    printf("launch interpreter\n");
    return 0;
    // TODO interpreter
  }
}

int main(int argc, char *argv[])
{
  return read_options(argc, argv, 0);
}
