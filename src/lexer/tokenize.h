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
  size_t size;
  char* content_start;
  size_t content_size;
};

struct expansion tokenize_expansion(char* s, int in_ari_exp);
size_t tokenize_comment(char* s, size_t i);
size_t tokenize_exp_normal(char *s);

#endif /* !TOKENIZE_H */
