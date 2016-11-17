#include <stdio.h>

#include "lexer.h"
#include "my_malloc.h"
#include "my_string.h"

char operator_list[][10] =
{
  ";", "&", "|", "&&", "||", ";;", "<", ">", "<<", ">>", "<&", ">&",
  "<>", "<<-", ">|", ""
};
char reserved_word[][10] =
{
  "if", "then", "else", "elif", "fi", "do", "done", "case", "esac", "while",
  "until", "for", "{", "}", "(", ")", "!", "in", "function", "\n", ""
};

char quote_symbol[][10] =
{
  "\\", "'", "\"", "\0"
};

char blank_list[][10] =
{
  "\n", " ", "\t"
};

void append_token(struct vector* v_token, enum terminal_symbol id, char* start, char* end);
int is_quoted(char quoted[3]);
size_t tokenize_expansion(char* s);
size_t tokenize_comment(char* s, size_t i);

int lexer(char* s, struct vector* v_token)
{
  char* start = s;
  char part_of_operator = 0;
  char part_of_word = 0;
  char quoted[3] = {0};
  enum terminal_symbol curr_token = UNDIFINED;
  // Contain { is_backslah_quoted, is_single_quoted, is double_quoted }
  size_t i;
  for (i = 0; s[i]; ++i)
  {
    v_print(v_token);
    if (quoted[BACKSLASH] > 0)
      quoted[BACKSLASH]--;
    // Rule 1 is handle after the for loop
    // Rule 2
    if (part_of_operator && !is_quoted(quoted)
             && begin_as(start, s + i, operator_list) != -1)
    {
      curr_token = begin_as(start, s + i, operator_list);
      continue;
    }
    // Rule 3
    if (part_of_operator && begin_as(start, s + i, operator_list) == -1)
    {
      if (is_in(start, s + i - 1, operator_list) != -1)
      {
        append_token(v_token, curr_token, start, s + i - 1);
        curr_token = UNDIFINED;
        start = s + i;
        part_of_operator = 0;
      }
      else
        part_of_operator = 0;
    }
    // Rule 4
    if (begin_as(s + i, s + i, quote_symbol) >= 0) // inverse boolean
    {
      if (s[i] == '\\' && !quoted[BACKSLASH] && !quoted[SINGLE_QUOTE])
        quoted[BACKSLASH] = 2; // Decrease at each loop iteration
      if (s[i] == '\'' && !quoted[BACKSLASH] && !quoted[DOUBLE_QUOTE])
        quoted[SINGLE_QUOTE] = 1;
      if (s[i] == '"' && !quoted[BACKSLASH] && !quoted[SINGLE_QUOTE])
        quoted[DOUBLE_QUOTE] = 1;
    }
    // Rule 5
    if ((s[i] == '$' || s[i] == '`') && !quoted[BACKSLASH] && !quoted[SINGLE_QUOTE])
    {
      int ret = tokenize_expansion(s + i);
      if (ret == 0)
        return 0;
      i += ret - 1;
      continue;
    }
    // Rule 6
    if (!is_quoted(quoted) && begin_as(s + i, s + i, operator_list) != -1)
    {
      append_token(v_token, curr_token, start, s + i - 1);
      start = s + i;
      part_of_operator = 1;
      curr_token = begin_as(s + i, s + i, operator_list);
      continue;
    }
    // Rule 7
    if (begin_as(s + i, s + i, blank_list) >= 0 && !is_quoted(quoted))
    {
      append_token(v_token, curr_token, start, s + i - 1);
      if (s[i] == '\n')
        append_token(v_token, NL, NULL, NULL);
      part_of_operator = 0;
      part_of_word = 0;
      curr_token = UNDIFINED;
      start = s + i + 1;
      continue;
    }
    // Rule 8
    if (part_of_word)
      continue;
    // Rule 9
    if (s[i] == '#')
      i += tokenize_comment(s, i);
    // Rule 10
    else
    {
      part_of_word = 1;
      curr_token = WORD;
    }
  }
  if (i != 0) // There is not any token
    append_token(v_token, curr_token, start, s + i - 1);
  append_token(v_token, EOF_SYM, NULL, NULL);
  return 1;
}

// Return the number of character in the comment
size_t tokenize_comment(char* s, size_t i)
{
  size_t j;
  for (j = 0; s[i + j] != '\0' && s[i + j] != '\n'; ++j)
    continue;
  return j - 1;
}

// Return the number of character in the expansion
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
      while (s[i] && s[i] != '\'' && s[i - 1] != '\\')
        i++;
    }
    else if (s[i] == '\"')
    {
      i++;
      while (s[i] && s[i] != '\"' && s[i - 1] != '\\')
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
    else if (s[i] == '\'' || s[i] == '\"')
    {
      char c = s[i];
      i++;
      while (s[i] && s[i] != c)
        i++;
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

void append_token(struct vector* v_token, enum terminal_symbol token_id,
                  char* start, char* end)
{
  if (token_id == UNDIFINED)
    return;
  struct token* new_token = my_malloc(sizeof(struct token));
  new_token->id = token_id;
  if (token_id == WORD)
  {
    int index_reserved_word = is_in(start, end, reserved_word);
    if (index_reserved_word != -1)
    {
      new_token->id = IF + index_reserved_word;
      new_token->s = NULL;
    }
    else
    {
      size_t s_size = (end - start) + 1;
      char* s = my_malloc(s_size + 1); // +1 for '\0'
      for (size_t i = 0; i < s_size + 1; ++i)
        s[i] = start[i];
      s[s_size] = '\0';
      new_token->s = s;
    }
  }
  v_append(v_token, new_token);
}

int is_quoted(char quoted[3])
{
  for (int i = 0; i < 3; i++)
    if (quoted[i])
      return 1;
  return 0;
}

