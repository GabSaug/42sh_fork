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

int my_begin_as(char* str, char s_list[][10])
{
  return begin_as(str, str + strlen(str) - 1, s_list);
}

int my_is_in(char* str, char s_list[][10])
{
  return is_in(str, str + strlen(str) - 1, s_list);
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

size_t my_strlen(char* s)
{
  size_t i;
  for (i = 0; s[i]; i++)
    continue;
  return i;
}

int is_digit(char c)
{
  return '0' <= c && c <= '9';
}

int is_number(char* s) // Negative number are NOT recognize as number
{
  for (size_t i = 0; s[i]; ++i)
    if (!is_digit(s[i]))
      return 0;
  return 1;
}

int is_quoted(char quoted[3])
{
  for (int i = 0; i < 3; i++)
    if (quoted[i])
      return 1;
  return 0;
}

int is_lower_case(char c)
{
  return 'a' <= c && c <= 'z';
}

int is_upper_case(char c)
{
  return 'A' <= c && c <= 'Z';
}
