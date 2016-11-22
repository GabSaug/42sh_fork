#include "tokenize.h"
#include "my_string.h"

// Return the number of character in the comment
size_t tokenize_comment(char* s, size_t i)
{
  size_t j;
  for (j = 0; s[i + j] != '\0' && s[i + j] != '\n'; ++j)
    continue;
  return j - 1;
}

static char operator_list[][10] =
{
  ";", "&", "|", "&&", "||", ";;", "<", ">", "<<", ">>", "<&", ">&",
  "<>", "<<-", ">|", ""
};
// Return the number of character in the expansion
size_t tokenize_exp_normal(char *s)
{
  size_t i;
  if (is_digit(s[1]))
    return 2;
  // for (i = 2 -> wtf ?
  for (i = 1; s[i] && s[i] != ' ' && s[i] != '\t' && s[i] != '\n'
       && s[i] != '$' && is_prefix_arr(s + i, operator_list) == -1; ++i) // To modify
  {
    if (s[i] == '\\' && s[i + 1])
      i++;
    else if (s[i] == '\'')
      for (i++; s[i] && s[i] != '\'' && s[i - 1] != '\\'; ++i)
        continue;
    else if (s[i] == '\"')
      for (i++; s[i] && s[i] != '\"' && s[i - 1] != '\\'; ++i)
        continue;
  }
  return i;
}

static size_t tokenize_exp_other(char *s, char b, char d)
{
  int count = 0;
  size_t i = 2;
  while (s[i] && (s[i] != d || count > 0))
  {
    if (s[i] == b)
      count++;
    else if (s[i] == d)
      count--;

    if (s[i] == '\\')
      i++;
    else if (s[i] == '\'' || s[i] == '\"')
    {
      char c = s[i];
      for (i++; s[i] && s[i] != c; ++i)
        continue;
    }

    i++;
  }

  if (!s[i])
    warn("Unexpected EOF, expected '%c;", d);
  return s[i] ? i : 0;
}

// Return the number of character in the expansion
size_t tokenize_expansion(char* s)
{
  char b = '{';
  char d = '{';
  if (s[0] == '`')
  {
    d = '`';
    b = '`';
  }
  else
  {
    b = s[1];
    switch (s[1])
    {
    case '{':
      d = '}';
      break;
    case '(':
      d = ')';
      break;
    default:
      return tokenize_exp_normal(s);
    }
  }
  return tokenize_exp_other(s, b, d);
}

