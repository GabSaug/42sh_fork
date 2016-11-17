#include <string.h>

#include "my_malloc.h"
#include "my_string.h"

int begin_as(char* start, char* end, char s_list[][10])
{
  size_t len = end - start + 1;
  for (int i = 0; s_list[i][0]; ++i)
    if (!strncmp(start, s_list[i], len) && strlen(s_list[i]) >= len)
      return i;
  return -1;
}

int is_in(char* start, char* end, char s_list[][10])
{
  size_t len = end - start + 1;
  for (int i = 0; s_list[i][0]; ++i)
    if (!strncmp(start, s_list[i], len) && strlen(s_list[i]) == len)
      return i;
  return -1;

}

char* my_strdup(char* s)
{
  size_t len = strlen(s) + 1;
  char* new_s = my_malloc(sizeof (char) * len);
  for (size_t i = 0; i < len; ++i)
    new_s[i] = s[i];
  return new_s;
}
