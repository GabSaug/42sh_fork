#ifndef TOKENIZE_H
# define TOKENIZE_H

# include <stdlib.h>
# include <err.h>

enum expansion_type
{
  ARI,
  BRACKET,
  CMD,
  NORMAL,
  NO_EXPANSION
};

struct expansion 
{
  enum expansion_type type;
  char* start;
  char* end;
  size_t size;
};

struct expansion tokenize_expansion(char* s);
size_t tokenize_comment(char* s, size_t i);
size_t tokenize_exp_normal(char *s);

#endif /* !TOKENIZE_H */
