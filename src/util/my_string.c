#include <string.h>

#include "my_malloc.h"
#include "str.h"
#include "my_string.h"

int is_alpha(char c)
{
  return is_lower_case(c) || is_upper_case(c);
}

int is_name_char(char c)
{
  return c == '_' || is_digit(c) || is_alpha(c);
}

int is_login_char(char c)
{
  return is_alpha(c) || is_digit(c) || c == '.' || c == '_' || c == '-';
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
    return 2 * quoted[SINGLE_QUOTE] - 1; // 1 if set; -1 if unset
  }
  if (c == '"' && !quoted[BACKSLASH] && !quoted[SINGLE_QUOTE])
  {
    quoted[DOUBLE_QUOTE] = !quoted[DOUBLE_QUOTE];
    return 2 * quoted[DOUBLE_QUOTE] - 1; // 1 if set; -1 if unset
  }
  return 0;
}

char* escape_quote(char* input)
{
  struct str* output = str_create();
  for (size_t i = 0; input[i]; ++i)
  {
    if (input[i] == '"' || input[i] == '\'')
      str_append(output, "\\", 1, 0);
    str_append(output, input + i, 1, 0);
  }
  char* res = output->s;
  str_destroy(output, 0);
  return res;
}
