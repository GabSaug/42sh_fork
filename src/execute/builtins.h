#ifndef BUILTINS_H
# define BUILTINS_H

/**
 * \file builtins.h
*/

#include "option_parser.h"

int builtin_alias(char *argv[]);
int builtin_unalias(char *argv[]);
int builtin_export(char *argv[]);

#endif /* !BUILTINS_H */
