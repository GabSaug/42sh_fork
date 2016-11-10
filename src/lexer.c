#include "lexer.h"
#include "my_malloc.h"
#include "vector.h"

char** control_operator = { "||", "&&", .. };

void append_token(vector v_token, enum token_id, char* start, char* end)
{
  struct token* new_token = my_malloc(sizeof(struct token));
  new_token->id = token_id;
  if (start)
  {
    size_t s_size = (end - start) + 1;
    char* s = my_malloc(s_size);
    for (size_t i = 0; i < end - start; ++i)
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

char** get_token(char* s)
{
  char* start = s;
  char part_of_operator = 0;
  char quoted[3] = 0;
  // Contain { is_backslah_quoted, is_single_quoted, is double_quoted }
  for (size_t i = 0; s[i]; ++i)
  {
    if (s[i] == "EOF") // Rule 1
    {
      if (start != s) // There is a current token
        append_token(v_token,
      else
        append_token(v_token, EOF, NULL, NULL);
    }
    else if (part_of_operator && !is_quoted) // Rule 2
    {
      continue;
    }
    // Rule 3
    else if (part_of_operator && !is_in(start, s + i - start, control_operator))
    {
      new_token = create_new_token(start, s + i - 1);
      list_append(token_list, new_token);
      start = s + i;
    }
    // Rule 4
    else if (is_in(s + i, 1, quoted_symbol))
    {
      if (s[i] == '\' && !is_quoted[BACKSLASH] && !is_quoted[SINGLE_QUOTE])
        is_quoted[BACKSLASH] = 1;
      if (s[i] == ''' && !is_quoted[BACKSLASH] && !is_quoted[DOUBLE_QUOTE])
        is_quoted[SINGLE_QUOTE] = 1;
      if (s[i] == '"' && !is_quoted[BACKSLASH] && !is_quoted[SINGLE_QUOTE])
        is_quoted[DOUBLE_QUOTE] = 1;
    }
    // Rule 5
    else if (s[i] == '$' || s[i] == '`')
    {
    }
    // Rule 6
    else if (is_in(s + i, 1, operator_list) && !is_quoted[BACKSLASH]
             && !is_quoted[SINGLE_QUOTE] && !is_quoted[DOUBLE_QUOTE]
             && is_in(s + i, 1, operator_list))
    {
      start = s + i;
      part_of_operator = 1;
    }
    // Rule 7
    else if (s[i] == '\n' && !is_quoted[BACKSLASH] && !is_quoted[SINGLE_QUOTE]
             && !is_quoted[DOUBLE_QUOTE])
    {
      
    }
    // Rule 8
    else if (!is_in(s + i, 1, blank_list) && !is_quoted[])
    {
      start, s + i - 1
    }
    // Rule 9
    else if (part_of_word)
    {
      continue;
    }
    // Rule 10
    else if (s[i] == '#')
    {
      
    }
    // Rule 11
    else
      part_of_word = 1;

  }
}
