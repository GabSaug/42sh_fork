#ifndef BUILTINS_H
# define BUILTINS_H

/**
 * \file builtins.h
*/

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "main.h"
#include "option_parser.h"

int builtin_shopt(char *argv[]);
int builtin_alias(char *argv[]);
int builtin_unalias(char *argv[]);
int builtin_export(char *argv[]);
int builtin_source(char *argv[]);

#endif /* !BUILTINS_H */
