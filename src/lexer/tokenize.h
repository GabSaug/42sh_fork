#ifndef TOKENIZE_H
# define TOKENIZE_H

# include <stdlib.h>
# include <err.h>

size_t tokenize_comment(char* s, size_t i);
size_t tokenize_expansion(char* s);
size_t tokenize_exp_normal(char *s);

#endif /* !TOKENIZE_H */
