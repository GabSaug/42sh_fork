#include <stddef.h>

#include "typer.h"
#include "my_string.h"
#include "lexer.h"
#include "string.h"

static char operator_list[][10] =
{
  ";", "&", "|", "&&", "||", ";;", "<", ">", "<<", ">>", "<&", ">&",
  "<>", "<<-", ">|", ""
};
static char reserved_word[][10] =
{
  "if", "then", "else", "elif", "fi", "do", "done", "case", "esac", "while",
  "until", "for", "{", "}", "(", ")", "!", "in", "function", "\n", ""
};

static int is_digit(char c)
{
  return ('0' <= c && c <= '9');
}

static int is_number(char* s)
{
  for (size_t i = 0; s[i]; ++i)
    if (!is_digit(s[i]))
      return 0;
  return 1;
}

void typer(struct vector* v_token)
{
  struct token* token;
  for (size_t i = 0; i < v_size(v_token); ++i)
  {
    token = v_get(v_token, i);
    if (token->id != UNDIFINED)
      continue;
    if (token->s == NULL)
    {
      token->id = EOF_SYM;
      continue;
    }
    //printf("lexer = %s\n", token->s);
    int index_operator_list = my_is_in(token->s, operator_list);
    if (index_operator_list != -1) // Rule 1
    {
      token->id = index_operator_list;
      continue;
    }
    if (is_number(token->s))
    {
      struct token* next = v_get(v_token, i + 1);
      if (next && next->s && (next->s[0] == '<' || next->s[0] == '>'))
      {
        token->id = IO_NUMBER;
        continue;
      }
    }
    struct token* prev = v_get(v_token, i - 1);
    if (prev == NULL || (prev->id == SEMI || prev->id == AND || prev->id == NL
                 || prev->id == BANG))
    {
      int index_reserved_word = my_is_in(token->s, reserved_word);
      if (index_reserved_word != -1)
      {
        token->id = IF + index_reserved_word;
        continue;
      }
      if (strchr(token->s, '=') > token->s) // = not in 1st position
      {
        token->id = ASSIGNMENT_WORD;
        continue;
      }
    }
    struct token* prev_prev = v_get(v_token, i - 2);
    if (prev_prev && (prev_prev->id == FOR || prev_prev->id == CASE))
    {
      if (my_is_in(token->s, reserved_word) == IN - IF)
      {
        token->id = IN;
        continue;
      }
    }
    token->id = WORD;
    
  }
}
