#ifndef LEXER_H
# define LEXER_H

#include "vector.h"

enum terminal_symbol
{
  UNDIFINED = -1,
  EOF_SYM,
  NEWLINE,
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
  L_BRACE,
  R_BRACE,
  L_PAR, // (
  R_PAR, // )
  BANG, // !
  IN,
  FUNCTION,
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

enum quote_type
{
  BACKSLASH,
  SINGLE_QUOTE,
  DOUBLE_QUOTE
};

void lexer(char* s, struct vector* v_token);

#endif /* !LEXER_H */
