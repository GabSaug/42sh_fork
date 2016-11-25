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

int is_lower_case(char c)
{
  return 'a' <= c && c <= 'z';
}

int is_upper_case(char c)
{
  return 'A' <= c && c <= 'Z';
}

int is_alpha(char c)
{
  return is_lower_case(c) || is_upper_case(c);
}

int is_name_char(char c)
{
  return c == '_' || is_digit(c) || is_alpha(c);
}

int my_atoi(const char str[])
{
  int res = 0;
  for (size_t i = 0; str[i]; ++i)
  {
    if (!is_digit(str[i]))
      return - 1;
    else
      res = res * 10 + str[i] - '0';
  }

  return res;
}

char* my_strdup(char* s)
{
  if (!s)
    return NULL;
  size_t len = strlen(s) + 1;
  char* new_s = my_malloc(sizeof (char) * len);
  for (size_t i = 0; i < len; ++i)
    new_s[i] = s[i];
  return new_s;
}

size_t min(size_t a, size_t b)
{
  return a < b ? a : b;
}

char* my_strndup(char* s, size_t n)
{
  if (!s)
    return NULL;
  size_t len = min(strlen(s), n);
  char* new_s = my_malloc(sizeof (char) * (len + 1));
  for (size_t i = 0; i < len; ++i)
    new_s[i] = s[i];
  new_s[len] = '\0';
  return new_s;
}

int is_prefix_arr(char* str, char s_list[][10])
{
  for (int i = 0; s_list[i][0]; ++i)
  {
    size_t len = strlen(s_list[i]);
    if (!strncmp(str, s_list[i], len) && strlen(s_list[i]) == len)
      return i;
  }
  return -1;
}

int update_quote(char c, char quoted[])
{
  if (c == '\\' && !quoted[BACKSLASH] && !quoted[SINGLE_QUOTE])
  {
    quoted[BACKSLASH] = 2; // Decrease at each loop iteration
    return 1;
  }
  if (c == '\'' && !quoted[BACKSLASH] && !quoted[DOUBLE_QUOTE])
  {
    quoted[SINGLE_QUOTE] = !quoted[SINGLE_QUOTE];
    return 1;
  }
  if (c == '"' && !quoted[BACKSLASH] && !quoted[SINGLE_QUOTE])
  {
    quoted[DOUBLE_QUOTE] = !quoted[DOUBLE_QUOTE];
    return 1;
  }
  return 0;
}

int is_quoted(char quoted[3])
{
  for (int i = 0; i < 3; i++)
    if (quoted[i])
      return 1;
  return 0;
}


