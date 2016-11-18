#include <stdio.h>

#include "execute.h"

static int builtin_echo(char* argv[]);

int builtin_execution(struct tree *ast, struct hash_table *ht, int bi)
{
  ht = ht;

  switch (bi)
  {
  case 0:
    exit(EXIT_SUCCESS);
  case 1:
    return 0;
  case 2:
    return 1;
  default:
    warn("%s: Invalid builtin", get_child_elt(ast, 0));
    return 1;
  // TODO Execute buitins
  }
}

static char* builtin_echo_help = "\n\
echo: echo [-neE] [arg ...]\n\
    Write arguments to the standard output.\n\
    \n\
    Display the ARGs, separated by a single space character and followed by a\
    newline, on the standard output.i\n\
    \n\
    Options:\n\
      -n        do not append a newline\n\
      -e        enable interpretation of the following backslash escapes\n\
      -E        explicitly suppress interpretation of backslash escapes\n\
    \n\
    `echo' interprets the following backslash-escaped characters:\n\
      \\a        alert (bell)\n\
      \\b        backspace\n\
      \\c        suppress further output\n\
      \\e        escape character\n\
      \\E        escape character\n\
      \\f        form feed\n\
      \\n        new line\n\
      \\r        carriage return\n\
      \\t        horizontal tab\n\
      \\v        vertical tab\n\
      \\\\        backslash\n\
      \\0nnn     the character whose ASCII code is NNN (octal).  NNN can be\n\
                0 to 3 octal digits\n\
      \\xHH      the eight-bit character whose value is HH (hexadecimal).  HH\n\
                can be one or two hex digits\n\
    \n\
    Exit Status:\n\
    Returns success unless a write error occurs.\n";

static char* builtin_echo_version = "echo version 4.2\n";

static int option_parser(char* argv[], int* i, char* opt_n, char* opt_e)
{
  if (argv[*i] == NULL)
    return -1;
  if (!strcmp(argv[*i], "-n"))
  {
    *opt_n = 1;
    option_parser(argv, i + 1, opt_n, opt_e);
  }
  else if (!strcmp(argv[*i], "-e"))
  {
    *opt_e = 1;
    option_parser(argv, i + 1, opt_n, opt_e);
  }
  else if (!strcmp(argv[*i], "-E"))
  {
    *opt_e = 0;
    option_parser(argv, i + 1, opt_n, opt_e);
  }
  return 0;
}

static void builtin_echo_print(char* s, char opt_n, char opt_e)
{
  opt_e = opt_e;
  printf("%s", s);
  if (!opt_n)
    printf("\n");
}

static int builtin_echo(char* argv[])
{ 
  if (!strcmp(argv[1], "--help"))
  {
    printf("%s", builtin_echo_help);
    return 0;
  }
  if (!strcmp(argv[1], "--version"))
  {
    printf("%s", builtin_echo_version);
    return 0;
  }

  char opt_n = 0; 
  char opt_e = 1;
  int i = 1;
  if (option_parser(argv, &i, &opt_n, &opt_e) == -1)
    return 0;
  if (argv[i])
    builtin_echo_print(argv[i], opt_n, opt_e);
  for (++i; argv[i]; ++i)
    builtin_echo_print(argv[i], opt_n, opt_e);
 return 0; 
}
