#include <err.h>

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
  if (is_digit(s[0]))
    return 1;
  // for (i = 2 -> wtf ?
  for (i = 0; s[i] && s[i] != ' ' && s[i] != '\t' && s[i] != '\n'
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
      for (i++; s[i] && s[i] != c; ++i) // add back_slash escape
        continue;
    }

    i++;
  }

  if (!s[i])
    warn("Unexpected EOF, expected '%c;", d);
  return s[i] ? i : 0;
}

// Return the number of character in the expansion
struct expansion tokenize_expansion(char* s)
{
  struct expansion exp =
  {
    BRACKET,
    NULL,
    NULL,
    0,
  };
  char b = 0;
  char d = 0;

  if (s[0] == '`')
  {
    d = '`';
    b = '`';
    exp.type = CMD;
    exp.start = s + 1;
  }
  else if (s[0] == '$')
  {
    if (s[1] == '{')
    {
      b = '{';
      d = '}';
      exp.type = BRACKET;
      exp.start = s + 2;
    }
    else if (s[1] == '(')
    {
      b = '(';
      d = ')';
      if (s[2] == '(')
      {
        exp.type = ARI;
        exp.start = s + 3;
      }
      else
      {
        exp.type = CMD;
        exp.start = s + 2;
      }
    }
    else
    {
      exp.type = NORMAL;
      size_t normal_size = tokenize_exp_normal(s + 1);
      exp.size = normal_size + 1;
      //printf("exp.size = %zu\n", exp.size);
      exp.start = s + 1;
      //exp.end = exp.start + 2;
      exp.end = exp.start + normal_size;
      return exp;
    }
  }
  else
    errx(2, "not an expansion\n");

  size_t other_size = tokenize_exp_other(s, b, d);
  exp.end = exp.start + other_size;
  exp.size = 2 * (exp.start - s) + other_size;
  if (exp.type != ARI)
    exp.size++;
  return exp;


  /*char b = '{';
  char d = '{';
  if (s[0] == '`')
  {
    d = '`';
    b = '`';
    exp.type = CMD;
  }
  else
  {
    b = s[1];
    switch (s[1])
    {
    case '{':
      d = '}';
      exp.type = BRACKET;
      break;
    case '(':
      d = ')';
      if (s[2] == ')')
        exp.type = ARI;
      break;
    default:
      exp.type = NORMAL;
      return tokenize_exp_normal(s);
    }
  }
  return tokenize_exp_other(s, b, d);*/
}

