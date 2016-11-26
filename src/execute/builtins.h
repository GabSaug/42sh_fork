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

/**
 * \brief Execution of shopt builtin
 * \param argv The arguments for the builtin
 * \return The success or failure of the builtin
*/
int builtin_shopt(char *argv[]);

/**
 * \brief Execution of alias builtin
 * \param argv The arguments for the builtin
 * \return The success or failure of the builtin
*/
int builtin_alias(char *argv[]);

/**
 * \brief Execution of unalias builtin
 * \param argv The arguments for the builtin
 * \return The success or failure of the builtin
*/
int builtin_unalias(char *argv[]);

/**
 * \brief Execution of export builtin
 * \param argv The arguments for the builtin
 * \return The success or failure of the builtin
*/
int builtin_export(char *argv[]);

/**
 * \brief Execution of source builtin
 * \param argv The arguments for the builtin
 * \return The success or failure of the builtin
*/
int builtin_source(char *argv[]);

#endif /* !BUILTINS_H */
