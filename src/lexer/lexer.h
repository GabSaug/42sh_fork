#ifndef LEXER_H
# define LEXER_H

/**
 * \file lexer.h
*/

#include <err.h>
#include "vector.h"

enum terminal_symbol
{
  UNDIFINED = -1,
  SEMI, // ;
  AND, // &
  PIPE,
  AND_IF,
  OR_IF,
  DSEMI,
  LESS,
  GREAT,
  DLESS,
  DGREAT,
  LESSAND,
  GREATAND,
  LESSGREAT,
  DLESSDASH,
  CLOBBER,
  L_BRACE,
  R_BRACE,
  L_PAR, // (
  R_PAR, // )
  BANG, // !
  EOF_SYM,
  IF,
  THEN,
  ELSE,
  ELIF,
  FI,
  DO,
  DONE,
  CASE,
  ESAC,
  WHILE,
  UNTIL,
  FOR,
  IN,
  FUNCTION,
  NL, // \n
  WORD,
  ASSIGNMENT_WORD,
  NAME,
  IO_NUMBER
};

struct token
{
  enum terminal_symbol id;
  char* s;
};

/**
 * \brief Fill a vector of token based on the input string
 * \param s The string to break into token
 * \param v_token A empty vector of token
 * \return 0 if an error occured, or 1 if the string was lexed successfully
*/
int lexer(char* s, struct vector* v_token);

/**
 * \brief Free the token given as argument
 * \param p The address of the token to free
*/
void token_destroy(void* p);

#endif /* !LEXER_H */
