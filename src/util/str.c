#include <string.h>
#include <stdlib.h>

#include "my_malloc.h"
#include "str.h"

struct str* str_create(void)
{
  struct str* new_str = my_malloc(sizeof (struct str));
  new_str->s = malloc(sizeof (char));
  new_str->s[0] = '\0';
  new_str->len = 0;
  new_str->capacity = 1;
  return new_str;
}

void str_append(struct str* str, char* s, ssize_t len_s, int free_s)
{
  if (!str || !s)
    return;
  if (len_s == -1)
    len_s = strlen(s);
  if (str->capacity < str->len + len_s + 1)
  {
    while (str->capacity < str->len + len_s + 1)
      str->capacity *= 2;
    str->s = realloc(str->s, sizeof (char) * str->capacity);
  }
  memcpy(str->s + str->len, s, len_s);
  if (free_s)
    free(s);
  str->len += len_s;
  str->s[str->len] = '\0';
}

void str_destroy(struct str* str, int destroy_s)
{
  if (destroy_s)
    free(str->s);
  free(str);
}
