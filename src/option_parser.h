#ifndef OPTION_PARSER_H
# define OPTION_PARSER_H

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
};

struct option parse_options(int argc, char *argv[], struct hash_table *ht);

#endif /* !OPTION_PARSER_H */
