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

static char g_special_parameter[] = "@*#?-$!0";

// Return the number of character in the expansion
size_t tokenize_exp_normal(char *s)
{
  size_t i;
  char quoted[3] = { 0 };
  if (!is_name_char(s[0]))
  {
    if (strchr(g_special_parameter, s[0]))
      return 1;
    return 0;
  }
  if (is_digit(s[0]))
    return 1;
  for (i = 1; s[i] && is_name_char(s[i]); ++i)
  {
    for (; is_quoted(quoted); ++i)
      update_quote(s[i], quoted);
  }
  return i;
}

static int check_end_pattern(char* s, size_t i, char d, int nb_char_intro)
{
  for (int j = 0; j < nb_char_intro; ++j)
    if (s[i - j] !=  d)
    {
      warnx("End pattern not found");
      return 0;
    }
  return 1;
}

// Modify so that $(( abc ) def ) triggers an error
static size_t tokenize_exp_other(char *s, char b, char d, char** start,
                                 size_t *content_size, int nb_char_intro)
{
  int count = 0;
  size_t i = 0;
  while (s[i] != b)
    ++i;
  char quoted[3] = { 0 };
  for (int j = 0; j < nb_char_intro; ++count, ++i, ++j)
    update_quote(s[i], quoted);
  for (*start = s + i; s[i] && count > 0; ++i)
  {
    update_quote(s[i], quoted);
    if (is_quoted(quoted))
      continue;
    if (s[i] == b)
      count++;
    else if (s[i] == d)
      if (--count == 0)
        break;
  }
  if (!check_end_pattern(s, i, d, nb_char_intro))
    return 0;

  *content_size = (s+i) - *start - nb_char_intro + 1;

  if (!s[i])
    warnx("Unexpected EOF, expected %c;", d);
  return s[i] ? i : 0;
}

static size_t tokenize_exp_b_quote(char *s, char** start, size_t *content_size)
{
  char quoted[3] = { 0 };
  update_quote(s[0], quoted);
  *start = s + 1;
  size_t i;
  for (i = 1; s[i]; ++i)
  {
    update_quote(s[i], quoted);
    if (!is_quoted(quoted) && s[i] == '`')
      break;
  }
  *content_size = (s + i) - *start;
  if (is_quoted(quoted))
  {
    warnx("Unexpected EOF, expected %c;", s[0]);
    return 0;
  }
  return i + 1;
}

static size_t tokenize_exp_quote(char *s, char** start, size_t *content_size)
{
  char quoted[3] = { 0 };
  update_quote(s[0], quoted);
  *start = s + 1;
  size_t i;
  for (i = 1; s[i]; ++i)
  {
    update_quote(s[i], quoted);
    if (!is_quoted(quoted))
      break;
  }
  *content_size = (s + i) - *start;
  if (is_quoted(quoted))
  {
    warnx("Unexpected EOF, expected %c;", s[0]);
    return 0;
  }
  return i + 1;
}

// must be null-terminated
static char exp_intro[][10] =
{
  "$((",
  "${",
  "$(",
  "`",
  "$",
  "'",
  "\"",
  ""
};

static char exp_begin[] =
{
  '(', // ARI
  '{', // BRACKET
  '(', // CMD
  '`', // CMD2
};

static char exp_end[] =
{
  ')',
  '}',
  ')',
  '`',
};

static struct expansion tokenize_expansion_aux(struct expansion exp, char* s)
{
  size_t size = 0;
  if (exp.type == NORMAL)
  {
    exp.content_size = size = tokenize_exp_normal(s + 1);
    exp.size = exp.content_size + 1;
    exp.content_start = s + 1;
  }
  else if (exp.type < CMD2)
  {
    size = tokenize_exp_other(s, exp_begin[exp.type], exp_end[exp.type],
        &exp.content_start, &exp.content_size, (exp.type == ARI) + 1);
    exp.size = size + 1;
  }
  else if (exp.type == CMD2)
  {
    size = tokenize_exp_b_quote(s, &exp.content_start, &exp.content_size);
    exp.type = CMD;
    exp.size = size;
  }
  else // quote
    exp.size = tokenize_exp_quote(s, &exp.content_start,
        &exp.content_size);
  if (size == 0)
    exp.type = NO_EXPANSION;
  return exp;
}

// Return the number of character in the expansion
struct expansion tokenize_expansion(char* s, int in_ari_exp)
{
  struct expansion exp;

  exp.type = is_prefix_arr(s, exp_intro);
  if (exp.type != NO_EXPANSION)
    exp = tokenize_expansion_aux(exp, s);
  else if (in_ari_exp)
  {
    exp.type = NORMAL;
    exp.content_size = tokenize_exp_normal(s);
    if (exp.content_size == 0 || is_digit(s[0]))
      exp.type = NO_EXPANSION;
    exp.size = exp.content_size;
    exp.content_start = s;
  }
  if (exp.type == NO_EXPANSION)
  {
    exp.size = 0;
    exp.content_start = NULL;
  }
  return exp;
}
