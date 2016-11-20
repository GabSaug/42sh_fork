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

int my_is_in(char* str, char s_list[][10])
{
  return is_in(str, str + strlen(str), s_list);
}

int my_strcmp(char* str1, char* str2)
{
  size_t i;
  for (i = 0; str1[i] && str2[i]; i++)
  {
    if (str1[i] != str2[i])
      return 0;
  }
  return str1[i] == str2[i];
}

char* my_strdup(char* s)
{
  size_t len = strlen(s) + 1;
  char* new_s = my_malloc(sizeof (char) * len);
  for (size_t i = 0; i < len; ++i)
    new_s[i] = s[i];
  return new_s;
}

int my_strnum(char* s)
{
  for (size_t i = 0; s[i]; i++)
  {
    if (s[i] < '0' || s[i] > '9')
      return 1;
  }
  return 1;
}

int is_prefix_arr(char* str, char s_list[][10])
{
  for (int i = 0; s_list[i][0]; ++i)
  {
    size_t len = strlen(list[i];
    if (!strncmp(start, s_list[i], len) && strlen(s_list[i]) == len)
      return i;
  }
  return -1;
}
