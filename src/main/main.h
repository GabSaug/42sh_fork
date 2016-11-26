#ifndef MAIN_H
# define MAIN_H

# ifndef O_CLOEXEC
#  define O_CLOEXEC 0
# endif

/**
 * \file main.h
*/

#include "hash_table.h"
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

int process_input(char* buff, struct vector *token);
char* get_PS(void);
/**
 * \brief Free all resource before exiting
*/
void exit_42sh(void);

#endif /* !MAIN_H */
