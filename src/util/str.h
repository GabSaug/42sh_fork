#ifndef STR_H
# define STR_H

# include <unistd.h>

struct str
{
  char* s;
  size_t len;
  size_t capacity;
};

struct str* str_create(void);
void str_append(struct str* str, char* s, ssize_t len_s, int free_s);
void str_destroy(struct str* str, int destroy_s);

#endif /* !STR_H */
