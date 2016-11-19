#ifndef OPTION_PARSER_H
# define OPTION_PARSER_H

#include <string.h>

#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "hash_table.h"

/**
 * \file option_parser.h
*/

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

/**
 * \brief Parse the options given as arguments
 * \param argc The number of parameters given
 * \param argv The arguments given
 * \return A structure option
*/
struct option parse_options(int argc, char *argv[]);

#endif /* !OPTION_PARSER_H */
