#include <stdio.h>

#include "lexer.h"
#include "my_malloc.h"
#include "my_string.h"

char operator_list[][10] =
{
  "", "\n", ";", "&", "|", "&&", "||", ";;", "<<", ">>", "<&", ">&", "<>",
  "<<-", ">|", "if", "then", "else", "elif", "fi", "do", "done", "case",
  "esac", "while", "until", "for", "{", "}", "(", ")", "!", "in", "function"
};

char quote_symbol[][10] =
{
  "\\", "'", "\""
};

char blank_list[][10] =
{
  "\n", " ", "\t"
};

void append_token(struct vector* v_token, enum token_id, char* start, char* end);
int is_quoted(char quoted[3]);
size_t tokenize_expansion(char* s);
size_t tokenize_comment(char* s, size_t i);

void lexer(char* s, struct vector* v_token)
{
  char* start = s;
  char part_of_operator = 0;
  char part_of_word = 0;
  char quoted[3] = {0};
  enum token_id curr_token = WORD;
  // Contain { is_backslah_quoted, is_single_quoted, is double_quoted }
  for (size_t i = 0; s[i]; ++i)
  {
    if (quoted[BACKSLASH] > 0)
      quoted[BACKSLASH]--;
    // Rule 1
    if (s[i] == EOF)
    {
      if (start == s) // There is not any token
        append_token(v_token, EOF, NULL, NULL);
      else
        append_token(v_token, WORD, start, s + i - 1);
      return;
    }
    // Rule 2
    else if (part_of_operator && !is_quoted(quoted)
             && begin_as(start, s + i, operator_list) != -1)
      curr_token = begin_as(start, s + i, operator_list);
    // Rule 3
    else if (part_of_operator && begin_as(start, s + i, operator_list) == -1)
    {
      append_token(v_token, curr_token, start, s + i - 1);
      start = s + i;
    }
    // Rule 4
    else if (begin_as(s + i, s + i + 1, quote_symbol) >= 0)
    {
      if (s[i] == '\\' && !quoted[BACKSLASH] && !quoted[SINGLE_QUOTE])
        quoted[BACKSLASH] = 2; // Decrease at each loop iteration
      if (s[i] == '\'' && !quoted[BACKSLASH] && !quoted[DOUBLE_QUOTE])
        quoted[SINGLE_QUOTE] = 1;
      if (s[i] == '"' && !quoted[BACKSLASH] && !quoted[SINGLE_QUOTE])
        quoted[DOUBLE_QUOTE] = 1;
    }
    // Rule 5
    else if (s[i] == '$' || s[i] == '`')
    {
      i = tokenize_expansion(s + i);
    }
    // Rule 6
    else if (!is_quoted(quoted) && begin_as(s + i, s + i + 1, operator_list) != -1)
    {
      append_token(v_token, curr_token, start, s + i - 1);
      start = s + i;
      part_of_operator = 1;
      curr_token = begin_as(s + i, s + i + 1, operator_list);
    }
    // Rule 7
    else if (begin_as(s + i, s + i + 1, blank_list) >= 0 && !is_quoted(quoted))
    {
      append_token(v_token, curr_token, start, s + i - 1);
      append_token(v_token, NEWLINE, NULL, NULL);
      start = s + i + 1;
    }
    /*else if (begin_as(s + i, 1, blank_list) != -1 && !is_quoted[])
    {
      append_token(v_token, curr_token, start, s + i - 1);
      start = s + i + 1;
    }*/
    // Rule 8
    else if (part_of_word)
    {
      continue;
    }
    // Rule 9
    else if (s[i] == '#')
      i += tokenize_comment(s, i);
    // Rule 10
    else
    {
      part_of_word = 1;
      curr_token = WORD;
    }
  }
}

// Return the number of character in the comment
size_t tokenize_comment(char* s, size_t i)
{
  size_t j;
  for (j = 0; s[i + j] != '\0' && s[i + j] != '\n'; ++j)
    continue;
  return j - 1;
}

static size_t tokenize_exp_normal(char *s)
{
  size_t i = 2;
  while (s[i] && s[i] != ' ' && s[i] != '\t' && s[i] != '\n' && s[i] != '$')
  {
    if (s[i] == '\\')
      i++;
    else if (s[i] == '\'')
    {
      i++;
      while (s[i] && s[i] != '\'')
        i++;
    }
    else if (s[i] == '\"')
    {
      i++;
      while (s[i] && s[i] != '\"')
        i++;
    }

    i++;
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
    else if (s[i] == '\'')
    {
      i++;
      while (s[i] && s[i] != '\'')
        i++;
    }
    else if (s[i] == '\"')
    {
      i++;
      while (s[i] && s[i] != '\"')
        i++;
    }

    i++;
  }

  return i;
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

void append_token(struct vector* v_token, enum token_id token_id,
                  char* start, char* end)
{
  struct token* new_token = my_malloc(sizeof(struct token));
  new_token->id = token_id;
  if (start)
  {
    size_t s_size = (end - start) + 1;
    char* s = my_malloc(s_size + 1); // +1 for '\0'
    for (size_t i = 0; i < s_size + 1; ++i)
      s[i] = start[i];
    s[s_size] = '\0';
    new_token->s = s;
  }
  else
    new_token->s = NULL;
  v_append(v_token, new_token);
}

int is_quoted(char quoted[3])
{
  for (int i = 0; i < 3; i++)
    if (quoted[i])
      return 1;
  return 0;
}

