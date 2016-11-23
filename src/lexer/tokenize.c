#include <err.h>
#include <string.h>

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

static char exp_intro[][10] =
{
  "$((",
  "$(",
  "${",
  "`",
  ""
};

static char exp_begin[] =
{
  '(',
  '(',
  '{',
  '`'
};

static char exp_end[] =
{
  ')', ')', '}', '`'
};


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

  int index_exp_type = my_begin_as(s, exp_intro);
  if (index_exp_type != -1)
  {
    exp.start = s + strlen(exp_intro[index_exp_type]);
    size_t other_size = tokenize_exp_other(s
                                           + strlen(exp_intro[index_exp_type]),
                    exp_begin[index_exp_type], exp_end[index_exp_type]);
    exp.end = exp.start + other_size;
    exp.size = 2 * (exp.start - s) + other_size;
    exp.type = index_exp_type > CMD ? CMD : index_exp_type;
    /*if (exp.type != ARI)
      exp.size++;*/
    return exp;
  }
  exp.type = NORMAL;
  size_t normal_size = tokenize_exp_normal(s + 1);
  exp.size = normal_size + 1;
  //printf("exp.size = %zu\n", exp.size);
  exp.start = s + 1;
  //exp.end = exp.start + 2;
  exp.end = exp.start + normal_size;
  return exp;
}

