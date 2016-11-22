#include <stddef.h>

#include "typer.h"
#include "my_string.h"
#include "lexer.h"
#include "my_string.h"
#include "string.h"

static char operator_list[][10] =
{
  ";", "&", "|", "&&", "||", ";;", "<", ">", "<<", ">>", "<&", ">&",
  "<>", "<<-", ">|", "{", "}", "(", ")", "!", ""
};
static char reserved_word[][10] =
{
  "if", "then", "else", "elif", "fi", "do", "done", "case", "esac", "while",
  "until", "for", "in", "function", "\n", ""
};


/*static char operator_list[][10] =
{
  ";", "&", "|", "&&", "||", ";;", "<", ">", "<<", ">>", "<&", ">&",
  "<>", "<<-", ">|", ""
};
static char reserved_word[][10] =
{
  "if", "then", "else", "elif", "fi", "do", "done", "case", "esac", "while",
  "until", "for", "{", "}", "(", ")", "!", "in", "function", "\n", ""
};*/
static enum terminal_symbol before_reserved_word[] =
{
  IF, THEN, ELSE, ELIF, DO, WHILE, UNTIL, IN, PIPE, AND_IF, OR_IF, SEMI, AND, DSEMI, NL
};

/*static enum terminal_symbol before_reserved_word[] =
{
  IF, THEN, ELSE, ELIF, DO, WHILE, UNTIL, L_BRACE, R_BRACE, L_PAR, R_PAR,
  BANG, IN, PIPE, AND_IF, OR_IF, SEMI, AND, DSEMI, NL
};*/

static int is_among(enum terminal_symbol sym, enum terminal_symbol arr[],
                    size_t len)
{
  for (size_t i = 0; i < len; ++i)
    if (sym == arr[i])
      return i;
  return -1;
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
    //if (prev == NULL || (prev->id == SEMI || prev->id == AND || prev->id == NL
  //             || prev->id == BANG))
    size_t len = sizeof (before_reserved_word) / sizeof (*before_reserved_word);
    if (prev == NULL || is_among(prev->id, before_reserved_word, len) != -1)
    {
      int index_reserved_word = my_is_in(token->s, reserved_word);
      if (index_reserved_word != -1)
      {
        token->id = IF + index_reserved_word;
        continue;
      }
      // = not in 1st position
      if (strchr(token->s, '=') > token->s && !is_digit(token->s[0]))
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
