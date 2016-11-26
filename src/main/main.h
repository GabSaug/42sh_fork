#ifndef MAIN_H
# define MAIN_H

# ifndef O_CLOEXEC
#  define O_CLOEXEC 0
# endif

/**
 * \file main.h
*/

#include "hash_table.h"
#include "option_parser.h"
#include "vector.h"

struct shell_tools
{
  struct hash_table *ht[3];
  struct option option;
};

/**
 *  \brief The main function
 *  \param argc number of args
 *  \param argv list of args
*/
int main(int argc, char* argv[]);

int process_input(char* buff, struct vector *token, struct shell_tools tools);
char* get_PS(struct hash_table *ht[]);
/**
 * \brief Free all resource before exiting
*/
void exit_42sh();

#endif /* !MAIN_H */
