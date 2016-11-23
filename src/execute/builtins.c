#include <stdio.h>

#include "execute.h"
#include "builtins.h"

extern struct hash_table* ht[3];

static int builtin_exit(char* argv[]);
static int builtin_true(char* argv[]);
static int builtin_false(char* argv[]);
static int builtin_cd(char* argv[]);
static int builtin_echo(char* argv[]);
static int builtin_shopt(char *argv[]);

static struct builtin_fun builtin_fun_array[] =
{
  { "exit", builtin_exit },
  { "true", builtin_true },
  { "false", builtin_false },
  { "cd", builtin_cd },
  { "echo", builtin_echo },
  { "shopt", builtin_shopt },
  { "alias", builtin_alias },
  { "unalias", builtin_unalias }
};
/*{ "export", builtin_export },
  { "continue", builtin_echo },
  { "break", builtin_break },
  { "source", builtin_source },
  { "history", builtin_history }
};*/

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
        printf("%s\t%s\n", names[j], val[0] == '0' ? "off" : "on");
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

static int builtin_shopt(char *argv[])
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
        printf("%s\t%s\n", names[i], val[0] == '0' ? "off" : "on");
    }
    return 0;
  }
  else
    return read_opt_shopt(argv, names);
}

int (*builtin_fun_match (char* s)) (char* argv[])
{
  size_t len_array = sizeof (builtin_fun_array) / sizeof (*builtin_fun_array);

  for (size_t i = 0; i < len_array; ++i)
  {
    if (!strcmp(builtin_fun_array[i].name, s))
      return builtin_fun_array[i].fun;
  }
  return NULL;
}

static int builtin_exit(char* argv[])
{
  char* ret_string = NULL;
  if (argv[1])
    ret_string = argv[1];
  if (ret_string == NULL)
    ret_string = get_data(ht[VAR], "$?");
  int ret = 0; 
  if (ret_string)
    ret = atoi(ret_string);
  exit(ret);
}

static int builtin_true(char* argv[])
{
  argv = argv;
  return 0;
}

static int builtin_false(char* argv[])
{
  argv = argv;
  return 1;
}

static int chdir_cd(char* argv[], char* directory)
{
  if (argv[1] == NULL)
  {
    char* home = get_data(ht[VAR], "HOME");
    if (home == NULL || home[0] == '\0')
    {
      printf("No HOME defined\n");
      return 0;
    }
    directory = home;
  }
  if (chdir(directory) == 0)
  {
    add_hash(ht[VAR], "OLDPWD", get_data(ht[VAR], "PWD"));
    add_hash(ht[VAR], "PWD", getcwd(NULL, 0));
    return 0;
  }
  return 1;
}

static int builtin_cd(char* argv[])
{
  if (argv[1] && !strcmp(argv[1], "-"))
  {
    char* old_pwd = get_data(ht[VAR], "OLDPWD");
    if (old_pwd)
      chdir(old_pwd);
    else
    {
      warnx("OLDPWD not set");
      return 1;
    }
    add_hash(ht[VAR], "OLDPWD", get_data(ht[VAR], "PWD"));
    add_hash(ht[VAR], "PWD", getcwd(NULL, 0));
    return 0;
  }
  char* directory = argv[1];
  //Rule 1 and 2
  if (chdir_cd(argv, directory) == 0)
    return 0;
  warn("cd: %s", argv[1]);
  return 1;
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
    *i += 1;
    return option_parser(argv, i, opt_n, opt_e);
  }
  else if (!strcmp(argv[*i], "-e"))
  {
    *opt_e = 1;
    *i += 1;
    return option_parser(argv, i, opt_n, opt_e);
  }
  else if (!strcmp(argv[*i], "-E"))
  {
    *opt_e = 0;
    *i += 1;
    return option_parser(argv, i, opt_n, opt_e);
  }
  return 0;
}

static int builtin_echo_print_2(char c2)
{
  if (!c2 || c2 == '\\')
    printf("\\");
  else if (c2 == 'n')
    printf("\n");
  else if (c2 == 'e')
    printf("%c", 0x1b);
  else if (c2 == 't')
    printf("\t");
  else if (c2 == 'a')
    printf("\a");
  else if (c2 == 'c')
    return 0;
  else if (c2 == 'f')
    printf("\f");
  else if (c2 == 'r')
    printf("\r");
  else if (c2 == 't')
    printf("\t");
  else if (c2 == 'v')
    printf("\v");
  else
    printf("\\%i", c2);
  return 1;
}

static int builtin_echo_print(char* s, char opt_e)
{
  for (size_t i = 0; s[i]; i++)
  {
    char c = s[i];
    if (c == '\\' && opt_e)
    {
      i++;
      if (builtin_echo_print_2(s[i]) == 0)
        return 0;
    }
    else
      printf("%c", c);
  }
  return 1;
}

static int builtin_echo(char* argv[])
{
  char opt_n = 0; 
  char opt_e = 0;
  if (argv[1])
  {
    if (!strcmp(argv[1], "--help"))  //Handle this extension the same as the others
    {
      printf("%s", builtin_echo_help);
      return 0;
    }
    if (!strcmp(argv[1], "--version"))
    {
      printf("%s", builtin_echo_version);
      return 0;
    }

    int i = 1;
    if (option_parser(argv, &i, &opt_n, &opt_e) == -1)
      return 0;
    for (; argv[i] && builtin_echo_print(argv[i], opt_e) && argv[i + 1]; ++i)
      printf(" ");
  }
  if (!opt_n)
    printf("\n");
  fflush(stdout);
 return 0; 
}

/*static void builtin_alias_print_all(struct hash_table* ht_alias)
{
  hash_dump(ht_alias);
}

static int builtin_alias(char* argv[])
{
  if (argv[1] == NULL)
  {
    builtin_alias_print_all(ht_alias);
    return 0;
  }
  size_t i = 1;
  for (i = 1; argv[i]; ++i)
  {
    char* ptr_equal = strchr(argv[1], '=');
    if (ptr_equal) // Assignment
    {
      argv[1][ptr_equal] = '\0';
      add_hash(ht, argv[1], arg[1] + ptr_equal + 1);
    }
    else // print
    {
      char* alias_value = get_data(ht_alias, agrv[1]);
      if (alias_value == NULL)
      {
        warn("not found");
        return 0;
      }
      printf("alias %s=\'%s\'\n", argv[i], alias_value);
    }
  }
  return 0;
}*/
