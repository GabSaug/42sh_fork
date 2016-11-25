#include <err.h>
#include <string.h>
#include <stdio.h>

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

// Return the number of character in the expansion
size_t tokenize_exp_normal(char *s)
{
  size_t i;
  if (!is_name_char(s[0]))
      return 0;
  if (is_digit(s[0]))
    return 1;
  // for (i = 2 -> wtf ?
  for (i = 1; s[i] && is_name_char(s[i]); ++i) // To modify
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

// Modify so that $(( abc ) def ) triggers an error
static size_t tokenize_exp_other(char *s, char b, char d, char** start,
                                 size_t *content_size, int nb_char_intro)
{
  int count = 0;
  size_t i = 0;
  //int nb_char_intro = 1;
  //int in_intro = 1;
  //printf("char = %c\n", d);
  while (s[i] != b)
    ++i;
  for (int j = 0; j < nb_char_intro; ++i, ++j)
    count++;
  *start = s + i;
  for (; s[i] && count > 0; ++i)
  {
    //printf("%c, count = %i\n", s[i], count);
    /*if (in_intro)
    {
      if (s[i] != b)
      {
        in_intro = 0;
        *start = s + i;
      }
      else
        nb_char_intro++;
    }*/

    if (s[i] == b)
      count++;
    else if (s[i] == d)
    {
      count--;
      if (count == 0)
        break;
    }

    if (s[i] == '\\')
      i++;
    else if (s[i] == '\'' || s[i] == '\"')
    {
      char c = s[i];
      for (i++; s[i] && s[i] != c; ++i) // add back_slash escape
        continue;
    }
  }
  for (int j = 0; j < nb_char_intro; ++j)
  {
    if (s[i - j] !=  d)
    {
      warn("End pattern not found");
      return 0;
    }
  }

  *content_size = (s+i) - *start - nb_char_intro + 1;

  if (!s[i])
    warn("Unexpected EOF, expected '%c;", d);
  return s[i] ? i : 0;
}

static char exp_intro[][10] =
{
  "$((",
  "${",
  "$(",
  "`",
  "$",
  "'",
  "\"",
};

static char exp_begin[] =
{
  '(', // ARI
  '{', // BRACKET
  '(', // CMD
  '`', // CMD2
  0,
  '\'', // SINGLE_QUOTE
  '"'  // DOUBLE_QUOTE
};

static char exp_end[] =
{
  ')',
  '}',
  ')',
  '`',
  0,
  '\'',
  '"'
};


// Return the number of character in the expansion
struct expansion tokenize_expansion(char* s, int in_ari_exp)
{
  struct expansion exp =
  {
    NO_EXPANSION,
    0,
    NULL,
    0,
  };

  exp.type = is_prefix_arr(s, exp_intro);
  if (exp.type != -1)
  {
    //exp.type = index_exp_type > CMD ? index_exp_type - 1 : index_exp_type;
    if (exp.type == NORMAL)
    {
      exp.content_size = tokenize_exp_normal(s + 1);
      exp.size = exp.content_size + 1;
      //printf("exp.size = %zu\n", exp.size);
      exp.content_start = s + 1;
      //exp.end = exp.start + 2;
      //exp.end = exp.start + normal_size;
    }
    else
    {
      //exp.start = s + strlen(exp_intro[index_exp_type]);
      size_t other_size = tokenize_exp_other(s, exp_begin[exp.type],
          exp_end[exp.type], &exp.content_start, &exp.content_size, (exp.type == ARI) + 1);
      //printf("other_size = %zu\n", other_size);
      //printf("s = ^%.*s$\n", (int)(exp.content_size), exp.content_start);
      //exp.end = exp.start + other_size - exp_intro[index_exp_type];
      //exp.size = 2 * (exp.start - s) + other_size;
      if (exp.type == CMD2)
        exp.type = CMD;
      if (other_size <= 0)
      {
        exp.type = NO_EXPANSION;
        return exp;
      }
      exp.size = other_size + 1;
    }
  }
  else if (in_ari_exp)
  {
    exp.type = NORMAL;
    exp.content_size = tokenize_exp_normal(s);
    exp.size = exp.content_size;
    exp.content_start = s;
  }
  return exp;
}

