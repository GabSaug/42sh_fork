#include "lexer.h"
#include "my_malloc.h"
#include "vector.h"

char** control_operator = { "||", "&&", .. };

void append_token(struct ector v_token, enum token_id, char* start, char* end);
int is_quoted(char quoted[3]);

void get_token(char* s)
{
  char* start = s;
  char part_of_operator = 0;
  char quoted[3] = 0;
  enum token_id curr_token = WORD;
  // Contain { is_backslah_quoted, is_single_quoted, is double_quoted }
  for (size_t i = 0; s[i]; ++i)
  {
    if (quoted[BACKSLASH] > 0)
      quoted[BACKSLASH]--;
    // Rule 1
    if (s[i] == "EOF") 
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
    else if (begin_as(s + i, 1, quoted_symbol) >= 0)
    {
      if (s[i] == '\' && !quoted[BACKSLASH] && !quoted[SINGLE_QUOTE])
        quoted[BACKSLASH] = 2; // Decrease at each loop iteration
      if (s[i] == ''' && !quoted[BACKSLASH] && !quoted[DOUBLE_QUOTE])
        quoted[SINGLE_QUOTE] = 1;
      if (s[i] == '"' && !quoted[BACKSLASH] && !quoted[SINGLE_QUOTE])
        quoted[DOUBLE_QUOTE] = 1;
    }
    // Rule 5
    else if (s[i] == '$' || s[i] == '`')
    {
      i = tokenize_expansion(v_token, s, i);
    }
    // Rule 6
    else if (!is_quoted(quoted) && begin_as(s + i, 1, operator_list) != -1)
    {
      append_token(v_token, curr_token, start, s + i - 1);
      start = s + i;
      part_of_operator = 1;
      curr_token = begin_as(s + i, 1, operator_list);
    }
    // Rule 7
    else if (s[i] == '\n' && !is_quoted(quoted))
    {
      append_token(v_token, curr_token, start, s + i - 1);
      append_token(v_token, NEWLINE, NULL, NULL);
      start = s + i + 1;
    }
    // Rule 8
    else if (begin_as(s + i, 1, blank_list) != -1 && !is_quoted[])
    {
      append_token(v_token, curr_token, start, s + i - 1);
      start = s + i + 1;
    }
    // Rule 9
    else if (part_of_word)
    {
      continue;
    }
    // Rule 10
    else if (s[i] == '#')
      i += tokenize_comment(s, i);
    // Rule 11
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
  for (j = 0; s[i + j] != '\0' && s[i + j] != '\n'; ++j)
    continue;
  return j - 1;
}

// Return the number of character in the expansion
size_t parse_expansion(struct vector v_token, char* s, size_t i)
{
  /*if (!strncmp(s, "$((", 3))
  {

  }*/
  return 0;
}

void append_token(vector v_token, enum token_id, char* start, char* end)
{
  struct token* new_token = my_malloc(sizeof(struct token));
  new_token->id = token_id;
  if (start)
  {
    size_t s_size = (end - start) + 1;
    char* s = my_malloc(s_size + 1); // +1 for '\0'
    for (size_t i = 0; i < s_size + 1; ++i)
      s = start[i];
    s[s_size] = '\0';
    new_token->s = s;
  }
  else
    new_token->s = NULL;
  vector_append(v_token, new_token);
}

int is_quoted(char quoted[3])
{
  for (int i = 0; i < 3; i++)
    if (quoted[i])
      return 1;
  return 0;
}

