enum token_id
{
  EOF,
  NEWLINE,
  SEMI, // ;
  AND, // &
  PIPE,
  AND_IF,
  OR_IF,
  DSEMI,
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
};

struct token
{
  enum token_id id;
  char* s;
};

