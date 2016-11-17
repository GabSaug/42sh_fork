#ifndef OPTION_PARSER_H
# define OPTION_PARSER_H

#include <string.h>

#include <err.h>
#include <stdio.h>
#include <stdlib.h>


#include "hash_table.h"

enum input_mode
{
  INTERACTIVE,
  COMMAND_LINE,
  INPUT_FILE
};

struct option
{
  int norc;
  enum input_mode input_mode;
  char* input;
};

struct option opt(int argc, char *argv[], struct hash_table *ht,
                  struct option options);
struct option parse_options(int argc, char *argv[], struct hash_table *ht);

#endif /* !OPTION_PARSER_H */
